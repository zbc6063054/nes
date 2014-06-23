#include <stdio.h>
#include <string.h>
#include "../global.h"
#include "nes.h"
#include "cpu.h"
#include "memory.h"
#include "debug.h"

#define CHECK_PAGEWRAP(n, o) isWrap = ((n)&0xFF00) == ((o)&0xFF00)? false : true

Cpu::Cpu(Nes *parent)
    :nes(parent){

}

int Cpu::init(){
    reg_a = (u8)0;
    reg_x = (u8)0;
    reg_y = (u8)0;
	
    reg_s = (u8)0;							//points to the top of stack
    reg_pc = (u16)0;
    reg_flag = (u8)0;
    isRunning = false;
    nmi_count = 0;
    isIRQ = false;
    isReset = false;
    isSleep = false;

	isWrap = false;
	isWrapAdd = false;

	preCycles = 0;
	cycles = 0;
	return 0;
}

void Cpu::setNMI(){
//	LOG_ON(nmi_count, "nmi occured in nmi! nmi count :%d \n", nmi_count);
    nmi_count = 2;
}

void Cpu::reset(){
    init();
    reg_pc = readWord(ADDR_RESET);
}

void Cpu::start(){
    if(!isRunning){
        isRunning = true;
    }
}

static u8 CycTable[256] =
{
	/*0x00*/ 7,6,2,8,3,3,5,5,3,2,2,2,4,4,6,6,
	/*0x10*/ 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	/*0x20*/ 6,6,2,8,3,3,5,5,4,2,2,2,4,4,6,6,
	/*0x30*/ 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	/*0x40*/ 6,6,2,8,3,3,5,5,3,2,2,2,3,4,6,6,
	/*0x50*/ 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	/*0x60*/ 6,6,2,8,3,3,5,5,4,2,2,2,5,4,6,6,
	/*0x70*/ 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	/*0x80*/ 2,6,2,6,3,3,3,3,2,2,2,2,4,4,4,4,
	/*0x90*/ 2,6,2,6,4,4,4,4,2,5,2,5,5,5,5,5,
	/*0xA0*/ 2,6,2,6,3,3,3,3,2,2,2,2,4,4,4,4,
	/*0xB0*/ 2,5,2,5,4,4,4,4,2,4,2,4,4,4,4,4,
	/*0xC0*/ 2,6,2,8,3,3,5,5,2,2,2,2,4,4,6,6,
	/*0xD0*/ 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	/*0xE0*/ 2,6,3,8,3,3,5,5,2,2,2,2,4,4,6,6,
	/*0xF0*/ 2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,0,//7,
};

static int WrapTable[256] = 
{
	/*0x00*/ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	/*0x10*/ 1,1,0,0,0,0,0,0, 0,1,0,0,0,1,0,0,
	/*0x20*/ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	/*0x30*/ 1,1,0,0,0,0,0,0, 0,1,0,0,0,1,0,0,
	/*0x40*/ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	/*0x50*/ 1,1,0,0,0,0,0,0, 0,1,0,0,0,1,0,0,
	/*0x60*/ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	/*0x70*/ 1,1,0,0,0,0,0,0, 0,1,0,0,0,1,0,0,
	/*0x80*/ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	/*0x90*/ 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	/*0xA0*/ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	/*0xB0*/ 1,1,0,0,0,0,0,0, 0,1,0,0,1,1,1,0,
	/*0xC0*/ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	/*0xD0*/ 1,1,0,0,0,0,0,0, 0,1,0,0,0,1,0,0,
	/*0xE0*/ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	/*0xF0*/ 1,1,0,0,0,0,0,0, 0,1,0,0,0,1,0,0,
};

bool boutput = false;

int Cpu::run(int cycle_req){
	//get next code
	u8 opcode = 0;
    cycles = 0;
    while(isRunning &&(cycles<cycle_req)){

		if(preCycles >= cycle_req){
			preCycles -= cycle_req;
			cycles = cycle_req;
			break;
		}else if(preCycles > 0){
			cycles += preCycles;
			preCycles = 0;
		}

        DEBUG_CPU_OPSTART

		isWrapAdd = isWrap = false;
		opcode = op_read_byte();

		if(boutput) printf("%04X%02X ", reg_pc-1, opcode);

        exec(opcode);
		cycles += isWrap&&WrapTable[opcode] ? CycTable[opcode]+1 : CycTable[opcode];
		//if(isWrap && WrapTable[opcode]) cycles += 1;
        /*if(isSleep){
           isSleep = false;
           return 0;
        }*/
        if(nmi_count){
			if(nmi_count == 1){
				push_word(reg_pc);
				push_byte(reg_flag);
				setFlag(FLAG_I, true);
				setFlag(FLAG_B, false);
				reg_pc = readWord(ADDR_NMI);
				cycles += 7;
			}
			--nmi_count;
        }else if(isIRQ){
            push_word(reg_pc);
            push_byte(reg_flag);
            setFlag(FLAG_I, true);
            setFlag(FLAG_B, false);
            reg_pc = readWord(ADDR_IRQ);
            cycles += 7;
            isIRQ = false;
		}

        DEBUG_CPU_OPEND
	}
    return cycles - cycle_req;
}

void Cpu::dump(){
	LOGI("cpu:\n    cycles: %d  totalCycles: %d  \n", cycles, nes->getTotalCycles()+cycles);
	LOGI("    A=%02X X=%02X Y=%02X  S=%02X PC=%04X FLAG=%04X\n", 
        reg_a,reg_x,reg_y,reg_s,reg_pc,reg_flag);
	LOGI("    C:%d Z:%d I:%d D:%d B:%d V:%d N:%d\n", 
            getFlag(FLAG_C), getFlag(FLAG_Z), getFlag(FLAG_I),
            getFlag(FLAG_D), getFlag(FLAG_B), getFlag(FLAG_V), getFlag(FLAG_N));
	LOGI("    Stack:");
	for(u8 i = reg_s+1; i!=0; ++i){
		LOGI("%02X ", readByte(STACK_ADDR(i)));
	}
	LOGI("\n");
}

// 0 1 2 3 4 5 6 7
// C Z I D B   V N
void Cpu::setFlag(u8 type, bool val){
    if(val){
        reg_flag |= (u8)(((u8)1)<<type);
    }else{
        reg_flag &= ~(u8)(((u8)1)<<type);
    }
}

bool Cpu::getFlag(u8 type){
    return ((reg_flag >> type) & 1) > 0;
}

inline u8 Cpu::op_read_byte(){
    return readByte(reg_pc++);
}
u16 Cpu::op_read_word(){
    u16 word = readWord(reg_pc);
    reg_pc += 2;
    return word;
}
//	stack
void Cpu::push_byte(u8 byte){
    writeByte(STACK_ADDR(reg_s), byte);
    --reg_s;
}
void Cpu::push_word(u16 word){
	push_byte(word>>8);
	push_byte(word&0xFF);
}
u16 Cpu::pop_word(){
    u16 word = pop_byte();
	word |= ((u16)pop_byte())<<8;
    return word;
}
u8 Cpu::pop_byte(){
    ++reg_s;
    return readByte(STACK_ADDR(reg_s));
}

/***************************         operations          *********************************/

void Cpu::op_LDA(u8 byte){
    reg_a = byte;
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
}
void Cpu::op_LDX(u8 byte){
    reg_x = byte;
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
}
void Cpu::op_LDY(u8 byte){
    reg_y = byte;
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
}

inline void Cpu::op_STA(u16 addr){
    writeByte(addr, reg_a);
}
inline void Cpu::op_STX(u16 addr){
    writeByte(addr, reg_x);
}
inline void Cpu::op_STY(u16 addr){
    writeByte(addr, reg_y);
}

inline void Cpu::op_TAX(){
    reg_x = reg_a;
    SET_FLAG_N(reg_x);
    SET_FLAG_Z(reg_x);
}
inline void Cpu::op_TXA(){
    reg_a = reg_x;
    SET_FLAG_N(reg_a);
    SET_FLAG_Z(reg_a);
}
inline void Cpu::op_TAY(){
    reg_y = reg_a;
    SET_FLAG_N(reg_y);
    SET_FLAG_Z(reg_y);
}
inline void Cpu::op_TYA(){
    reg_a = reg_y;
    SET_FLAG_N(reg_a);
    SET_FLAG_Z(reg_a);
}
inline void Cpu::op_TXS(){
    reg_s = reg_x;
}
inline void Cpu::op_TSX(){
    reg_x = reg_s;
    SET_FLAG_N(reg_x);
    SET_FLAG_Z(reg_x);
}

inline void Cpu::op_PHA(){
    push_byte(reg_a);
}
inline void Cpu::op_PLA(){
    reg_a = pop_byte();
    SET_FLAG_N(reg_a);
    SET_FLAG_Z(reg_a);
}
inline void Cpu::op_PHP(){
    push_byte(reg_flag | (1<<FLAG_B) | (1<<FLAG_R));
}
inline void Cpu::op_PLP(){
    reg_flag = pop_byte();
    setFlag(FLAG_R, true);
}

u8 Cpu::op_ASL(u8 byte){
    setFlag(FLAG_C, byte & 0x80);
	byte = byte << 1;
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
	return byte;
}
u8 Cpu::op_LSR(u8 byte){
    setFlag(FLAG_C, byte & 1);
	byte = byte >> 1;
//	byte &= ~(1<<7);
//    setFlag(FLAG_N, false);				//flag v must be false
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
    return byte;
}
u8 Cpu::op_ROL(u8 byte){
    if(getFlag(FLAG_C)){
        setFlag(FLAG_C, byte & 0x80);
        byte = byte << 1;
        byte |= 0x01;
    }else{
        setFlag(FLAG_C, byte & 0x80);
        byte = byte << 1;
    }
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
	return byte;
}
u8 Cpu::op_ROR(u8 byte){
    if(getFlag(FLAG_C)){
        setFlag(FLAG_C, byte & 0x01);
        byte = byte >> 1;
        byte |= 0x80;
    }else{
        setFlag(FLAG_C, byte & 0x01);
        byte = byte >> 1;
    }
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
	return byte;
}

void Cpu::op_ADC(u8 byte){
    u16 res16 = (u16)byte + (u16)reg_a;
    if(getFlag(FLAG_C))
		++res16;
    setFlag(FLAG_C, res16 > 0xff);
	u8 res = (u8)res16;
    setFlag(FLAG_V, ((~(byte^reg_a))& (res^reg_a) & 0x80) );
    SET_FLAG_N(res);
    SET_FLAG_Z(res);
    reg_a = res;
}
void Cpu::op_SBC(u8 byte){
	byte = ~byte;
	op_ADC(byte);
}
void Cpu::op_CMP(u8 byte){
    u16 res16 = (u16)reg_a - (u16)byte;
    setFlag(FLAG_C, (res16&0x8000)==0);
	byte=(u8)res16;
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
}
void Cpu::op_CPX(u8 byte){
    u16 res16 = ((u16)reg_x) - ((u16)byte);
    setFlag(FLAG_C, (res16&0x8000)==0);
	byte=(u8)res16;
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
}
void Cpu::op_CPY(u8 byte){
    u16 res16 = (u16)reg_y - (u16)byte;
    setFlag(FLAG_C, (res16&0x8000)==0);
	byte=(u8)res16;
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
}

u8 Cpu::op_INC(u8 byte){
	++byte;
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
	return byte;
}
void Cpu::op_INX(){
    reg_x++;
    SET_FLAG_N(reg_x);
    SET_FLAG_Z(reg_x);
}
void Cpu::op_INY(){
    reg_y++;
    SET_FLAG_N(reg_y);
    SET_FLAG_Z(reg_y);
}
u8 Cpu::op_DEC(u8 byte){
	--byte;
    SET_FLAG_N(byte);
    SET_FLAG_Z(byte);
	return byte;
}
void Cpu::op_DEX(){
    --reg_x;
    SET_FLAG_N(reg_x);
    SET_FLAG_Z(reg_x);
}
void Cpu::op_DEY(){
    --reg_y;
    SET_FLAG_N(reg_y);
    SET_FLAG_Z(reg_y);
}
	
void Cpu::op_SEC(){
    setFlag(FLAG_C,true);
}
void Cpu::op_SED(){
    setFlag(FLAG_D,true);
}
void Cpu::op_SEI(){
    setFlag(FLAG_I,true);
}
void Cpu::op_CLC(){
    setFlag(FLAG_C,false);
}
void Cpu::op_CLD(){
    setFlag(FLAG_D,false);
}
void Cpu::op_CLV(){
    setFlag(FLAG_V,false);
}
void Cpu::op_CLI(){
    setFlag(FLAG_I,false);
}
void Cpu::op_AND(u8 byte){
    reg_a &= byte;
    SET_FLAG_N(reg_a);
    SET_FLAG_Z(reg_a);
}
void Cpu::op_ORA(u8 byte){
    reg_a |= byte;
    SET_FLAG_N(reg_a);
    SET_FLAG_Z(reg_a);
}
void Cpu::op_EOR(u8 byte){
    reg_a ^= byte;
    SET_FLAG_N(reg_a);
    SET_FLAG_Z(reg_a);
}
void Cpu::op_BIT(u8 byte){
    SET_FLAG_Z(reg_a & byte);
    setFlag(FLAG_N, byte & 0x80);
    setFlag(FLAG_V, byte & 0x40);
}

void Cpu::op_JMP(u16 addr){
    reg_pc = addr;
}

#define TEST_FLAG(f) {	if(f) {	 op_JMP(addr_rel(op_read_byte()));  cycles++;}\
					      else{	 reg_pc++; } }

void Cpu::op_BCC(){
    TEST_FLAG(!getFlag(FLAG_C));
}
void Cpu::op_BCS(){
    TEST_FLAG(getFlag(FLAG_C));
}
void Cpu::op_BEQ(){
    TEST_FLAG(getFlag(FLAG_Z));
}
void Cpu::op_BNE(){
    TEST_FLAG(!getFlag(FLAG_Z));
}
void Cpu::op_BVC(){
    TEST_FLAG(!getFlag(FLAG_V));
}
void Cpu::op_BVS(){
    TEST_FLAG(getFlag(FLAG_V));
}
void Cpu::op_BMI(){
    TEST_FLAG(getFlag(FLAG_N));
}
void Cpu::op_BPL(){
    TEST_FLAG(!getFlag(FLAG_N));
}

void Cpu::op_JSR(){
	u16 addr = op_read_word();
    push_word(--reg_pc);
	op_JMP(addr);
}
void Cpu::op_RTS(){
    u16 addr = pop_word();
    op_JMP(++addr);
}
void Cpu::op_RTI(){
    reg_flag = pop_byte();
    reg_pc = pop_word();
}
void Cpu::op_BRK(){
    reg_pc++;
    push_word(reg_pc);
    setFlag(FLAG_B, 1);
    push_byte(reg_flag);
    setFlag(FLAG_I, 1);
    reg_pc = readWord(ADDR_IRQ);
}

/****************************   Addressing functions    **********************************/

//Immediate Addressing
//u16 addr_imm(){
//	return op_read_byte();	
//}

//Direct Addressing
u16 Cpu::addr_abs(){
	u16 addr = op_read_word();
	return addr;
}

//Zero page Addressing 
u16 Cpu::addr_zero(){
	u8 addr = op_read_byte();
	return addr;
}

//Register Addressing
u16 Cpu::addr_reg(u8 reg){
	return (u16)reg;
}

//Zero page Indexed Addressing 
u16 Cpu::addr_zero_reg(u8 reg){
    u8 addr = op_read_byte();
	addr += reg;
    return (u8)addr;
}

//Direct Indexed Addressing 
u16 Cpu::addr_abs_reg(u8 reg){
	u16 addr = op_read_word();
	u16 addr_new = addr + reg;
	CHECK_PAGEWRAP(addr_new, addr);
	return addr_new;
}

// TODO need test
u16 Cpu::addr_rel(u8 addr){
	u16 addr_new = reg_pc +(u16)(s8)addr;		//fill high bits with the bit of sign
	CHECK_PAGEWRAP(addr_new, reg_pc);
    return addr_new;						
}
//
u16 Cpu::addr_ind_x(){
    u8 addr = op_read_byte();
    addr += reg_x;
    return ((u16)readByte(addr))+(((u16)readByte((u8)(addr+1)))<<8);
}

u16 Cpu::addr_ind_y(){
    u8 addr = op_read_byte();
    u16 res = ((u16)readByte(addr))+(((u16)readByte((u8)(addr+1)))<<8);
	u16 addr_new = res + reg_y;
	CHECK_PAGEWRAP(addr_new ,res);
    return addr_new;
}

int Cpu::exec(u8 opcode){
	u16 addr = 0;
	u8 val = 0;
	u8 cycle = 0;
	switch(opcode){
	case 0xA9:				//LDA imm	
		op_LDA(op_read_byte());	
		cycle = 2;
		break;
	case 0xA5:				//zero page Addressing
		op_LDA(readByte(addr_zero()));
		cycle = 3;
		break;
	case 0xB5:				//zero page x indexed addressing
		op_LDA(readByte(
			addr_zero_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0xAD:				//absolute addressing
		op_LDA(readByte(
			addr_abs()
			));
		cycle = 4;

		break;
	case 0xBD:				//absolute x indexed addressing
		op_LDA(readByte(
			addr_abs_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0xB9:				//absolute y indexed addressing
		op_LDA(readByte(
			addr_abs_reg(reg_y)
			));
		cycle = 4;
		break;
	case 0xA1:				//x
		op_LDA(readByte(
			addr_ind_x()
			));
		cycle = 6;
		break;
	case 0xB1:				//y
		op_LDA(readByte(
			addr_ind_y()
			));
		cycle = 5;
		break;
		/*************LDX************/
	case 0xA2:				//LDX imm
		op_LDX(op_read_byte());
		cycle = 2;
		break;
	case 0xA6:				//zero page Addressing
		op_LDX(readByte(
			addr_zero()
			));
		cycle = 3;
		break;
	case 0xB6:				//zero page y indexed Addressing
		op_LDX(readByte(
			addr_zero_reg(reg_y)
			));
		cycle = 4;
		break;
	case 0xAE:				//absolute Addressing
		op_LDX(readByte(
			addr_abs()
			));
		cycle = 4;
		break;
	case 0xBE:				//absolute y indexed Addressing 
		op_LDX(readByte(
			addr_abs_reg(reg_y)
			));
		cycle = 4;
		break;
		/****************LDY************************/		
	case 0xA0:				//LDY imm
		op_LDY(op_read_byte());
		cycle = 2;
		break;
	case 0xA4:				//zero page Addressing
		op_LDY(readByte(
			addr_zero()
			));
		cycle = 3;
		break;
	case 0xB4:				//zero page x indexed Addressing
		op_LDY(readByte(
			addr_zero_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0xAC:				//absolute Addressing
		op_LDY(readByte(
			addr_abs()
			));
		cycle = 4;
		break;
	case 0xBC:				//absolute x indexed Addressing 
		op_LDY(readByte(
			addr_abs_reg(reg_x)
			));
		cycle = 5;
		break;
		/**************** STA ***************/
	case 0x85:				//zero page Addressing
		op_STA(addr_zero());
		cycle = 3;
		break;
	case 0x95:				//zero page x indexed addressing
		op_STA(addr_zero_reg(reg_x));
		cycle = 4;
		break;
	case 0x8D:				//absolute addressing
		op_STA(addr_abs());
		cycle = 4;
		break;
	case 0x9D:				//absolute x indexed addressing
		op_STA(addr_abs_reg(reg_x));
		cycle = 5;
		break;
	case 0x99:				//absolute y indexed addressing
		op_STA(addr_abs_reg(reg_y));
		cycle = 5;
		break;
	case 0x81:				//x
		op_STA(addr_ind_x());
		cycle = 6;
		break;
	case 0x91:				//y
		op_STA(addr_ind_y());
		cycle = 6;
		break;
		/**************** STX ***************/
	case 0x86:				//zero page Addressing
		op_STX(addr_zero());
		cycle = 3;
		break;
	case 0x96:				//zero page y indexed addressing
		op_STX(addr_zero_reg(reg_y));
		cycle = 4;
		break;
	case 0x8E:				//absolute addressing
		op_STX(addr_abs());
		cycle = 4;
		break;
		/**************** STY ***************/
	case 0x84:				//zero page Addressing
		op_STY(addr_zero());
		cycle = 3;
		break;
	case 0x94:				//zero page x indexed addressing
		op_STY(addr_zero_reg(reg_x));
		cycle = 4;
		break;
	case 0x8C:				//absolute addressing
		op_STY(addr_abs());
		cycle = 4;
		break;

	case 0x8A:
		op_TXA();
		cycle = 2;
		break;
	case 0xAA:
		op_TAX();
		cycle = 2;
		break;
	case 0x98:
		op_TYA();
		cycle = 2;
		break;
	case 0xA8:
		op_TAY();
		cycle = 2;
		break;
	case 0x9A:
		op_TXS();
		cycle = 2;
		break;
	case 0xBA:
		op_TSX();
		cycle = 2;
		break;

	case 0x48:
		op_PHA();
		cycle = 3;
		break;
	case 0x68:
		op_PLA();
		cycle = 4;
		break;
	case 0x08:
		op_PHP();
		cycle = 3;
		break;
	case 0x28:
		op_PLP();
		cycle = 4;
		break;

		/*************** ASL **************/
	case 0x0A:					//register a Addressing
		val = reg_a;
		val = op_ASL(val);
		reg_a = val;
		cycle = 2;
		break;
	case 0x06:					//zero page Addressing 
		addr = addr_zero();
		val = readByte(addr);
		val = op_ASL(val);
		writeByte(addr, val);
		cycle = 5;
		break;
	case 0x16:					//zero page x Indexed Addressing
		addr = addr_zero_reg(reg_x);
		val = readByte(addr);
		val = op_ASL(val);
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0x0E:					//absolute Addressing 
		addr = addr_abs();
		val = readByte(addr);
		val = op_ASL(val);
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0x1E:					//absolute x Indexed Addressing
		addr = addr_abs_reg(reg_x);
		val = readByte(addr);
		val = op_ASL(val);
		writeByte(addr, val);
		cycle = 7;
		break;

		/*************** LSR **************/
	case 0x4A:					//register a Addressing
		val = reg_a;
		val = op_LSR(val);
		reg_a = val;
		cycle = 2;
		break;
	case 0x46:					//zero page Addressing 
		addr = addr_zero();
		val = readByte(addr);
		val = op_LSR(val);
		writeByte(addr, val);
		cycle= 5;
		break;
	case 0x56:					//zero page x Indexed Addressing
		addr = addr_zero_reg(reg_x);
		val = readByte(addr);
		val = op_LSR(val);
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0x4E:					//absolute Addressing 
		addr = addr_abs();
		val = readByte(addr);
		val = op_LSR(val);
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0x5E:					//absolute x Indexed Addressing
		addr = addr_abs_reg(reg_x);
		val = readByte(addr);
		val = op_LSR(val);
		writeByte(addr, val);
		cycle = 7;
		break;
		/*************** ROL **************/
	case 0x2A:					//register a Addressing
		val = reg_a;
		val = op_ROL(val);
		reg_a = val;
		cycle = 2;
		break;
	case 0x26:					//zero page Addressing 
		addr = addr_zero();
		val = readByte(addr);
		val = op_ROL(val);
		writeByte(addr, val);
		cycle = 5;
		break;
	case 0x36:					//zero page x Indexed Addressing
		addr = addr_zero_reg(reg_x);
		val = readByte(addr);
		val = op_ROL(val);
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0x2E:					//absolute Addressing 
		addr = addr_abs();
		val = readByte(addr);
		val = op_ROL(val);
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0x3E:					//absolute x Indexed Addressing
		addr = addr_abs_reg(reg_x);
		val = readByte(addr);
		val = op_ROL(val);
		writeByte(addr, val);
		cycle = 7;
		break;
		/*************** ROR **************/
	case 0x6A:					//register a Addressing
		val = reg_a;
		val = op_ROR(val);
		reg_a = val;
		cycle = 2;
		break;
	case 0x66:					//zero page Addressing 
		addr = addr_zero();
		val = readByte(addr);
		val = op_ROR(val);
		writeByte(addr, val);
		cycle = 5;
		break;
	case 0x76:					//zero page x Indexed Addressing
		addr = addr_zero_reg(reg_x);
		val = readByte(addr);
		val = op_ROR(val);
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0x6E:					//absolute Addressing 
		addr = addr_abs();
		val = readByte(addr);
		val = op_ROR(val);
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0x7E:					//absolute x Indexed Addressing
		addr = addr_abs_reg(reg_x);
		val = readByte(addr);
		val = op_ROR(val);
		writeByte(addr, val);
		cycle = 7;
		break;

		/****************** ADC ********************/
	case 0x69:					// ADC imm
		op_ADC(op_read_byte());
		cycle = 2;
		break;
	case 0x65:					//zero page 
		addr = addr_zero();
		val = readByte(addr);
		op_ADC(val);
		cycle = 3;
		break;
	case 0x75:					//zero page x indexed
		op_ADC(readByte(
			addr_zero_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0x6D:					//absolute 
		op_ADC(readByte(
			addr_abs()
			));
		cycle = 4;
		break;
	case 0x7D:					//absolute x indexed
		op_ADC(readByte(
			addr_abs_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0x79:					//absolute y indexed 
		op_ADC(readByte(
			addr_abs_reg(reg_y)
			));
		cycle = 5;
		break;
	case 0x61:					//x
		op_ADC(readByte(
			addr_ind_x()
			));
		cycle = 6;
		break;
	case 0x71:					//y
		op_ADC(readByte(
			addr_ind_y()
			));
		cycle = 4;
		break;

		/****************** SBC ********************/
	case 0xE9:					// SBC imm
		op_SBC(op_read_byte());
		cycle = 2;
		break;
	case 0xE5:					//zero page 
		addr = addr_zero();
		val = readByte(addr);
		op_SBC(val);
		cycle = 3;
		break;
	case 0xF5:					//zero page x indexed
		op_SBC(readByte(
			addr_zero_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0xED:					//absolute 
		op_SBC(readByte(
			addr_abs()
			));
		cycle = 4;
		break;
	case 0xFD:					//absolute x indexed
		op_SBC(readByte(
			addr_abs_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0xF9:					//absolute y indexed 
		op_SBC(readByte(
			addr_abs_reg(reg_y)
			));
		cycle = 4;
		break;
	case 0xE1:					//x
		op_SBC(readByte(
			addr_ind_x()
			));
		cycle = 6;
		break;
	case 0xF1:					//y:
		op_SBC(readByte(
			addr_ind_y()
			));
		cycle = 5;
		break;

		/****************** CMP ********************/
	case 0xC9:					// CMP imm
		op_CMP(op_read_byte());
		cycle = 2;
		break;
	case 0xC5:					//zero page 
		addr = addr_zero();
		val = readByte(addr);
		op_CMP(val);
		cycle = 3;
		break;
	case 0xD5:					//zero page x indexed
		op_CMP(readByte(
			addr_zero_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0xCD:					//absolute 
		op_CMP(readByte(
			addr_abs()
			));
		cycle = 4;
		break;
	case 0xDD:					//absolute x indexed
		op_CMP(readByte(
			addr_abs_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0xD9:					//absolute y indexed 
		op_CMP(readByte(
			addr_abs_reg(reg_y)
			));
		cycle = 4;
		break;
	case 0xC1:					//x
		op_CMP(readByte(
			addr_ind_x()
			));
		cycle = 6;
		break;
	case 0xD1:					//y:
		op_CMP(readByte(
			addr_ind_y()
			));
		cycle = 5;
		break;

		/**************** CPX/CPY *****************/
	case 0xE0:					//imm
		op_CPX(op_read_byte());
		cycle = 2;
		break;
	case 0xE4:					//zero page
		op_CPX(readByte(
			addr_zero()	
			));
		cycle = 3;
		break;
	case 0xEC:					//absolute
		op_CPX(readByte(
			addr_abs()
			));
		cycle = 4;
		break;
	case 0xC0:
		op_CPY(op_read_byte());
		cycle = 2;
		break;
	case 0xC4:
		op_CPY(readByte(
			addr_zero()
			));
		cycle = 3;
		break;
	case 0xCC:
		op_CPY(readByte(
			addr_abs()
			));
		cycle = 4;
		break;

		/*************** INC/DEC **************/	
	case 0xE6:					//zero page
		addr = addr_zero();
		val = op_INC(readByte(addr));
		writeByte(addr, val);
		cycle = 5;
		break;
	case 0xF6:					//zero page x indexed
		addr = addr_zero_reg(reg_x);
		val = op_INC(readByte(addr));
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0xEE:					//absolute
		addr = addr_abs();
		val = op_INC(readByte(addr));
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0xFE:					//absolute x Indexed
		addr = addr_abs_reg(reg_x);
		val = op_INC(readByte(addr));
		writeByte(addr, val);
		cycle = 7;
		break;
	case 0xC6:					//zero page
		addr = addr_zero();
		val = op_DEC(readByte(addr));
		writeByte(addr, val);
		cycle = 5;
		break;
	case 0xD6:					//zero page x indexed
		addr = addr_zero_reg(reg_x);
		val = op_DEC(readByte(addr));
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0xCE:					//absolute
		addr = addr_abs();
		val = op_DEC(readByte(addr));
		writeByte(addr, val);
		cycle = 6;
		break;
	case 0xDE:					//absolute x Indexed
		addr = addr_abs_reg(reg_x);
		val = op_DEC(readByte(addr));
		writeByte(addr, val);
		cycle = 7;
		break;

	case 0xE8:					//INX
		op_INX();
		cycle = 2;
		break;
	case 0xCA:					//DEX
		op_DEX();
		cycle = 2;
		break;
	case 0xC8:					//INY
		op_INY();
		cycle = 2;
		break;
	case 0x88:					//DEY
		op_DEY();
		cycle = 2;
		break;

	case 0x38:
		op_SEC();
		cycle = 2;
		break;
	case 0xF8:
		op_SED();
		cycle = 2;
		break;
	case 0x78:
		op_SEI();
		cycle = 2;
		break;
	case 0x18:
		op_CLC();
		cycle = 2;
		break;
	case 0xD8:
		op_CLD();
		cycle = 2;
		break;
	case 0xB8:
		op_CLV();
		cycle = 2;
		break;
	case 0x58:
		op_CLI();
		cycle = 2;
		break;

		/****************** AND ********************/
	case 0x29:					// AND imm
		op_AND(op_read_byte());
		cycle = 2;
		break;
	case 0x25:					//zero page 
		addr = addr_zero();
		val = readByte(addr);
		op_AND(val);
		cycle = 3;
		break;
	case 0x35:					//zero page x indexed
		op_AND(readByte(
			addr_zero_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0x2D:					//absolute 
		op_AND(readByte(
			addr_abs()
			));
		cycle = 4;
		break;
	case 0x3D:					//absolute x indexed
		op_AND(readByte(
			addr_abs_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0x39:					//absolute y indexed 
		op_AND(readByte(
			addr_abs_reg(reg_y)
			));
		cycle = 4;
		break;
	case 0x21:					//x
		op_AND(readByte(
			addr_ind_x()
			));
		cycle = 6;
		break;
	case 0x31:					//y
		op_AND(readByte(
			addr_ind_y()
			));
		cycle = 5;
		break;

		/****************** ORA ********************/
	case 0x09:					// ORA imm
		op_ORA(op_read_byte());
		cycle = 2;
		break;
	case 0x05:					//zero page 
		addr = addr_zero();
		val = readByte(addr);
		op_ORA(val);
		cycle = 3;
		break;
	case 0x15:					//zero page x indexed
		op_ORA(readByte(
			addr_zero_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0x0D:					//absolute 
		op_ORA(readByte(
			addr_abs()
			));
		cycle = 4;
		break;
	case 0x1D:					//absolute x indexed
		op_ORA(readByte(
			addr_abs_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0x19:					//absolute y indexed 
		op_ORA(readByte(
			addr_abs_reg(reg_y)
			));
		cycle = 4;
		break;
	case 0x01:					//x
		op_ORA(readByte(
			addr_ind_x()
			));
		cycle = 6;
		break;
	case 0x11:					//y
		op_ORA(readByte(
			addr_ind_y()
			));
		cycle = 5;
		break;

		/****************** EOR ********************/
	case 0x49:					// EOR imm
		op_EOR(op_read_byte());
		cycle = 2;
		break;
	case 0x45:					//zero page 
		addr = addr_zero();
		val = readByte(addr);
		op_EOR(val);
		cycle = 3;
		break;
	case 0x55:					//zero page x indexed
		op_EOR(readByte(
			addr_zero_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0x4D:					//absolute 
		op_EOR(readByte(
			addr_abs()
			));
		cycle = 4;
		break;
	case 0x5D:					//absolute x indexed
		op_EOR(readByte(
			addr_abs_reg(reg_x)
			));
		cycle = 4;
		break;
	case 0x59:					//absolute y indexed 
		op_EOR(readByte(
			addr_abs_reg(reg_y)
			));
		cycle = 4;
		break;
	case 0x41:					//x
		op_EOR(readByte(
			addr_ind_x()
			));
		cycle = 6;
		break;
	case 0x51:					//y
		op_EOR(readByte(
			addr_ind_y()
			));
		cycle = 5;
		break;

		/******************** BIT ********************/
	case 0x24:					//zero page 
		op_BIT(readByte(
			addr_zero()
			));
		cycle = 3;
		break;
	case 0x2C:					//absolute 
		op_BIT(readByte(
			addr_abs()
			));
		cycle = 4;
		break;

		/******************* JMP ********************/
	case 0x4C:
		addr = addr_abs();
		if(addr == reg_pc-2){           //ugly impelement, just for reduce the cpu useage
			isSleep = true;
		}else{
			op_JMP(addr);
		}
		cycle = 3;
		break;
	case 0x6C:
		addr = op_read_word();
		val = readByte(addr);
		addr = (addr & 0xFF00) | ((addr+1)&0x00FF);
		op_JMP((((u16)readByte(addr))<<8) | val);
		cycle = 5;
		break;
	case 0x90:
		op_BCC();
		cycle = 2;
		break;
	case 0xB0:
		op_BCS();
		cycle = 2;
		break;
	case 0xF0:
		op_BEQ();
		cycle = 2;
		break;
	case 0xD0:
		op_BNE();
		cycle = 2;
		break;
	case 0x50:
		op_BVC();
		cycle = 2;
		break;
	case 0x70:
		op_BVS();
		cycle = 2;
		break;
	case 0x30:
		op_BMI();
		cycle = 2;
		break;
	case 0x10:
		op_BPL();
		cycle = 2;
		break;

	case 0x20:
		op_JSR();
		cycle = 6;
		break;
	case 0x40:
		op_RTI();
		cycle = 6;
		break;
	case 0x60:
		op_RTS();
		cycle = 6;
		break;
	case 0xEA:          //NOP
		cycle = 2;
		break;
	case 0x44:
	case 0x04:
		cycle = 3;
		break;
	case 0x00:          //BRK
		op_BRK();
		cycle = 7;
		break;
#ifdef _DEBUG
	case 0xFF:
		//		ASSERT(false);
		//LOGI("cycles :%d\n", nes->getTotalCycles()+cycles);
		nes->dump();
		break;
#endif

	default:
		printf("error: unknow operation codes:%02X \n", opcode);
		nes->dump();
		isRunning = false;
		return -1;
		break;
	}
	return cycle;
}
//


