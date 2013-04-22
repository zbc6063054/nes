#include <stdio.h>
#include "../global.h"

#include "memory.h"
#include "cpu.h"


#define FLAG_C 0
#define FLAG_Z 1
#define FLAG_I 2
#define FLAG_D 3
#define FLAG_B 4
#define	FLAG_V 6 
#define FLAG_N 7

#define SET_FLAG_N(x) (set_flag(FLAG_N, ((x)>>7)&1 > 0))
#define SET_FLAG_Z(x) (set_flag(FLAG_Z, (x) == 0))

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

// 0 1 2 3 4 5 6 7
// C Z I D B   V N
void set_flag(u8 type, bool val){
	if(val){
		cpu.reg_flag |= (u8)(1<<type);
	}else{
		cpu.reg_flag &= ~(u8)(1<<type);
	}
}
inline bool get_flag(u8 type){
	return ((cpu.reg_flag >> type) & 1) > 0;
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

void push_byte(u8 byte){
	mem_write_byte(STACK_ADDR(cpu.reg_s), byte);
	--cpu.reg_s;
}
void push_word(u16 word){
	--cpu.reg_s;
	mem_write_word(STACK_ADDR(cpu.reg_s), word);
	--cpu.reg_s;
}
u16 pop_word(){
	++cpu.reg_s;
	u16 word = mem_read_word(STACK_ADDR(cpu.reg_s));
	++cpu.reg_s;
	return word;
}
u8 pop_byte(){
	++cpu.reg_s;
	return mem_read_byte(STACK_ADDR(cpu.reg_s));
}

void cpu_run(){
	//get next code
	u8 opcode = op_read_byte();
}

int exec(u8 opcode){
	u16 addr = 0;
	u8 val = 0;

	switch(opcode){
	case 0xA9:				//LDA imm	
		op_LDA(op_read_byte());	
		break;
	case 0xA5:				//zero page Addressing
		op_LDA(mem_read_byte(addr_zero()));
		break;
	case 0xB5:				//zero page x indexed addressing
		op_LDA(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0xAD:				//absolute addressing
		op_LDA(mem_read_byte(
				addr_abs()
				));
		break;
	case 0xBD:				//absolute x indexed addressing
		op_LDA(mem_read_byte(
				addr_abs_reg(cpu.reg_x)
				));
		break;
	case 0xB9:				//absolute y indexed addressing
		op_LDA(mem_read_byte(
				addr_abs_reg(cpu.reg_y)
				));
		break;
	case 0xA1:				//x
		op_LDA(mem_read_byte(
				addr_ind_x()
				));
		break;
	case 0xB1:				//y
		op_LDA(mem_read_byte(
				addr_ind_y()
				));
		break;
	/*************LDX************/
	case 0xA2:				//LDX imm
		op_LDX(op_read_byte());
		break;
	case 0xA6:				//zero page Addressing
		op_LDX(mem_read_byte(
				addr_zero()
				));
		break;
	case 0xB6:				//zero page y indexed Addressing
		op_LDX(mem_read_byte(
				addr_zero_reg(cpu.reg_y)
				));
		break;
	case 0xAE:				//absolute Addressing
		op_LDX(mem_read_byte(
				addr_abs()
				));
		break;
	case 0xBE:				//absolute y indexed Addressing 
		op_LDX(mem_read_byte(
				addr_abs_reg(cpu.reg_y)
				));
		break;
	/****************LDY************************/		
	case 0xA0:				//LDY imm
		op_LDY(op_read_byte());
		break;
	case 0xA4:				//zero page Addressing
		op_LDY(mem_read_byte(
				addr_zero()
				));
		break;
	case 0xB4:				//zero page x indexed Addressing
		op_LDY(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0xAC:				//absolute Addressing
		op_LDY(mem_read_byte(
				addr_abs()
				));
		break;
	case 0xBC:				//absolute x indexed Addressing 
		op_LDY(mem_read_byte(
				addr_abs_reg(cpu.reg_x)
				));
		break;
	/**************** STA ***************/
	case 0x85:				//zero page Addressing
		op_STA(addr_zero());
		break;
	case 0x95:				//zero page x indexed addressing
		op_STA(addr_zero_reg(cpu.reg_x));
		break;
	case 0x8D:				//absolute addressing
		op_STA(addr_abs());
		break;
	case 0x9D:				//absolute x indexed addressing
		op_STA(addr_abs_reg(cpu.reg_x));
		break;
	case 0x99:				//absolute y indexed addressing
		op_STA(addr_abs_reg(cpu.reg_y));
		break;
	case 0x81:				//x
		op_STA(addr_ind_x());
		break;
	case 0x91:				//y
		op_STA(addr_ind_y());
		break;
	/**************** STX ***************/
	case 0x86:				//zero page Addressing
		op_STX(addr_zero());
		break;
	case 0x96:				//zero page y indexed addressing
		op_STX(addr_zero_reg(cpu.reg_y));
		break;
	case 0x8E:				//absolute addressing
		op_STX(addr_abs());
		break;
	/**************** STY ***************/
	case 0x84:				//zero page Addressing
		op_STY(addr_zero());
		break;
	case 0x94:				//zero page x indexed addressing
		op_STY(addr_zero_reg(cpu.reg_x));
		break;
	case 0x8C:				//absolute addressing
		op_STY(addr_abs());
		break;

	case 0x8A:
		op_TXA();
		break;
	case 0xAA:
		op_TAX();
		break;
	case 0x98:
		op_TYA();
		break;
	case 0xA8:
		op_TAY();
		break;
	case 0x9A:
		op_TXS();
		break;
	case 0xBA:
		op_TSX();
		break;

	case 0x48:
		op_PHA();
		break;
	case 0x68:
		op_PLA();
		break;
	case 0x08:
		op_PHP();
		break;
	case 0x28:
		op_PLP();
		break;

	/*************** ASL **************/
	case 0x0A:					//register a Addressing
		val = cpu.reg_a;
		val = op_ASL(val);
		mem_write_byte(addr, val);
		break;
	case 0x06:					//zero page Addressing 
		addr = addr_zero();
		val = mem_read_byte(addr);
		val = op_ASL(val);
		mem_write_byte(addr, val);
		break;
	case 0x16:					//zero page x Indexed Addressing
		addr = addr_zero_reg(cpu.reg_x);
		val = mem_read_byte(addr);
		val = op_ASL(val);
		mem_write_byte(addr, val);
		break;
	case 0x0E:					//absolute Addressing 
		addr = addr_abs();
		val = mem_read_byte(addr);
		val = op_ASL(val);
		mem_write_byte(addr, val);
		break;
	case 0x1E:					//absolute x Indexed Addressing
		addr = addr_abs_reg(cpu.reg_x);
		val = mem_read_byte(addr);
		val = op_ASL(val);
		mem_write_byte(addr, val);
		break;

	/*************** LSR **************/
	case 0x4A:					//register a Addressing
		val = cpu.reg_a;
		val = op_LSR(val);
		mem_write_byte(addr, val);
		break;
	case 0x46:					//zero page Addressing 
		addr = addr_zero();
		val = mem_read_byte(addr);
		val = op_LSR(val);
		mem_write_byte(addr, val);
		break;
	case 0x56:					//zero page x Indexed Addressing
		addr = addr_zero_reg(cpu.reg_x);
		val = mem_read_byte(addr);
		val = op_LSR(val);
		mem_write_byte(addr, val);
		break;
	case 0x4E:					//absolute Addressing 
		addr = addr_abs();
		val = mem_read_byte(addr);
		val = op_LSR(val);
		mem_write_byte(addr, val);
		break;
	case 0x5E:					//absolute x Indexed Addressing
		addr = addr_abs_reg(cpu.reg_x);
		val = mem_read_byte(addr);
		val = op_LSR(val);
		mem_write_byte(addr, val);
		break;
	/*************** ROL **************/
	case 0x2A:					//register a Addressing
		val = cpu.reg_a;
		val = op_ROL(val);
		mem_write_byte(addr, val);
		break;
	case 0x26:					//zero page Addressing 
		addr = addr_zero();
		val = mem_read_byte(addr);
		val = op_ROL(val);
		mem_write_byte(addr, val);
		break;
	case 0x36:					//zero page x Indexed Addressing
		addr = addr_zero_reg(cpu.reg_x);
		val = mem_read_byte(addr);
		val = op_ROL(val);
		mem_write_byte(addr, val);
		break;
	case 0x2E:					//absolute Addressing 
		addr = addr_abs();
		val = mem_read_byte(addr);
		val = op_ROL(val);
		mem_write_byte(addr, val);
		break;
	case 0x3E:					//absolute x Indexed Addressing
		addr = addr_abs_reg(cpu.reg_x);
		val = mem_read_byte(addr);
		val = op_ROL(val);
		mem_write_byte(addr, val);
		break;
	/*************** ROR **************/
	case 0x6A:					//register a Addressing
		val = cpu.reg_a;
		val = op_ROR(val);
		mem_write_byte(addr, val);
		break;
	case 0x66:					//zero page Addressing 
		addr = addr_zero();
		val = mem_read_byte(addr);
		val = op_ROR(val);
		mem_write_byte(addr, val);
		break;
	case 0x76:					//zero page x Indexed Addressing
		addr = addr_zero_reg(cpu.reg_x);
		val = mem_read_byte(addr);
		val = op_ROR(val);
		mem_write_byte(addr, val);
		break;
	case 0x6E:					//absolute Addressing 
		addr = addr_abs();
		val = mem_read_byte(addr);
		val = op_ROR(val);
		mem_write_byte(addr, val);
		break;
	case 0x7E:					//absolute x Indexed Addressing
		addr = addr_abs_reg(cpu.reg_x);
		val = mem_read_byte(addr);
		val = op_ROR(val);
		mem_write_byte(addr, val);
		break;

	/****************** ADC ********************/
	case 0x69:					// ADC imm
		op_ADC(op_read_byte());
		break;
	case 0x65:					//zero page 
		addr = addr_zero();
		val = mem_read_byte(addr);
		op_ADC(val);
		break;
	case 0x75:					//zero page x indexed
		op_ADC(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0x6D:					//absolute 
		op_ADC(mem_read_byte(
				addr_abs()
				));
		break;
	case 0x7D:					//absolute x indexed
		op_ADC(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0x79:					//absolute y indexed 
		op_ADC(mem_read_byte(
				addr_zero_reg(cpu.reg_y)
				));
		break;
	case 0x61:					//x
		op_ADC(mem_read_byte(
				addr_ind_x()
				));
		break;
	case 0x71:					//y
		op_ADC(mem_read_byte(
				addr_ind_y()
				));
		break;

	/****************** SBC ********************/
	case 0xE9:					// SBC imm
		op_SBC(op_read_byte());
		break;
	case 0xE5:					//zero page 
		addr = addr_zero();
		val = mem_read_byte(addr);
		op_SBC(val);
		break;
	case 0xF5:					//zero page x indexed
		op_SBC(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0xED:					//absolute 
		op_SBC(mem_read_byte(
				addr_abs()
				));
		break;
	case 0xFD:					//absolute x indexed
		op_SBC(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0xF9:					//absolute y indexed 
		op_SBC(mem_read_byte(
				addr_zero_reg(cpu.reg_y)
				));
		break;
	case 0xE1:					//x
		op_SBC(mem_read_byte(
				addr_ind_x()
				));
		break;
	case 0xF1:					//y:
		op_SBC(mem_read_byte(
				addr_ind_y()
				));
		break;

	/****************** CMP ********************/
	case 0xC9:					// CMP imm
		op_CMP(op_read_byte());
		break;
	case 0xC5:					//zero page 
		addr = addr_zero();
		val = mem_read_byte(addr);
		op_CMP(val);
		break;
	case 0xD5:					//zero page x indexed
		op_CMP(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0xCD:					//absolute 
		op_CMP(mem_read_byte(
				addr_abs()
				));
		break;
	case 0xDD:					//absolute x indexed
		op_CMP(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0xD9:					//absolute y indexed 
		op_CMP(mem_read_byte(
				addr_zero_reg(cpu.reg_y)
				));
		break;
	case 0xC1:					//x
		op_CMP(mem_read_byte(
				addr_ind_x()
				));
		break;
	case 0xD1:					//y:
		op_CMP(mem_read_byte(
				addr_ind_y()
				));
		break;

	/**************** CPX/CPY *****************/
	case 0xE0:					//imm
		op_CPX(op_read_byte());
		break;
	case 0xE4:					//zero page
		op_CPX(mem_read_byte(
				addr_zero()	
				));
		break;
	case 0xEC:					//absolute
		op_CPX(mem_read_byte(
				addr_abs()
				));
		break;
	case 0xC0:
		op_CPY(op_read_byte());
		break;
	case 0xC4:
		op_CPY(mem_read_byte(
				addr_zero()
				));
		break;
	case 0xCC:
		op_CPY(mem_read_byte(
				addr_abs()
				));
		break;

	/*************** INC/DEC **************/	
	case 0xE6:					//zero page
		addr = addr_zero();
		val = op_INC(mem_read_byte(addr));
		mem_write_byte(addr, val);
		break;
	case 0xF6:					//zero page x indexed
		addr = addr_zero_reg(cpu.reg_x);
		val = op_INC(mem_read_byte(addr));
		mem_write_byte(addr, val);
		break;
	case 0xEE:					//absolute
		addr = addr_abs();
		val = op_INC(mem_read_byte(addr));
		mem_write_byte(addr, val);
		break;
	case 0xFE:					//absolute x Indexed
		addr = addr_abs_reg(cpu.reg_x);
		val = op_INC(mem_read_byte(addr));
		mem_write_byte(addr, val);
		break;
	case 0xC6:					//zero page
		addr = addr_zero();
		val = op_DEC(mem_read_byte(addr));
		mem_write_byte(addr, val);
		break;
	case 0xD6:					//zero page x indexed
		addr = addr_zero_reg(cpu.reg_x);
		val = op_DEC(mem_read_byte(addr));
		mem_write_byte(addr, val);
		break;
	case 0xCE:					//absolute
		addr = addr_abs();
		val = op_DEC(mem_read_byte(addr));
		mem_write_byte(addr, val);
		break;
	case 0xDE:					//absolute x Indexed
		addr = addr_abs_reg(cpu.reg_x);
		val = op_DEC(mem_read_byte(addr));
		mem_write_byte(addr, val);
		break;

	case 0xE8:					//INX
		op_INX();
		break;
	case 0xCA:					//DEX
		op_DEX();
		break;
	case 0xC8:					//INY
		op_INY();
		break;
	case 0x88:					//DEY
		op_DEY();
		break;

	case 0x38:
		op_SEC();
		break;
	case 0xF8:
		op_SED();
		break;
	case 0x18:
		op_CLC();
		break;
	case 0xD8:
		op_CLD();
		break;
	case 0xB8:
		op_CLV();
		break;

	/****************** AND ********************/
	case 0x29:					// AND imm
		op_AND(op_read_byte());
		break;
	case 0x25:					//zero page 
		addr = addr_zero();
		val = mem_read_byte(addr);
		op_AND(val);
		break;
	case 0x35:					//zero page x indexed
		op_AND(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0x2D:					//absolute 
		op_AND(mem_read_byte(
				addr_abs()
				));
		break;
	case 0x3D:					//absolute x indexed
		op_AND(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0x39:					//absolute y indexed 
		op_AND(mem_read_byte(
				addr_zero_reg(cpu.reg_y)
				));
		break;
	case 0x21:					//x
		op_AND(mem_read_byte(
				addr_ind_x()
				));
		break;
	case 0x31:					//y
		op_AND(mem_read_byte(
				addr_ind_y()
				));
		break;

	/****************** ORA ********************/
	case 0x09:					// ORA imm
		op_ORA(op_read_byte());
		break;
	case 0x05:					//zero page 
		addr = addr_zero();
		val = mem_read_byte(addr);
		op_ORA(val);
		break;
	case 0x15:					//zero page x indexed
		op_ORA(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0x0D:					//absolute 
		op_ORA(mem_read_byte(
				addr_abs()
				));
		break;
	case 0x1D:					//absolute x indexed
		op_ORA(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0x19:					//absolute y indexed 
		op_ORA(mem_read_byte(
				addr_zero_reg(cpu.reg_y)
				));
		break;
	case 0x01:					//x
		op_ORA(mem_read_byte(
				addr_ind_x()
				));
		break;
	case 0x11:					//y
		op_ORA(mem_read_byte(
				addr_ind_y()
				));
		break;

	/****************** EOR ********************/
	case 0x49:					// EOR imm
		op_EOR(op_read_byte());
		break;
	case 0x45:					//zero page 
		addr = addr_zero();
		val = mem_read_byte(addr);
		op_EOR(val);
		break;
	case 0x55:					//zero page x indexed
		op_EOR(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0x4D:					//absolute 
		op_EOR(mem_read_byte(
				addr_abs()
				));
		break;
	case 0x5D:					//absolute x indexed
		op_EOR(mem_read_byte(
				addr_zero_reg(cpu.reg_x)
				));
		break;
	case 0x59:					//absolute y indexed 
		op_EOR(mem_read_byte(
				addr_zero_reg(cpu.reg_y)
				));
		break;
	case 0x41:					//x
		op_EOR(mem_read_byte(
				addr_ind_x()
				));
		break;
	case 0x51:					//y
		op_EOR(mem_read_byte(
				addr_ind_y()
				));
		break;

	/******************** BIT ********************/
	case 0x24:					//zero page 
		op_BIT(mem_read_byte(
				addr_zero()
				));
		break;
	case 0x2C:					//absolute 
		op_BIT(mem_read_byte(
				addr_abs()
				));
		break;

	/******************* JMP ********************/
	case 0x4C:
		op_JMP(addr_abs());
		break;
	case 0x6C:
		op_JMP(mem_read_word(addr_abs()));
		break;

	case 0x90:
		op_BCC();
		break;
	case 0xB0:
		op_BCS();
		break;
	case 0xF0:
		op_BEQ();
		break;
	case 0xD0:
		op_BNE();
		break;
	case 0x50:
		op_BVC();
		break;
	case 0x70:
		op_BVS();
		break;
	case 0x30:
		op_BMI();
		break;
	case 0x10:
		op_BPL();
		break;

	case 0x20:
		op_JSR();
		break;
	case 0x60:
		op_RTS();
		break;

	default:
		printf("error: unknow operation codes:%02X", opcode);
		break;
	}
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
	SET_FLAG_N(cpu.reg_x);
	SET_FLAG_Z(cpu.reg_x);
}
inline void op_TXA(){
	cpu.reg_a = cpu.reg_x;
	SET_FLAG_N(cpu.reg_a);
	SET_FLAG_Z(cpu.reg_a);
}
inline void op_TAY(){
	cpu.reg_y = cpu.reg_a;
	SET_FLAG_N(cpu.reg_y);
	SET_FLAG_Z(cpu.reg_y);
}
inline void op_TYA(){
	cpu.reg_a = cpu.reg_y;
	SET_FLAG_N(cpu.reg_a);
	SET_FLAG_Z(cpu.reg_a);
}
inline void op_TXS(){
	cpu.reg_s = cpu.reg_x;
}
inline void op_TSX(){
	cpu.reg_x = cpu.reg_s;
	SET_FLAG_N(cpu.reg_x);
	SET_FLAG_Z(cpu.reg_x);
}

inline void op_PHA(){
	push_byte(cpu.reg_a);
}
inline void op_PLA(){
	cpu.reg_a = pop_byte();
}
inline void op_PHP(){
	push_byte(cpu.reg_flag);
}
inline void op_PLP(){
	cpu.reg_flag = pop_byte();
}

u8 op_ASL(u8 byte){
	set_flag(FLAG_C, (byte>>7)&1 > 0);	
	byte = byte << 1;
	SET_FLAG_N(byte);
	SET_FLAG_Z(byte);
	return byte;
}
u8 op_LSR(u8 byte){
	set_flag(FLAG_C, byte&1 > 0);
	byte = byte >> 1;
	byte &= ~(1<<7);
	set_flag(FLAG_N, false);				//flag v must be false
	SET_FLAG_Z(byte);
}
u8 op_ROL(u8 byte){
	set_flag(FLAG_C, (byte>>7)&1 > 0);
	byte = byte << 1;
	byte |= (u8)get_flag(FLAG_C);
	SET_FLAG_N(byte);
	SET_FLAG_Z(byte);
	return byte;
}
u8 op_ROR(u8 byte){
	set_flag(FLAG_C, (byte>>7)&1 > 0);
	byte = byte >> 1;
	byte |= ((u8)get_flag(FLAG_C)) << 7;
	SET_FLAG_N(byte);
	SET_FLAG_Z(byte);
	return byte;
}

void op_ADC(u8 byte){
	u16 res16 = (u16)byte + (u16)cpu.reg_a;
	if(get_flag(FLAG_C))
		++res16;
	set_flag(FLAG_C, res16> 0xff);
	u8 res = (u8)res16;
	set_flag(FLAG_V, ((0x80 & !(byte^cpu.reg_a))
					&& ((res^byte) & 0x80)) );
	SET_FLAG_N(byte);
	SET_FLAG_Z(byte);
	cpu.reg_a=res;
}
void op_SBC(u8 byte){
	byte = ~byte;
	op_ADC(byte);
}
void op_CMP(u8 byte){
	u16 res16 = (u16)cpu.reg_a - (u16)byte;
	set_flag(FLAG_C, res16<0x100);
	byte=(u8)res16;
	SET_FLAG_N(byte);
	SET_FLAG_Z(byte);
}
void op_CPX(u8 byte){
	u16 res16 = (u16)cpu.reg_x - (u16)byte;
	set_flag(FLAG_C, res16<0x100);
	byte=(u8)res16;
	SET_FLAG_N(byte);
	SET_FLAG_Z(byte);
}
void op_CPY(u8 byte){
	u16 res16 = (u16)cpu.reg_x - (u16)byte;
	set_flag(FLAG_C, res16<0x100);
	byte=(u8)res16;
	SET_FLAG_N(byte);
	SET_FLAG_Z(byte);
}

u8 op_INC(u8 byte){
	++byte;
	SET_FLAG_N(byte);
	SET_FLAG_Z(byte);
	return byte;
}
void op_INX(){
	cpu.reg_x++;
	SET_FLAG_N(cpu.reg_x);
	SET_FLAG_Z(cpu.reg_x);
}
void op_INY(){
	cpu.reg_y++;
	SET_FLAG_N(cpu.reg_y);
	SET_FLAG_Z(cpu.reg_y);
}
u8 op_DEC(u8 byte){
	--byte;
	SET_FLAG_N(byte);
	SET_FLAG_Z(byte);
	return byte;
}
void op_DEX(){
	--cpu.reg_x;
	SET_FLAG_N(cpu.reg_x);
	SET_FLAG_Z(cpu.reg_x);
}
void op_DEY(){
	--cpu.reg_y;
	SET_FLAG_N(cpu.reg_y);
	SET_FLAG_Z(cpu.reg_y);
}
	
void op_SEC(){
	set_flag(FLAG_C,true);
}
void op_SED(){
	set_flag(FLAG_D,true);
}
void op_CLC(){
	set_flag(FLAG_C,false);
}
void op_CLD(){
	set_flag(FLAG_D,false);
}
void op_CLV(){
	set_flag(FLAG_V,false);
}

void op_AND(u8 byte){
	cpu.reg_a &= byte;
	SET_FLAG_N(cpu.reg_a);
	SET_FLAG_Z(cpu.reg_a);
}
void op_ORA(u8 byte){
	cpu.reg_a |= byte;
	SET_FLAG_N(cpu.reg_a);
	SET_FLAG_Z(cpu.reg_a);
}
void op_EOR(u8 byte){
	cpu.reg_a ^= byte;
	SET_FLAG_N(cpu.reg_a);
	SET_FLAG_Z(cpu.reg_a);
}
void op_BIT(u8 byte){
	SET_FLAG_Z(cpu.reg_a & byte);
	set_flag(FLAG_N, byte & 0x80);
	set_flag(FLAG_V, byte & 0x40);
}

void op_JMP(u16 addr){
	cpu.reg_pc = addr;
}
void op_BCC(){
	if(!get_flag(FLAG_C))
		op_JMP(addr_rel(op_read_byte()));
}
void op_BCS(){
	if(get_flag(FLAG_C))
		op_JMP(addr_rel(op_read_byte()));
}
void op_BEQ(){
	if(get_flag(FLAG_Z))
		op_JMP(addr_rel(op_read_byte()));
}
void op_BNE(){
	if(!get_flag(FLAG_Z))
		op_JMP(addr_rel(op_read_byte()));
}
void op_BVC(){
	if(!get_flag(FLAG_V))
		op_JMP(addr_rel(op_read_byte()));
}
void op_BVS(){
	if(get_flag(FLAG_V))
		op_JMP(addr_rel(op_read_byte()));
}
void op_BMI(){
	if(get_flag(FLAG_N))
		op_JMP(addr_rel(op_read_byte()));
}
void op_BPL(){
	if(!get_flag(FLAG_N))
		op_JMP(addr_rel(op_read_byte()));
}

void op_JSR(){
	u16 addr = op_read_word();
	push_byte(cpu.reg_pc);
	op_JMP(addr);
}
void op_RTS(){
	op_JMP(pop_word());
}
/****************************   Addressing functions    **********************************/

//Immediate Addressing
//u16 addr_imm(){
//	return op_read_byte();	
//}

//Direct Addressing
u16 addr_abs(){
	u16 addr = op_read_word();
	return addr;
}

//Zero page Addressing 
u16 addr_zero(){
	u8 addr = op_read_byte();
	return addr;
}

//Register Addressing
u16 addr_reg(u8 reg){
	return (u16)reg;
}

//Zero page Indexed Addressing 
u16 addr_zero_reg(u8 reg){
	u16 addr = (u16)op_read_byte();
	addr += reg;
	return addr;
}

//Direct Indexed Addressing 
u16 addr_abs_reg(u8 reg){
	u16 addr = op_read_word();
	addr += reg;
	return addr;
}

// TODO need test
u16 addr_rel(u8 addr){
	return cpu.reg_pc +(u16)(s8)addr;						//fill high bits with the bit of sign
}
//
u16 addr_ind_x(){
	u16 addr = (u16)op_read_byte();
	addr += cpu.reg_x;
	addr = mem_read_word(addr);
	return addr;
}

u16 addr_ind_y(){
	u16 addr = (u16)op_read_byte();
	addr = mem_read_word(addr);
	addr += cpu.reg_y;
	return addr;
}

//


