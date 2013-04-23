#include <stdio.h>
#include "../global.h"

#define MEMORY_SIZE	0x10000

BYTE mem[MEMORY_SIZE];

u8 mem_read_byte(u16 addr){
	//should judge the range of addr
	return mem[addr];
}

u16 mem_read_word(u16 addr){
	return *(u16 *)(mem + (u32)addr);
}

void mem_write_byte(u16 addr, BYTE byte){
	mem[addr] = byte;
}

//need to confirm the order of bytes in memory
void mem_write_word(u16 addr, u16 word){
	*(u16 *)(mem + (u32)addr) = word;
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
