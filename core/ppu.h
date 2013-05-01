#pragma once

#include "../global.h"
#include "memory.h"

#define CYCLES_SCANLINE 1364
#define TOTAL_SCAMLINE	262

typedef struct {
	u8 reg_ctrl1;		//$2000 W
	u8 reg_ctrl2;		//$2001 W
	u8 reg_status;		//$2002 R
	u8 reg_spr_addr;	//$2003 W
	u16 reg_vram_addr;	//$2006 W
	u16 reg_temp;		//temp addressing
	bool toggle;
	u8 vram_buf;		//for the ppu buffer bug
	u8 offset_x;
} StructPPU;

