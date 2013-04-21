
#include "../global.h"

#include "memory.h"
#include "cpp.h"

struct struct_cpu cpu;

int cpu_init(){
	cpu.reg_a=(u8)0;
	cpu.reg_x=(u8)0;
	cpu.reg_y=(u8)0;

	cpu.reg_pc=(u16)0;
	cpu.reg_flag=(u8)0;
	return 0;
}

BYTE op_read_byte(){
	BYTE byte = mem_read_byte(cpu.reg_pc);
	++cpu.reg_pc;
	return byte;
}

u16 op_read_word(){
	u16 word = mem_read_word(cpu.reg_pc);
	cpu.reg_pc += 2;
	return word;
}

int op(){
	
	//get next code
	//
	switch(opcode){

	}
	
	//move pc
}

//Immediate Addressing
BYTE addr_imm(){
	return op_read_byte();	
}

//Direct Addressing
BYTE addr_abs(){
	u16 addr = op_read_word();
	return mem_read_byte(addr);
}

//Zero page Addressing 
BYTE addr_zero(){
	u8 addr = op_read_byte();
	return mem_read_byte((u16)addr);
}

//Register Addressing
BYTE addr_reg(u8 reg){
	return mem_read_byte((u16)reg);
}

//Zero page Indexed Addressing 
BYTE addr_zero_reg(u8 reg){
	u16 addr = (u16)op_read_byte();
	addr += reg;
	return mem_read_byte(addr);
}

//Direct Indexed Addressing 
BYTE addr_abs_reg(u8 reg){
	u16 addr = op_read_word();
	addr += reg;
	return mem_read_byte(addr);
}

//
BYTE addr_ind_x(){
	u16 addr = (u16)op_read_byte();
	addr += cpu.reg_x;
	addr = mem_read_word(addr);
	return mem_read_byte(addr);
}

BYTE addr_ind_y(){
	u16 addr = (u16)op_read_byte();
	addr = mem_read_word(addr);
	addr += cpu.reg_y;
	return mem_read_byte(addr);
}

//


