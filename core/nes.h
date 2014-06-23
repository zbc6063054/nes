#ifndef NES_H
#define NES_H

#include "../global.h"
#include "memmap.h"
#include "ppu.h"
#include "rom.h"
#include "log.h"
#include "threadutil.h"

#ifdef RGB
#undef RGB
#endif
#define RGB(r,g,b) 	((r<<16)|(g<<8)|b|0xFF000000)

class Cpu;
class PPU;
class MemMap;
class Rom;
class Controller;
class EmuThread;

class Nes
{
public:
    Nes();
    virtual ~Nes();

private:
    static u32 sysColorMap[];

public:
    Cpu *cpu;
    PPU *ppu;
    MemMap *map;
    Rom *rom;
    Controller *control;
    EmuThread *thread;

private:
    float fps;
    int fpsLoop;
    timemark fpsStart;
    timemark fpsEnd;
	long totalCycles;
	int currLine;

	void _reset();
	void _dump();

public:
    inline u8 readByte(u16 addr){
        return map->readByte(addr);
    }
    inline void writeByte(u16 addr, u8 byte){
        map->writeByte(addr, byte);
    }
    inline u16 readWord(u16 addr){
        return map->readWord(addr);
    }
    inline void writeWord(u16 addr, u16 word){
        return map->writeWord(addr, word);
    }

    inline u32 getColor(const u8 index){
        if(index >= 64){
            LOGW("color index [%d] is out of range!\n", index);
            return 0;
        }
        return sysColorMap[index];
    }
    inline const u32* getScreenData(){
        return ppu->getScreenData();
    }
    inline int loadFile(const char *filename){
        return rom->readFile(filename);
    }
    inline float getFps(){
        return fps;
    }
	inline long getTotalCycles(){
		return totalCycles;
	}
    void setRefreshFunc(void (* func)(void *), void *data);

    void init();
    void reset();
    void exeFrame();
    void start();
    void stop();
    void pause();
    void resume();
    bool isRunning();
	void dump();
};

#endif // NES_H
