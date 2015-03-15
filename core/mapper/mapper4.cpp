#include "mapper4.h"
#include "../log.h"

Mapper4::Mapper4(Nes *parent):Mapper(parent)
{
    LOGW("WARNING: Using Mapper4, with IRQ unimplemented! \n");
}

void Mapper4::reset() {
    reg_crtl = 0; 		//enable swapping for $8000 and $A000
                        //use normal address for command 0-5

    memset(reg_cmd, 0, sizeof(reg_cmd));

    set_ppu_block(0, 8, 0);     //swap first 8k chr-rom into ppu

    reg_cmd[6] = 0;
    reg_cmd[7] = 1;
    setupPrgRom();
}

void Mapper4::writeByte(u16 addr, u8 byte) {
    switch (addr) {
    case 0x8000:
        // if PRG address select changed
        if ((byte ^ reg_crtl) & 0x40) {
            setupPrgRom();
        }
        // if CHR address select changed
        if ((byte ^ reg_crtl) & 0x80) {
            setupChrRom();
        }
        reg_crtl = byte;
        break;
    case 0x8001:
    {
        u8 cmd = reg_crtl & 0xF;
        u16 mask = reg_crtl & 0x80 ? 0x4 : 0;

        reg_cmd[cmd] = byte;

        switch (cmd) {
        case 0:
            set_ppu_block(0^mask, 2, byte);
            break;
        case 1:
            set_ppu_block(2^mask, 2, byte);
            break;
        case 2:
            set_ppu_block(4^mask, 1, byte);
            break;
        case 3:
            set_ppu_block(5^mask, 1, byte);
            break;
        case 4:
            set_ppu_block(6^mask, 1, byte);
            break;
        case 5:
            set_ppu_block(7^mask, 1, byte);
        case 6:

        case 7:
            setupPrgRom();
            break;
        }
    }
        break;
    default:
        break;
    }
}

void Mapper4::setupChrRom() {
    u8 totalpage = header.rom_chr_num * 8;
    for (int i = 0; i < 6; ++i) {
        reg_cmd[i] %= totalpage;
    }
    u16 mask = reg_crtl & 0x80 ? 0x4 : 0;
    set_ppu_block(0^mask, 2, reg_cmd[0]);
    set_ppu_block(2^mask, 2, reg_cmd[1]);
    set_ppu_block(4^mask, 1, reg_cmd[2]);
    set_ppu_block(5^mask, 1, reg_cmd[3]);
    set_ppu_block(6^mask, 1, reg_cmd[4]);
    set_ppu_block(7^mask, 1, reg_cmd[5]);
}

void Mapper4::setupPrgRom() {
    u8 totalblock= header.rom_prg_num * 16;
    u8 totalpage = header.rom_prg_num * 2;
    reg_cmd[6] %= totalpage;
    reg_cmd[7] %= totalpage;
    if (reg_crtl & 0x40) {               // Enable swapping for $A000 and $C000
        set_prg_block(8, 8, (reg_cmd[6] ) << 3);
        set_prg_block(16, 8, (reg_cmd[7] ) << 3);

        set_prg_block(0, 8, totalblock - 16);
        set_prg_block(24, 8, totalblock - 8);
    } else {                        // Enable swapping for $8000 and $A000
        set_prg_block(0, 8, (reg_cmd[6] ) << 3);
        set_prg_block(8 , 8, (reg_cmd[7] )<< 3);

        set_prg_block(16, 8, totalblock - 16);
        set_prg_block(24, 8, totalblock - 8);
    }

}
