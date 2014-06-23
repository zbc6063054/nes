#include <QString>
#include <QStringListModel>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QScrollBar>
#include <QMessageBox>

#include "debugdialog.h"
#include "dataview.h"
#include "memory.h"
#include "../core/log.h"
#include "../core/nes.h"
#include "../core/cpu.h"
#include "../core/ppu.h"
#include "../core/debug.h"
#include "../core/asm.h"

#define STATESTR(s, f, x)   tr(s).append(QString().sprintf(f, x))

//the opsize table is used to quickly grab the instruction sizes (in bytes)
const u8 opsize[256] = {
/*0x00*/	1,2,0,0,0,2,2,0,1,2,1,0,0,3,3,0,
/*0x10*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0,
/*0x20*/	3,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,
/*0x30*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0,
/*0x40*/	1,2,0,0,0,2,2,0,1,2,1,0,3,3,3,0,
/*0x50*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0,
/*0x60*/	1,2,0,0,0,2,2,0,1,2,1,0,3,3,3,0,
/*0x70*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0,
/*0x80*/	0,2,0,0,2,2,2,0,1,0,1,0,3,3,3,0,
/*0x90*/	2,2,0,0,2,2,2,0,1,3,1,0,0,3,0,0,
/*0xA0*/	2,2,2,0,2,2,2,0,1,2,1,0,3,3,3,0,
/*0xB0*/	2,2,0,0,2,2,2,0,1,3,1,0,3,3,3,0,
/*0xC0*/	2,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,
/*0xD0*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0,
/*0xE0*/	2,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,
/*0xF0*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0
};

extern Nes nes;

DebugDialog* DebugDialog::currentDebugDialog = NULL;

DebugDialog::DebugDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    if(currentDebugDialog == NULL){
        currentDebugDialog = this;
    }else{
        LOGW("already has a Debug Dialog now!\n");
    }
    modelCpu = NULL;
    modelPpu = NULL;
    modelSource = NULL;
    modelBreak = NULL;
    addrSourceStart = 0;
    addrSourceEnd = 0;

    connect(btnRefresh, SIGNAL(clicked()), this, SLOT(refreshData()));
    connect(btnRest, SIGNAL(clicked()), this, SLOT(emuReset()));
    connect(btnOpenFile, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(btnStep, SIGNAL(clicked()), this, SLOT(cpuStep()));
    connect(this, SIGNAL(notifyDataChange()), this, SLOT(refreshData()));
    connect(checkPpuVLBANK, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxClicked()));
    connect(checkPpuHIT0, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxClicked()));
    connect(checkPpuBGVS, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxClicked()));
    connect(checkPpuSPVS, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxClicked()));
    connect(btnAddBreak, SIGNAL(clicked()), this, SLOT(onAddBreak()));
    connect(btnRemoveBreak, SIGNAL(clicked()), this, SLOT(onRemoveBreak()));
    connect(btnEmuBreak, SIGNAL(clicked()), this, SLOT(onEmuBreakControl()));
    connect(listSource, SIGNAL(clicked(QModelIndex)), this, SLOT(onListSourceSelect(QModelIndex)));
    connect(listBreak, SIGNAL(clicked(QModelIndex)), this, SLOT(onListBreakSelect(QModelIndex)));

    debug::set_notify_func(debugBreak, this);
//    refreshData();
}

void DebugDialog::closeEvent(QCloseEvent *){
    if(currentDebugDialog == this)
        currentDebugDialog = NULL;
}

void DebugDialog::refreshCpu(){
    if(nes.cpu->isRunning){
        if(debug::isBreaked()){
            labelCpustate->setStyleSheet("color:red");
            labelCpustate->setText("Breaked");
        }else{
            labelCpustate->setStyleSheet("color:green");
            labelCpustate->setText("Running");
        }
    }else{
        labelCpustate->setStyleSheet("color:red");
        labelCpustate->setText("Stopped");
    }

	QString buf;
	for(u8 i = nes.cpu->reg_s+1; i!=0; ++i){
		buf += QString().sprintf("%02X ", nes.readByte(STACK_ADDR(i)));
	}
	txtStack->setText(buf);
	labelCycles->setText(QString().sprintf("%d", nes.cpu->cycles + nes.getTotalCycles()));
    if(modelCpu == NULL){
        modelCpu = new QStringListModel();
        listCpu->setModel(modelCpu);
    }
    modelCpu->removeRows(0, modelCpu->rowCount());
    QStringList strList;
    strList += STATESTR("a :            ", "%02X", nes.cpu->reg_a);
    strList += STATESTR("x :            ", "%02X", nes.cpu->reg_x);
    strList += STATESTR("y :            ", "%02X", nes.cpu->reg_y);
    strList += STATESTR("s :            ", "%02X", nes.cpu->reg_s);
    strList += STATESTR("pc :         ", "%02X", nes.cpu->reg_pc);
    modelCpu->setStringList(strList);

    checkCpuC->setChecked(BIT(nes.cpu->reg_flag, FLAG_C));
    checkCpuZ->setChecked(BIT(nes.cpu->reg_flag, FLAG_Z));
    checkCpuI->setChecked(BIT(nes.cpu->reg_flag, FLAG_I));
    checkCpuD->setChecked(BIT(nes.cpu->reg_flag, FLAG_D));
    checkCpuB->setChecked(BIT(nes.cpu->reg_flag, FLAG_B));
    checkCpuV->setChecked(BIT(nes.cpu->reg_flag, FLAG_V));
    checkCpuN->setChecked(BIT(nes.cpu->reg_flag, FLAG_N));
}

void DebugDialog::refreshPpu(){
    checkPpuNMI->setChecked(nes.ppu->reg_ctrl1 & C1_NMI);
    checkPpuSP16->setChecked(nes.ppu->reg_ctrl1 & C1_SP16);
    checkPpuADINC32->setChecked(nes.ppu->reg_ctrl1 & C1_ADDRINC32);
    checkPpuBGVS->setChecked(nes.ppu->reg_ctrl2 & C2_BGVS);
    checkPpuSPVS->setChecked(nes.ppu->reg_ctrl2 & C2_SPVS);
    checkPpuVLBANK->setChecked(nes.ppu->reg_status & STATUS_VBLANK);
    checkPpuHIT0->setChecked(nes.ppu->reg_status & STATUS_HIT0);

    txtPpuCtl1->setText(QString().sprintf("%02X", nes.ppu->reg_ctrl1));
    txtPpuCtl2->setText(QString().sprintf("%02X", nes.ppu->reg_ctrl2));
    txtPpuState->setText(QString().sprintf("%02X", nes.ppu->reg_status));
}

void DebugDialog::refreshSource(){
    if(modelSource == NULL){
        modelSource = new QStringListModel();
        listSource->setModel(modelSource);
#ifdef WIN32
		listSource->setFont(QFont("Courier New"));
#else
		listSource->setFont(QFont("Andale Mono"));
#endif
		addrSourceStart = debug::getLastPc();
        readSource(NULL, debug::getLastPc(), debug::getLastPc()+30, 0);
        std::map<int, int>::iterator it = mapSource.find(nes.cpu->reg_pc);
        if(it != mapSource.end()){
            listSource->setCurrentIndex(modelSource->index(it->second, 0));
        }
        return;
    }
    u16 addr = debug::getLastPc();
    if( addr < addrSourceStart ){
        readSource(sourceFileName, addr, addrSourceStart, 0);
        listSource->verticalScrollBar()->setMinimum(addrSourceStart);
        listSource->verticalScrollBar()->setMaximum(addrSourceEnd);
    }else if( addr >= addrSourceEnd ){
        readSource(sourceFileName, addrSourceEnd, addr+30, modelSource->rowCount());
        listSource->verticalScrollBar()->setMinimum(addrSourceStart);
        listSource->verticalScrollBar()->setMaximum(addrSourceEnd);
    }
    std::map<int, int>::iterator it = mapSource.find(nes.cpu->reg_pc);
    if(it != mapSource.end()){
        listSource->setCurrentIndex(modelSource->index(it->second, 0));
    }
}

void DebugDialog::refreshBreak(){
    if(modelBreak == NULL){
        modelBreak = new QStringListModel();
        listBreak->setModel(modelBreak);
    }
    modelBreak->removeRows(0, modelBreak->rowCount());

    u16 arr[MAX_BREAK_POINTS];
    int len = debug::get_breakarr(arr);
    QStringList list;
    for(int i=0; i<len; ++i){
        list.append(QString().sprintf("%04X", arr[i]));
    }
    modelBreak->setStringList(list);
}

void DebugDialog::refreshButtonState(){
    if(nes.cpu->isRunning){
        if(!btnEmuBreak->isEnabled()){
            btnEmuBreak->setEnabled(true);
        }
        if(debug::isBreaked()){
            btnEmuBreak->setText(tr("Resume"));
        }else{
            btnEmuBreak->setText(tr("Pause"));
        }
    }else{
        btnEmuBreak->setEnabled(false);
    }
}

void DebugDialog::emuReset(){
    debug::cpu_wakeup();
    nes.reset();
    debug::break_cpu();
    nes.start();
}

void DebugDialog::refreshData(){
    refreshCpu();
    refreshPpu();
    refreshSource();
    refreshBreak();
    refreshButtonState();
    repaint();
}

int DebugDialog::readSource(const QString &filename, int addrStart, int addrEnd, int insertPos){
//    QFile file(filename);
//    if(!file.open(QFile::ReadOnly | QFile::Text)){
//        LOGE("open source file failed! \n");
//        return -1;
//    }
//    sourceFileName = filename;
//    QTextStream reader(&file);
//    QString buf, null, buf_addr;
//    bool find = false;
//    QStringList list = modelSource->stringList();
//    while( !( buf = reader.readLine() ).isNull() ){
//        u32 addr = 0;
//        QTextStream out(&buf);
//        buf_addr = buf.left(buf.indexOf(":"));
//        bool ok = false;
//        addr = buf_addr.toUInt(&ok, 16);
//        if(!ok){
//            LOGE("read source address failed!\n");
//        }
//        out >> null;
//        addr += -16 + 0x8000;                        //the nes header
//        if( !find ){
//            if( addr > addrStart){
//                find = true;
//                char hex[8], op[8], num[16];
//                out >> hex >> op >> num;
//                mapSource.insert(std::pair<int, int>(addr, insertPos));
//                list.insert(insertPos++, QString().sprintf("%04X:%-7s %-4s %-8s", addr, hex, op, num));
//                addrSourceStart = addrSourceStart < addr ?
//                            addrSourceStart : addr;
//                addrSourceEnd = addrSourceEnd > addr ?
//                            addrSourceEnd : addr;
//            }
//        }else{
//            if( addr < addrEnd){
//                char hex[8], op[8], num[16];
//                out >> hex >> op >> num;
//                mapSource.insert(std::pair<int, int>(addr, insertPos));
//                list.insert(insertPos++, QString().sprintf("%04X:%-7s %-4s %-8s", addr, hex, op, num));
//                addrSourceEnd = addrSourceEnd > addr ?
//                            addrSourceEnd : addr;
//            }
//        }
//    }
    u16 addr = addrStart;
    addrSourceStart = addrSourceStart < addr ?
                      addrSourceStart : addr;
    QStringList list = modelSource->stringList();
    while(addr < addrEnd){
//        ASSERT(addr >= 0x8000);
        u8 code[] = { nes.readByte(addr), nes.readByte(addr + 1), nes.readByte(addr + 2) };
        char* string = Disassemble(addr, code);
        mapSource.insert(std::pair<int, int>(addr, insertPos));
        list.insert(insertPos++, QString().sprintf("%04X:    %s", addr, string));
		addr += opsize[code[0]] == 0 ? 1:opsize[code[0]];
    }
    addrSourceEnd = addrSourceEnd > addr ?
                addrSourceEnd : addr;
    modelSource->removeRows(0, modelCpu->rowCount());
    modelSource->setStringList(list);
    return 0;
}

void DebugDialog::openFile(){
    QString file = QFileDialog::getOpenFileName(this, tr("open nes source file"),
                                 tr("~"), tr("source file (*.txt)"));
    if( !file.isNull() ){
        refreshSource();
        readSource(file, nes.cpu->reg_pc - 1, nes.cpu->reg_pc+30, 0);
    }
}

void DebugDialog::cpuStep(){
    debug::cpu_step();
    refreshData();
}

void DebugDialog::debugBreak(void *data){
    emit ((DebugDialog *)data)->notifyDataChange();
}

void DebugDialog::onCheckBoxClicked(){
    QCheckBox *box = (QCheckBox *)sender();
    u8 *des = NULL;
    int mask = 0;
    bool find = true;
    if(box->text() == tr("VLBANK")){
        des = &(nes.ppu->reg_status);
        mask = STATUS_VBLANK;
    }else if(box->text() == tr("HIT0")){
        des = &(nes.ppu->reg_status);
        mask = STATUS_HIT0;
    }else if(box->text() == tr("BGVS")){
        des = &(nes.ppu->reg_ctrl2);
        mask = C2_BGVS;
    }else if(box->text() == tr("SPVS")){
        des = &(nes.ppu->reg_ctrl2);
        mask = C2_SPVS;
    }else{
        LOGW("unknow check box [%s] clicked! \n", box->text().constData());
        find = false;
    }

    if(find){
        if(box->checkState()){
             (* des) |= mask;
        }else{
             (* des) &= ~mask;
        }
        refreshData();
    }
}

void DebugDialog::onAddBreak(){
    if(!txtBreak->text().isEmpty()){
        bool ok = false;
        u16 addr = txtBreak->text().toUInt(&ok, 16);
        if(ok){
            debug::add_break(addr);
            refreshBreak();
        }
    }
}

void DebugDialog::onRemoveBreak(){
    if(!txtBreak->text().isEmpty()){
        bool ok = false;
        u16 addr = txtBreak->text().toUInt(&ok, 16);
        if(ok){
            debug::remove_break(addr);
            refreshBreak();
        }
    }
}

void DebugDialog::onEmuBreakControl(){
    if(btnEmuBreak->text() == tr("Resume")){
        debug::cpu_wakeup();
        btnEmuBreak->setText(tr("Pause"));
    }else{
        debug::break_cpu();
        btnEmuBreak->setText(tr("Resume"));
    }
    refreshData();
}

void DebugDialog::onListSourceSelect(QModelIndex index){
    QString str = modelSource->data(index, 0).toString();
    txtBreak->setText(str.left(4));
}

void DebugDialog::onListBreakSelect(QModelIndex index){
    QString str = modelBreak->data(index, 0).toString();
    txtBreak->setText(str.left(4));
}
