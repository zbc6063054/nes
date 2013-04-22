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


void mem_dump(u16 addr){
	printf("*********************** memory dump *****************\n");
	printf("%04X:", addr);
	for(u16 i=0; i<16; ++i){
		printf("%02X", mem_read_byte(addr+i));
		if(i == 7)
			printf("~");
		else
			printf(" ");
	}
	printf("\n");
}
