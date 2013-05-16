#pragma once

#include "../global.h"

#define CYCLES_SCANLINE 140
#define TOTAL_SCAMLINE	262

#define C1_NMI 			0x80
#define C1_SP16        0x20
#define C1_BGCHRADDR 	0x10
#define C1_SPCHRADDR 	0x08
#define C1_ADDRINC32	0x04
#define C1_ADDRNT		0x03

#define C2_BGCOLOR		0xE0
#define C2_SPVS			0x10
#define C2_BGVS			0x08
#define C2_SPCLIP		0x04
#define C2_BGCLIP		0x02
#define C2_COLORTYPE	0x01

#define STATUS_VBLANK	0x80
#define STATUS_HIT0		0x40
#define STATUS_SPCNT	0x20
#define STATUS_VRAM		0x10

#define SP_VTLREV       0x80
#define SP_HTLREV       0x40
#define SP_PRIORITY     0x20
#define SP_HCOLOR       0x03

#define C2_BGCOLOR_BALCK   0x00
#define C2_BGCOLOR_GREEN   0x20
#define C2_BGCOLOR_RED     0x80
#define C2_BGCOLOR_BLUE    0x40

#define BGCOLOR_BALCK   0x1D
#define BGCOLOR_GREEN   0x1A
#define BGCOLOR_RED     0x16
#define BGCOLOR_BLUE    0x12

typedef struct {
    u8 pos_y;
    u8 index;
    u8 attr;
    u8 pos_x;
} Sprite;

class Nes;

class PPU{
public:
    friend class Nes;
    PPU(Nes *parent);

public:
    u8 reg_ctrl1;		//$2000 W
    u8 reg_ctrl2;		//$2001 W
    u8 reg_status;		//$2002 R
    u8 reg_spr_addr;	//$2003 W
    u16 reg_vram_addr;	//$2006 W
    u16 reg_temp;		//temp addressing
    bool toggle;
    u8 vram_buf;		//for the ppu buffer bug
    u8 offset_x;
    u8 currentLine;
    u8 bgColor;

    u32 screen_mem[34*8*32*8];
    u8 revBit[256];
    Nes *nes;


private:
    static u32 sysColorMap[];

public:
    void init();
    void reset();

    void writeReg(u16 addr, u8 byte);
    u8 readReg(u16 addr);

    void scanline();
    void setScanline(int line);
    void renderTile(u8 index, u8 attr, u32 *pScreen, u8* pattle, u16 addr_base);
    void drawPattern(u8 attr, u32 *pScreen);
    void drawSprite(u32 *pScreen);
    void drawNameTable(u32 *pScreen, u8 tableIndex);

    void vBlankStart();
    void vBlankEnd();

    inline const u32* getScreenData(){
        return screen_mem;
    }
};



