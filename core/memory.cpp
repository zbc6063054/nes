#include <stdio.h>
#include <string.h>
#include "../global.h"
#include "memory.h"

#define CPU_MEMORY_SIZE	0x800

u8 cpu_mem[CPU_MEMORY_SIZE];
u8 save_ram[0x2000];
u8 prg_rom[0x8000];
u8 ex_rom[0x1FE0];

u8 chr_rom[0x2000];
u8 nt_ram[0x1000];				//name table
u8 spr_mem[256];				//256 bytes sprite memory
u8 spr_pattle[16];
u8 bg_pattle[16];				//

u8* ppu_mem[12];				//1k block

void cpu_access_func(u16, u8&, AccessType);
void ppu_access_func(u16, u8&, AccessType);
void rom_access_func(u16, u8&, AccessType);
void reg_access_func(u16, u8&, AccessType);

u8 ppu_read_reg(u16);
void ppu_write_reg(u16, u8);

StructMemMap mapper[] = {
	0, 0x1FFF, MEMMAP_FUNCTION, (void *)cpu_access_func,
	0x2000, 0x3FFF, MEMMAP_FUNCTION, (void *)ppu_access_func,
    0x4000, 0x40FF, MEMMAP_FUNCTION, (void *)reg_access_func,
	0x4100, 0x5FFF, MEMMAP_ADDRESS, (void *)ex_rom,
	0x6000, 0x7FFF, MEMMAP_ADDRESS, (void *)save_ram,
	0x4100, 0xFFFF, MEMMAP_FUNCTION, (void *)rom_access_func,
};

void set_mem_block(int start, int size, u8* addr){
	for(int i=0; i<size; ++i){
		ppu_mem[start+i] = addr + 0x400 * i;
	}
}

void mem_init(){
	set_mem_block(0, 8, chr_rom);
	set_nt_mirror(MIRROR_HORIZONTAL);		//just for test
}

void set_nt_mirror(int id1, int id2, int id3, int id4){
	set_mem_block(7, 1, nt_ram+id1*0x400);
	set_mem_block(8, 1, nt_ram+id2*0x400);
	set_mem_block(9, 1, nt_ram+id3*0x400);
	set_mem_block(10, 1, nt_ram+id4*0x400);
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

void mem_access_control(u16 addr, u8& byte, AccessType type){
	int i = 0, len = sizeof(mapper) / sizeof(mapper[0]);
	for(i=0; i<len; ++i){
		if(addr>=mapper[i].addr_start && addr<=mapper[i].addr_end){
			if(mapper[i].type == MEMMAP_FUNCTION){
				((MemAccessFunc)mapper[i].data)(addr, byte, type);
			}else{
				if(type == MEMREAD){
					byte = ((u8 *)mapper[i].data)[addr - mapper[i].addr_start];
				}else{
					((u8 *)mapper[i].data)[addr - mapper[i].addr_start] = byte;
				}
			}
			break;
		}
	}
}

u8 mem_read_byte(u16 addr){
	u8 byte = 0;
	mem_access_control(addr, byte, MEMREAD);
	return byte;
}

void mem_write_byte(u16 addr, BYTE byte){
	mem_access_control(addr, byte, MEMWRITE);
}

//only available for cpu memory
u16 mem_read_word(u16 addr){
	return *(u16 *)(cpu_mem + (u32)addr);
}

//need to confirm the order of bytes in memory
void mem_write_word(u16 addr, u16 word){
	*(u16 *)(cpu_mem + (u32)addr) = word;
}

void cpu_access_func(u16 addr, u8& byte,AccessType type){
	addr = addr & 0x7FF;				//memory mirror
	if(type == MEMREAD){
		byte = cpu_mem[addr];
	}else{
		cpu_mem[addr] = byte;
	}
}

u8 ppu_read_reg(u16);
void ppu_write_reg(u16, u8);

void ppu_access_func(u16 addr, u8& byte, AccessType type){
	addr &= 0xE007;
	if(type == MEMREAD){
		byte = ppu_read_reg(addr);
	}else{
		ppu_write_reg(addr, byte);
	}
}

void DMA(u8 byte){
    memcpy(spr_mem, &cpu_mem[((u16)byte)<<8], 256);
}

void reg_access_func(u16 addr, u8& byte, AccessType type){
    switch(addr){
        case 0x4017:
            DMA(byte);
            break;
        default:
            //TODO
            if(type == MEMREAD){
                byte = 0;
            }
            break;
    }
}

//TODO
void rom_access_func(u16 addr, u8& byte, AccessType type){
	if(type == MEMREAD){
		byte = prg_rom[addr - 0x8000];
	}
	//do nothing when require write
}

void mem_dump(u16 addr_start, u16 addr_end){
	printf("%04X: ", addr_start);
	for(u16 i=0; i < addr_end-addr_start; ++i){
		printf("%02X", mem_read_byte(addr_start+i));
		if((i+1)%16 == 0 && (i+1)!=addr_end-addr_start){
			printf("\n%04X:", addr_start+i+1);
		}
		if((i+1)%16 == 7)
			printf("~");
		else
			printf(" ");
	}
	printf("\n");
}

void mem_dump(u16 addr){
	mem_dump(addr, addr + 0x40);
}
