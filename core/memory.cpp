#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../global.h"
#include "memory.h"

#define CPU_MEMORY_SIZE 0x800
#define ZEROMEMORY(x) memset(x, 0, sizeof(x))

u8 cpu_mem[CPU_MEMORY_SIZE];
u8 save_ram[0x2000];

u8 ex_rom[0x1FE0];

u8 nt_ram[0x1000];				//name table
u8 spr_mem[256];				//256 bytes sprite memory
u8 spr_pattle[16];
u8 bg_pattle[16];

u8* ppu_mem[12];				//12 1k block
u8* prg_rom[32];                //32 1k block

u8* pPRG = NULL;
u8* pCHR = NULL;


// map addr Page from to PPU memory Page start
void set_ppu_block(int start, int size, u8* addr, int from){
    assert( start + size <= LEN(ppu_mem) );
    for(int i=0; i<size; ++i){
        ppu_mem[start+i] = addr + 0x400 * (i + from);
    }
}

// map size k block from addr to ppu start
void set_ppu_block(int start, int size, int from ){
    set_ppu_block(start, size, pCHR, from);
}

void set_prg_block(int start, int size, int addr){
    assert( start + size <= LEN(prg_rom) );
    for(int i=0; i<size; ++i){
        prg_rom[start+i] = pPRG + 0x400 * (addr + i);
    }
}

void mem_init(){
	ZEROMEMORY(ppu_mem);
    ZEROMEMORY(prg_rom);
    ZEROMEMORY(cpu_mem);
    ZEROMEMORY(nt_ram);
}

void set_nt_mirror(int id1, int id2, int id3, int id4){
    set_ppu_block(8, 1, nt_ram, id1);
    set_ppu_block(9, 1, nt_ram, id2);
    set_ppu_block(10, 1, nt_ram, id3);
    set_ppu_block(11, 1, nt_ram, id4);
}

void set_nt_mirror(NtMirrorType type){
	switch(type){
		case MIRROR_HORIZONTAL:
			set_nt_mirror(0, 0, 1, 1);
			break;
		case MIRROR_VERTICAL:
			set_nt_mirror(0, 1, 0, 1);
			break;
		case MIRROR_SINGLE:
			set_nt_mirror(0, 0, 0, 0);
			break;
		case MIRROR_FOUR:
			set_nt_mirror(0, 1, 2, 3);
			break;
		default:
			//do nothing
			break;
	}
}

//void mem_dump(u16 addr_start, u16 addr_end){
//	printf("%04X: ", addr_start);
//	for(u16 i=0; i < addr_end-addr_start; ++i){
//		printf("%02X", mem_read_byte(addr_start+i));
//		if((i+1)%16 == 0 && (i+1)!=addr_end-addr_start){
//			printf("\n%04X:", addr_start+i+1);
//		}
//		if((i+1)%16 == 7)
//			printf("~");
//		else
//			printf(" ");
//	}
//	printf("\n");
//}

//void mem_dump(u16 addr){
//	mem_dump(addr, addr + 0x40);
//}
