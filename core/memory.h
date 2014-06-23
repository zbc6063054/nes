#pragma once

#include "../global.h"
#include <string.h>


#define CHR_ROM(x) chr_rom[(x)>>12][(x)&0x0FFF]

enum NtMirrorType{
	MIRROR_HORIZONTAL = 0,
	MIRROR_VERTICAL = 1,
	MIRROR_SINGLE = 2,
    MIRROR_FOUR = 3
};

extern u8 cpu_mem[];
extern u8 save_ram[];
extern u8* prg_rom[2];              //16K size 0x4000
extern u8 ex_rom[];

extern u8* chr_rom[2];              //4K size  0x10ss00
extern u8 nt_ram[];                 //name table
extern u8 spr_mem[];				//256 bytes sprite memory
extern u8 spr_pattle[];
extern u8 bg_pattle[];				//

extern u8* ppu_mem[];				//1k block

inline void DMA(u8 byte){
    memcpy(spr_mem, &cpu_mem[((u16)byte)<<8], 256*sizeof(u8));
}
void mem_init();

void set_nt_mirror(NtMirrorType type);
void set_ppu_block(int start, int size, u8* addr);

void mem_dump(u16 addr);
void mem_dump(u16 addr_start, u16 addr_end);

#if 0
#define PRG_ROM(x) prg_rom[(x)>>14][(x)&0x3FFF]
#else
inline u8 PRG_ROM(u16 x) {
	int b = x>>14;
	int k = x&0x3FFF;
	return prg_rom[(x)>>14][(x)&0x3FFF];
}
#endif
