#include "memmap.h"
#include "nes.h"
#include "ppu.h"
#include "cpu.h"
#include "controller.h"
#include "mapper/mapper.h"

StructMemMap MemMap::mapper[]= {
	   { 0x8000, 0xFFFF, MEMMAP_FUNCTION, (void *)rom_access_func },
       { 0, 0x1FFF, MEMMAP_FUNCTION, (void *)cpu_access_func },
       { 0x2000, 0x3FFF, MEMMAP_FUNCTION, (void *)ppu_access_func },
       { 0x4000, 0x40FF, MEMMAP_FUNCTION, (void *)reg_access_func },
       { 0x4100, 0x5FFF, MEMMAP_ADDRESS, (void *)ex_rom },
       { 0x6000, 0x7FFF, MEMMAP_ADDRESS, (void *)save_ram },
   };

MemMap::MemMap(Nes *parent)
       :nes(parent)
{
	
}

void MemMap::init(){
    mem_init();
}

void MemMap::reset(){

}

u8 MemMap::readByte(u16 addr){
    u8 byte = 0;
    memAccessControl(addr, byte, MEMREAD);
    return byte;
}

void MemMap::writeByte(u16 addr, BYTE byte){
    memAccessControl(addr, byte, MEMWRITE);
}

u16 MemMap::readWord(u16 addr){
    return ((u16)readByte(addr+1)<<8) | (u16)readByte(addr);
}

void MemMap::writeWord(u16 addr, u16 word){
    writeByte(addr, (u8)word);
    writeByte(addr+1, (u8)(word>>8));
}

void MemMap::memAccessControl(u16 addr, u8& byte, AccessType type){
    int i = 0, len = sizeof(mapper) / sizeof(mapper[0]);
    for(i=0; i<len; ++i){
        if(addr>=mapper[i].addr_start && addr<=mapper[i].addr_end){
            if(mapper[i].type == MEMMAP_FUNCTION){
                ((MemAccessFunc)mapper[i].data)(this, addr, byte, type);
            }else{
                if(type == MEMREAD){
                    byte = ((u8 *)mapper[i].data)[addr - mapper[i].addr_start];
                }else{
                    ((u8 *)mapper[i].data)[addr - mapper[i].addr_start] = byte;
                }
            }
            break;
        }
    }
    if(i == len){
        LOGW("memroy address %04X are not in range!\n", addr);
    }
}

void MemMap::cpu_access_func(MemMap *map, u16 addr, u8& byte, AccessType type){
    addr = addr & 0x7FF;				//memory mirror
    if(type == MEMREAD){
        byte = cpu_mem[addr];
    }else{
        cpu_mem[addr] = byte;
    }
}

void MemMap::ppu_access_func(MemMap *map, u16 addr, u8& byte, AccessType type){
    addr &= 0xE007;
    if(type == MEMREAD){
        byte = map->nes->ppu->readReg(addr);
    }else{
        map->nes->ppu->writeReg(addr, byte);
    }
}

void MemMap::ctl_access_func(MemMap *map, u16 addr, u8& byte, AccessType type){

}

void MemMap::reg_access_func(MemMap *map, u16 addr, u8& byte, AccessType type){
    switch(addr){
        case 0x4014:
			if(type == MEMWRITE){
				DMA(byte);
				map->nes->cpu->preCycles += CYCLES_DMA;
			}else{
				byte = addr & 0xFF;
			}
            break;
        case 0x4016:
        case 0x4017:
            if(type == MEMREAD){
                byte = map->nes->control->readByte(addr);
            }else{
                map->nes->control->writeByte(addr, byte);
            }
            break;
        default:
            //TODO
            if(type == MEMREAD){
                byte = 0;
            }
//          LOGW("access un implement register %04X !\n", addr);
            break;
    }
}


//TODO
void MemMap::rom_access_func(MemMap *map, u16 addr, u8& byte, AccessType type){
    if(type == MEMREAD){
        byte = PRG_ROM(addr - 0x8000);
    }else{
        map->nes->rom->getMapper()->writeByte(addr, byte);
    }
}
