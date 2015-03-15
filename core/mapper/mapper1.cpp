#include "mapper1.h"
#include "../nes.h"
#include "../memory.h"
#include "../cpu.h"

Mapper1::Mapper1(Nes *parent)
    :Mapper(parent)
{
}

Mapper1::~Mapper1(){

}

void Mapper1::reset(){
    reg0 = 0x0C;
    reg1 = reg2 = 0;
    buf = 0;
    offset = 0;
    setPrgBank(BANK_LOW, 0);
    setPrgBank(BANK_HIGH, header.rom_prg_num - 1);
    setChrBank(BANK_ALL, 0);
}

void Mapper1::writeByte(u16 addr, u8 byte){
    if(byte & FLAG_RESET){
        buf = 0;
        offset = 0;
        reg0 |= 0x0C;
        return;
    }

    buf |= (byte&1)<<offset;
    if(++offset<5){
        return;
    }
    byte = buf;
    buf = 0;
    offset = 0;
    u8 code = (addr & 0x6000)>>13;
	u8 val = 0;
    switch(code){
    case 0:
        reg0 = byte;
        if(byte & REG0_ONERSCREENMIRROR){
            if(byte & REG0_VHMIRROR){
                set_nt_mirror(MIRROR_HORIZONTAL);
            }else{
                set_nt_mirror(MIRROR_VERTICAL);
            }
        }else{
            set_nt_mirror(MIRROR_SINGLE);
        }
        break;
    case 1:
        reg1 = byte;
		val = byte & 0x10 ? (byte&0x0F) + header.rom_chr_num : (byte & 0x0F);
        if(reg0 & REG0_VROM4KSIZE){
			setChrBank(BANK_LOW, val);
        }else{
            setChrBank(BANK_ALL, val & 0xFE);
        }
        break;
    case 2:
        reg2 = byte;
		val = byte & 0x10 ? (byte&0x0F) + header.rom_chr_num : (byte & 0x0F);
		if(reg0 & REG0_VROM4KSIZE){
			setChrBank(BANK_HIGH, val);
		}
        break;
    case 3:
		val = byte & 0x10 ? (byte&0x0F) + (header.rom_prg_num>>1) : (byte & 0x0F);
        if(reg0 & REG0_PRGROM16KSIZE){
            if(reg0 & REG0_PRGROMPOS8000){
				ASSERT(nes->cpu->reg_pc >= 0xC000);
                setPrgBank(BANK_LOW, val);
            }else{
				ASSERT(nes->cpu->reg_pc < 0xC000);
                setPrgBank(BANK_HIGH, val);
            }
        }else{
            setPrgBank(BANK_ALL, val&0xFE);
        }
        break;
    default:
        break;
    }
}
