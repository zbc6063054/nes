#include <stdio.h>
#include "../global.h"

#define MEMORY_SIZE	0x10000

BYTE mem[MEMORY_SIZE];

void cpu_access_func(u16, u8&, type);
void ppu_access_func(u16, u8&, type);

StructMemMap mapper[] = {
	0, 0x1FFF, cpu_access_func,
	0x2000, 
}

u8 mem_read_byte(u16 addr){
	int i = 0, len = sizeof(mapper) / sizeof(mapper[0]);
	u8 byte = 0;
	for(i=0; i<len; ++i){
		if(addr>=mapper[i].addr_start && addr<=mapper.addr_end){
			mapper[i].access_func(addr, &byte, MEMREAD);
		}
	}
	return byte;
}

void mem_write_byte(u16 addr, BYTE byte){
	int i = 0, len = sizeof(mapper) / sizeof(mapper[0]);
	for(i=0; i<len; ++i){
		if(addr>=mapper[i].addr_start && addr<=mapper.addr_end){
			mapper[i].access_func(addr, &byte, MEMWRITE);
		}
	}
}

//only for cpu memory
u16 mem_read_word(u16 addr){
	return *(u16 *)(mem + (u32)addr);
}

//need to confirm the order of bytes in memory
void mem_write_word(u16 addr, u16 word){
	*(u16 *)(mem + (u32)addr) = word;
}

void cpu_access_func(u16 addr, u8& byte,AccessType type){
	if(type == MEMREAD){
		byte = mem[addr];
	}else{
		mem[addr] = byte;
	}
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
