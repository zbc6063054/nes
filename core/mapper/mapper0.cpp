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
    set_prg_block(0, 16, 0);
    if(header.rom_prg_num == 1){
        set_prg_block(16, 16, 0);
    }else{
        set_prg_block(16, 16, 16);
    }

    set_ppu_block(0, 4, 0);
    set_ppu_block(4, 4, 4);
}

void Mapper0::writeByte(u16 addr, u8 byte){

}
