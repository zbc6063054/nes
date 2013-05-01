#pragma once

#define STACK_ADDR(x) (0x100|(u16)(x))


struct struct_cpu {
	u8 reg_a;						//register a
	u8 reg_x;						//register b
	u8 reg_y;						//register c
	u8 reg_s;

	u8 reg_flag;					//flag
	bool isNMI;
	bool isIRQ;
	bool isRuning;
	u16 reg_pc;						//register pc
};

enum register_type{
	reg_type_a,
	reg_type_x,
	reg_type_y
};

void push_byte(u8);
u8 pop_byte();
void push_word(u16);
u16 pop_word();

int cpu_init();
int cpu_run(int);
int exec(u8);
void cpu_dump();

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
void op_TXS();
void op_TSX();
void op_PHA();
void op_PLA();
void op_PHP();
void op_PLP();
u8 op_ASL(u8);
u8 op_LSR(u8);
u8 op_ROL(u8);
u8 op_ROR(u8);
void op_ADC(u8);
void op_SBC(u8);
void op_CMP(u8);
void op_CPX(u8);
void op_CPY(u8);
u8 op_INC(u8);
void op_INX();
void op_INY();
u8 op_DEC(u8);
void op_DEX();
void op_DEY();
void op_SEC();
void op_SED();
void op_SEI();
void op_CLC();
void op_CLD();
void op_CLV();
void op_CLI();
void op_AND(u8);
void op_ORA(u8);
void op_EOR(u8);
void op_BIT(u8);
void op_JMP(u16);
void op_BCC();
void op_BCS();
void op_BEQ();
void op_BNE();
void op_BVC();
void op_BVS();
void op_BMI();
void op_BPL();
void op_JSR();
void op_RTS();


u16 addr_imm();
u16 addr_abs();
u16 addr_zero();
u16 addr_reg(u8 reg);
u16 addr_zero_reg(u8 reg);
u16 addr_abs_reg(u8 reg);
u16 addr_rel(u8);
u16 addr_ind_x();
u16 addr_ind_y();

extern struct struct_cpu cpu;
