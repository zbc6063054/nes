
#include "../global.h"

#include "memory.h"
#include "cpu.h"

#define STACK_ADDR(x) (0x100|(u16)(x))

struct struct_cpu cpu;

int cpu_init(){
	cpu.reg_a = (u8)0;
	cpu.reg_x = (u8)0;
	cpu.reg_y = (u8)0;
	
	cpu.reg_s = (u8)0xff;							//points to the top of stack
	cpu.reg_pc = (u16)0;
	cpu.reg_flag = (u8)0;
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

inline void push(u8 byte){
	mem_write_byte(STACK_ADDR(cpu.reg_s), byte);
	--cpu.reg_s;
}

inline u8 pop(){
	--cpu.reg_s;
	return mem_read_byte(STACK_ADDR(cpu.reg_s));
}

int op(){
	
	//get next code
	u8 opcode = op_read_byte();
	switch(opcode){
	case 0xA9:				//LDA imm	
			
		break;
	}

	return 0;
}

int main(){

	return 0;
}

/***************************         operations          *********************************/

inline void op_LDA(u8 byte){
	cpu.reg_a = byte;
}
inline void op_LDX(u8 byte){
	cpu.reg_x = byte;
}
inline void op_LDY(u8 byte){
	cpu.reg_y = byte;
}

inline void op_STA(u16 addr){
	mem_write_byte(addr, cpu.reg_a);
}
inline void op_STX(u16 addr){
	mem_write_byte(addr, cpu.reg_x);
}
inline void op_STY(u16 addr){
	mem_write_byte(addr, cpu.reg_y);
}

inline void op_TAX(){
	cpu.reg_x = cpu.reg_a;
}
inline void op_TXA(){
	cpu.reg_a = cpu.reg_x;
}
inline void op_TAY(){
	cpu.reg_y = cpu.reg_a;
}
inline void op_TYA(){
	cpu.reg_a = cpu.reg_y;
}
inline void op_TXY(){
	cpu.reg_y = cpu.reg_x;
}
inline void op_TYX(){
	cpu.reg_x = cpu.reg_y;
}

inline void op_PHA(){
	push(cpu.reg_a);
}
inline void op_PLA(){
	cpu.reg_a = pop();
}
inline void op_PHP(){
	push(cpu.reg_flag);
}
inline void op_PLP(){
	cpu.reg_flag = pop();
}


/****************************   Addressing functions    **********************************/

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


