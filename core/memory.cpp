
#include "../global.h"

#define MEMORY_SIZE	10000h

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
