#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>
#include <QStringList>
#include <map>
#include "../global.h"
#include "ui_debugdialog.h"

#define CACHESIZE 256

class QStringListModel;

class DebugDialog : public QDialog, public Ui::DebugDialog
{
    Q_OBJECT

public:
    explicit DebugDialog(QWidget *parent = 0);

private:
    QStringListModel *modelCpu;
    QStringListModel *modelPpu;
    QStringListModel *modelSource;
    QStringListModel *modelBreak;
    QStringList strListSource;
    QString sourceFileName;
    u16 addrSourceStart;
    u16 addrSourceEnd;
    std::map<int, int> mapSource;
    struct {
        struct {
            char code[8];
            char op[8];
            char num[16];
        } data[CACHESIZE];
        int size;
        int addr_start;
    } sourceCache;

public:
    static DebugDialog *currentDebugDialog;

private:
    void refreshCpu();
    void refreshPpu();
    void refreshSource();
    void refreshBreak();
    void refreshButtonState();

protected:
    void closeEvent(QCloseEvent *);

public:
    int readSource(const QString &filename, int addrStart, int addrEnd, int insertPos = 0);
    static void debugBreak(void *data);

signals:
    void notifyDataChange();
    
public slots:
    void refreshData();
    void emuReset();
    void openFile();
    void cpuStep();
    void onCheckBoxClicked();
    void onRemoveBreak();
    void onAddBreak();
    void onEmuBreakControl();
    void onListSourceSelect(QModelIndex index);
    void onListBreakSelect(QModelIndex index);
};

#endif // DEBUGDIALOG_H
