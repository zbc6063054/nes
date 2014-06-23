#pragma once
#include "../global.h"
#include "nes.h"

#define CYCLES_DMA 513

#define FLAG_C 0
#define FLAG_Z 1
#define FLAG_I 2
#define FLAG_D 3
#define FLAG_B 4
#define FLAG_R 5
#define	FLAG_V 6
#define FLAG_N 7

#define SET_FLAG_N(x) (setFlag(FLAG_N, ((x) & 0x80)))
#define SET_FLAG_Z(x) (setFlag(FLAG_Z, (x) == 0))

#define ADDR_NMI 	0xFFFA
#define ADDR_RESET	0xFFFC
#define ADDR_IRQ 	0xFFFE

#define STACK_ADDR(x) (0x100|(u16)(x))

enum register_type{
    reg_type_a,
    reg_type_x,
    reg_type_y
};

class Cpu{
public:
    friend class Nes;
    Cpu(Nes *parent);

public:
    u8 reg_a;						//register a
    u8 reg_x;						//register b
    u8 reg_y;						//register c
    u8 reg_s;
    u8 reg_flag;					//flag
    int nmi_count;
    bool isIRQ;
    bool isRunning;
    u16 reg_pc;						//register pc
    bool isReset;
    bool isSleep;
	int preCycles;
	int cycles;
	bool isWrap;
	bool isWrapAdd;

    Nes *nes;

public:
    void    push_byte(u8);
    u8      pop_byte();
    void    push_word(u16);
    u16     pop_word();
    void    setFlag(u8 type, bool val);
    bool    getFlag(u8 type);
    inline u8 op_read_byte();
    u16     op_read_word();

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
    void op_RTI();
    void op_BRK();

    u16 addr_imm();
    u16 addr_abs();
    u16 addr_zero();
    u16 addr_reg(u8 reg);
    u16 addr_zero_reg(u8 reg);
    u16 addr_abs_reg(u8 reg);
    u16 addr_rel(u8);
    u16 addr_ind_x();
    u16 addr_ind_y(); 

private:
    inline u8 readByte(u16 addr){
		u8 byte = nes->readByte(addr);
		extern bool boutput;
		if(addr == 0x4016 && byte == 0x41) boutput = true;
        return byte;
    }
    inline void writeByte(u16 addr, u8 byte){
        nes->writeByte(addr, byte);
    }
    inline u16 readWord(u16 addr){
        return nes->readWord(addr);
    }
    inline void writeWord(u16 addr, u16 word){
        nes->writeWord(addr, word);
    }

public:
    int init();
    int run(int);
    int exec(u8);
    void dump();
    void reset();
    void pause(){
        isRunning = false;
    }
    void start();

    void setNMI();
};


