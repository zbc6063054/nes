#pragma once


struct struct_cpu {
	u8 reg_a;						//register a
	u8 reg_x;						//register b
	u8 reg_y;						//register c
	u8 reg_s;

	u16 reg_pc;						//register pc
	u8 reg_flag;					//flag
};

enum register_type{
	reg_type_a,
	reg_type_x,
	reg_type_y
};

void op_LDA(u8);
void op_LDX(u8);
void op_LDY(u8);
void op_STA(u16);
void op_STX(u16);
void op_STY(u16);
void op_TAX();
void op_TXA();
void op_TAY();
void op_TYA();
void op_TXY();
void op_TYX();
void op_PHA();
void op_PLA();
void op_PHP();
void op_PLP();

BYTE addr_imm();
BYTE addr_abs();
BYTE addr_zero();
BYTE addr_zero();
BYTE addr_reg(u8 reg);
BYTE addr_zero_reg(u8 reg);
BYTE addr_abs_reg(u8 reg);
BYTE addr_ind_x();
BYTE addr_ind_y();

extern struct struct_cpu cpu;
