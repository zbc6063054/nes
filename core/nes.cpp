#include "nes.h"
#include "cpu.h"
#include "ppu.h"
#include "controller.h"
#include "log.h"
#include "emuthread.h"
#define NULL 0

u32 Nes::sysColorMap[]={
    RGB(0x75,0x75,0x75),RGB(0x27,0x1B,0x8F),RGB(0x00,0x00,0xAB),RGB(0x47,0x00,0x9F),
    RGB(0x8F,0x00,0x77),RGB(0xAB,0x00,0x13),RGB(0xA7,0x00,0x00),RGB(0x7F,0x0B,0x00),
    RGB(0x43,0x2F,0x00),RGB(0x00,0x47,0x00),RGB(0x00,0x51,0x00),RGB(0x00,0x3F,0x17),
    RGB(0x1B,0x3F,0x5F),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),
    RGB(0xBC,0xBC,0xBC),RGB(0x00,0x73,0xEF),RGB(0x23,0x3B,0xEF),RGB(0x83,0x00,0xF3),
    RGB(0xBF,0x00,0xBF),RGB(0xE7,0x00,0x5B),RGB(0xDB,0x2B,0x00),RGB(0xCB,0x4F,0x0F),
    RGB(0x8B,0x73,0x00),RGB(0x00,0x97,0x00),RGB(0x00,0xAB,0x00),RGB(0x00,0x93,0x0B),
    RGB(0x00,0x83,0x8B),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),
    RGB(0xFF,0xFF,0xFF),RGB(0x3F,0xBF,0xFF),RGB(0x5F,0x97,0xFF),RGB(0xA7,0x8B,0xFD),
    RGB(0xF7,0x7B,0xFF),RGB(0xFF,0x77,0xB7),RGB(0xFF,0x77,0x63),RGB(0xFF,0x9B,0x3B),
    RGB(0xF3,0xBF,0x3F),RGB(0x83,0xD3,0x13),RGB(0x4F,0xDF,0x4B),RGB(0x58,0xF8,0x98),
    RGB(0x00,0xEB,0xDB),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),
    RGB(0xFF,0xFF,0xFF),RGB(0xAB,0xE7,0xFF),RGB(0xC7,0xD7,0xFF),RGB(0xD7,0xCB,0xFF),
    RGB(0xFF,0xC7,0xDB),RGB(0xFF,0xC7,0xDB),RGB(0xFF,0xBF,0xB3),RGB(0xFF,0xDB,0xAB),
    RGB(0xFF,0xE7,0xA3),RGB(0xE3,0xFF,0xA3),RGB(0xAB,0xF3,0xBF),RGB(0xB3,0xFF,0xCF),
    RGB(0x9F,0xFF,0xF3),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)
};

Nes::Nes()
{
    fps = 0;
    fpsLoop = 0;

    ppu = new PPU(this);
    cpu = new Cpu(this);
    map = new MemMap(this);
    rom = new Rom(this);
    control = new Controller(this);
    thread = new EmuThread(this);
}

Nes::~Nes(){
    delete ppu;
    ppu = NULL;
    delete cpu;
    cpu = NULL;
    delete map;
    map = NULL;
    delete rom;
    rom = NULL;
    delete control;
    control = NULL;
    delete thread;
    thread = NULL;
}

void Nes::init(){
    if( ppu && cpu && map){
        cpu->init();
        ppu->init();
        map->init();
    }
}

void Nes::reset(){
    if( ppu && cpu && map){
        thread->reset();
        control->reset();
        cpu->reset();
        ppu->reset();
        map->reset();
    }
}

void Nes::exeFrame(){
    if(ppu->reg_ctrl2&( C2_SPVS | C2_BGVS )){
        ppu->reg_vram_addr = ppu->reg_temp;
    }
    int line = 0;
    while(true){
        ppu->setScanline(line);
        cpu->run(CYCLES_SCANLINE);
        if(line<240){
           ppu->scanline();
        }
        if(line == 240){
            ppu->vBlankStart();
        }
        if(line == TOTAL_SCAMLINE-1){
            ppu->vBlankEnd();
            break;
        }
        ++line;
    }
    fpsLoop++;
    if(fpsLoop == 40){
        getTimeMark(&fpsEnd);
        int time = diffTimeMark(fpsStart, fpsEnd);
        fps = (float)40*1000 / time;
        fpsLoop = 0;
    }
    if(fpsLoop == 0){
        getTimeMark(&fpsStart);
    }
}

void Nes::setRefreshFunc(void (* func)(void *), void *data){
    thread->setRefreshFunc(func, data);
}

void Nes::resume(){
    thread->resume();
}

void Nes::start(){
    if(!rom->getLoadState()){
        LOGW("need load nes file first!\n");
        return;
    }
    cpu->start();
    thread->start();
}

void Nes::pause(){
    thread->pause();
}

void Nes::stop(){
    thread->stop();
}

bool Nes::isRunning(){
    return thread->getRunState();
}
