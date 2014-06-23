#include "mapper0.h"
#include "../nes.h"
#include "memory.h"

Mapper0::Mapper0(Nes *parent)
    :Mapper(parent)
{
}

Mapper0::~Mapper0(){

}

void Mapper0::reset(){
    if(header.rom_prg_num == 1){
        prg_rom[1] = prg_rom[0] = pPrgRom;
    }else{
        prg_rom[0] = pPrgRom;
        prg_rom[1] = pPrgRom + 0x4000;
    }
    chr_rom[0] = pChrRom;
    chr_rom[1] = pChrRom + 0x1000;

    set_ppu_block(0, 4, chr_rom[0]);
    set_ppu_block(4, 4, chr_rom[1]);
}

void Mapper0::writeByte(u16 addr, u8 byte){

}
