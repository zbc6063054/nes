#include "mapper2.h"

Mapper2::Mapper2(Nes *parent)
    :Mapper(parent)
{
}

Mapper2::~Mapper2(){

}

void Mapper2::reset(){
    setPrgBank(BANK_LOW, 0);
    setPrgBank(BANK_HIGH, header.rom_prg_num-1);
    setChrBank(BANK_ALL, 0);
}

void Mapper2::writeByte(u16 addr, u8 byte){
    setPrgBank(BANK_LOW, byte);
}
