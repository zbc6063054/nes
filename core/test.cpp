#include <stdio.h>

#include "../global.h"
#include "memory.h"
#include "cpu.h"

void mem_test(){
	mem_write_byte(0, 16);
	mem_write_word(3, 0x2010);
	mem_dump(0);

	u16 temp=mem_read_word(3);
	printf("%x mem:%x\n", 0x2010, temp);
}

void stack_test(){
	push_byte(5);
	push_byte(10);
	push_byte(20);
	push_word(0x2010);
	mem_dump(STACK_ADDR(cpu.reg_s));
	printf("%X stack %X\n",0x2010,pop_word());
	pop_byte();
	pop_byte();
	printf("%x stack:%x\n", 5, pop_byte());
}

int main(){
	printf("test start!\n");
	
	cpu_init();
	mem_test();
	stack_test();
	return 0;
}
