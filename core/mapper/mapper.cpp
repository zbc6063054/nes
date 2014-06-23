#include "mapper.h"
#include "../nes.h"
#include <string.h>
#include "../log.h"

Mapper::Mapper(Nes *parent)
    :nes(parent)
{
    memset(&header, 0, sizeof(header));
}

Mapper::~Mapper(){

}

void Mapper::setChrBank(Bank bank, int bankNum){
	ASSERT(header.rom_chr_num>0 ? header.rom_chr_num*2 > bankNum:true);
    switch(bank){
    case BANK_LOW:
        set_ppu_block(0, 4, pChrRom + bankNum * 0x1000);
        break;
    case BANK_HIGH:
        set_ppu_block(4, 4, pChrRom + bankNum * 0x1000);
        break;
    case BANK_ALL:
        set_ppu_block(0, 4, pChrRom + bankNum * 0x1000);
        set_ppu_block(4, 4, pChrRom + (bankNum+1) * 0x1000);
        break;
    }
}

void Mapper::setPrgBank(Bank bank, int bankNum){
    ASSERT(header.rom_prg_num > bankNum);
    switch(bank){
    case BANK_LOW:
        prg_rom[0] = pPrgRom + bankNum * 0x4000;
        break;
    case BANK_HIGH:
        prg_rom[1] = pPrgRom + bankNum * 0x4000;
        break;
    case BANK_ALL:
        prg_rom[0] = pPrgRom + bankNum * 0x4000;
        prg_rom[1] = prg_rom[0] + 0x4000;
        break;
    }
}

#include "mapper0.h"
#include "mapper1.h"
#include "mapper2.h"
Mapper* Mapper::getMapper(Nes *nes, int code){
    switch(code){
    case 0:										//Mapper 0
        return new Mapper0(nes);
	case 1:
		return new Mapper1(nes);
    case 2:
        return new Mapper2(nes);
    default:
        return NULL;
    }
}
