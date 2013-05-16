#include <string.h>
#include "memory.h"
#include "ppu.h"
#include "nes.h"
#include "cpu.h"

u32 PPU::sysColorMap[]={
    RGB(0x75,0x75,0x75),RGB(0x27,0x1B,0x8F),RGB(0x00,0x00,0xAB),RGB(0x47,0x00,0x9F),
    RGB(0x8F,0x00,0x77),RGB(0xAB,0x00,0x13),RGB(0xA7,0x00,0x00),RGB(0x7F,0x0B,0x00),
    RGB(0x43,0x2F,0x00),RGB(0x00,0x47,0x00),RGB(0x00,0x51,0x00),RGB(0x00,0x3F,0x17),
    RGB(0x1B,0x3F,0x5F),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),
    RGB(0xBC,0xBC,0xBC),RGB(0x00,0x73,0xEF),RGB(0x23,0x3B,0xEF),RGB(0x83,0x00,0xF3),
    RGB(0xBF,0x00,0xBF),RGB(0xE7,0x00,0x5B),RGB(0xDB,0x2B,0x00),RGB(0xCB,0x4F,0x0F),
    RGB(0x8B,0x73,0x00),RGB(0x00,0x97,0x00),RGB(0x00,0xAB,0x00),RGB(0x00,0x93,0x0B),
    RGB(0x00,0x83,0x8B),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),
    RGB(0xFF,0xFF,0xFF),RGB(0x3F,0xBF,0xFF),RGB(0x5F,0x97,0xFF),RGB(0xA7,0x8B,0xFD),
    RGB(0xF7,0x7B,0xFF),RGB(0xFF,0x77,0xB7),RGB(0xFF,0x77,0x63),RGB(0xFF,0x9B,0x3B),
    RGB(0xF3,0xBF,0x3F),RGB(0x83,0xD3,0x13),RGB(0x4F,0xDF,0x4B),RGB(0x58,0xF8,0x98),
    RGB(0x00,0xEB,0xDB),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),
    RGB(0xFF,0xFF,0xFF),RGB(0xAB,0xE7,0xFF),RGB(0xC7,0xD7,0xFF),RGB(0xD7,0xCB,0xFF),
    RGB(0xFF,0xC7,0xDB),RGB(0xFF,0xC7,0xDB),RGB(0xFF,0xBF,0xB3),RGB(0xFF,0xDB,0xAB),
    RGB(0xFF,0xE7,0xA3),RGB(0xE3,0xFF,0xA3),RGB(0xAB,0xF3,0xBF),RGB(0xB3,0xFF,0xCF),
    RGB(0x9F,0xFF,0xF3),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)
};
u32 sysColorMap2[64] =
{
   RGB(0x80,0x80,0x80), RGB(0x00,0x00,0xBB), RGB(0x37,0x00,0xBF), RGB(0x84,0x00,0xA6),
   RGB(0xBB,0x00,0x6A), RGB(0xB7,0x00,0x1E), RGB(0xB3,0x00,0x00), RGB(0x91,0x26,0x00),
   RGB(0x7B,0x2B,0x00), RGB(0x00,0x3E,0x00), RGB(0x00,0x48,0x0D), RGB(0x00,0x3C,0x22),
   RGB(0x00,0x2F,0x66), RGB(0x00,0x00,0x00), RGB(0x05,0x05,0x05), RGB(0x05,0x05,0x05),

   RGB(0xC8,0xC8,0xC8), RGB(0x00,0x59,0xFF), RGB(0x44,0x3C,0xFF), RGB(0xB7,0x33,0xCC),
   RGB(0xFF,0x33,0xAA), RGB(0xFF,0x37,0x5E), RGB(0xFF,0x37,0x1A), RGB(0xD5,0x4B,0x00),
   RGB(0xC4,0x62,0x00), RGB(0x3C,0x7B,0x00), RGB(0x1E,0x84,0x15), RGB(0x00,0x95,0x66),
   RGB(0x00,0x84,0xC4), RGB(0x11,0x11,0x11), RGB(0x09,0x09,0x09), RGB(0x09,0x09,0x09),

   RGB(0xFF,0xFF,0xFF), RGB(0x00,0x95,0xFF), RGB(0x6F,0x84,0xFF), RGB(0xD5,0x6F,0xFF),
   RGB(0xFF,0x77,0xCC), RGB(0xFF,0x6F,0x99), RGB(0xFF,0x7B,0x59), RGB(0xFF,0x91,0x5F),
   RGB(0xFF,0xA2,0x33), RGB(0xA6,0xBF,0x00), RGB(0x51,0xD9,0x6A), RGB(0x4D,0xD5,0xAE),
   RGB(0x00,0xD9,0xFF), RGB(0x66,0x66,0x66), RGB(0x0D,0x0D,0x0D), RGB(0x0D,0x0D,0x0D),

   RGB(0xFF,0xFF,0xFF), RGB(0x84,0xBF,0xFF), RGB(0xBB,0xBB,0xFF), RGB(0xD0,0xBB,0xFF),
   RGB(0xFF,0xBF,0xEA), RGB(0xFF,0xBF,0xCC), RGB(0xFF,0xC4,0xB7), RGB(0xFF,0xCC,0xAE),
   RGB(0xFF,0xD9,0xA2), RGB(0xCC,0xE1,0x99), RGB(0xAE,0xEE,0xB7), RGB(0xAA,0xF7,0xEE),
   RGB(0xB3,0xEE,0xFF), RGB(0xDD,0xDD,0xDD), RGB(0x11,0x11,0x11), RGB(0x11,0x11,0x11)
};

PPU::PPU(Nes *parent)
    :nes(parent){
    for(int i=0; i<256; ++i){
        u8 byte = 0;
        for(int j=0; j<8; ++j){
            if(i&(1<<j)){
                byte |= 1<<(7-j);
            }
        }
        revBit[i] = byte;
    }
}

void PPU::init(){

}

void PPU::reset(){
    reg_ctrl1 = 0;
    reg_ctrl2 = 0;
    reg_status = 0;
    reg_spr_addr = 0;
    reg_vram_addr = 0;
    reg_temp =0;
    toggle = false;
    offset_x = 0;
    vram_buf = 0;
    currentLine = 0;
    bgColor = BGCOLOR_BALCK;
    memset(screen_mem, 0, sizeof(screen_mem));
}

u8 PPU::readReg(u16 addr){
	u8 byte = 0;
	switch(addr){
	case 0x2002:
		//status register
        byte = reg_status;
		//reset toggle and VBlank
        reg_status &= ~STATUS_VBLANK;
        toggle = false;
		break;
	case 0x2004:
        byte = spr_mem[reg_spr_addr++];
		break;
	case 0x2007:
        byte = vram_buf;
        addr = reg_vram_addr & 0x3FFF;
        if(reg_ctrl1 & C1_ADDRINC32){
            reg_vram_addr += 32;
		}else{
            reg_vram_addr += 1;
		}
		if(addr>0x3F00){
			if(!(addr&0x10)){
				//$3F00-3FFF no buffer
				return bg_pattle[addr&0x0F];
			}else{
				return spr_pattle[addr&0x0F];
			}
		}
		if(addr >= 0x3000){
			//mirror
			addr &= 0xEFFF;
		}
        vram_buf = ppu_mem[addr>>10][addr&0x3FF];
		break;
	default:
		//for write only registers
        LOGW("read unreadable registers %04X !\n", addr);
		byte = 0;
		break;
	}
	return byte;
}

void PPU::writeReg(u16 addr, u8 byte){
	switch(addr){
	case 0x2000:
        reg_temp = (reg_temp&0xF3FF)|((byte&0x03)<<10);
		//if NMI is disabled before and have a VBlank currently,post a NMI
        if(byte&(!reg_ctrl1)&C1_NMI
            &&(reg_status&STATUS_VBLANK)){
            nes->cpu->setNMI();
		}
        reg_ctrl1 = byte;
		break;
	case 0x2001:
        reg_ctrl2 = byte;
        switch(reg_ctrl2 & C2_BGCOLOR){
        case C2_BGCOLOR_BALCK:
            bgColor = BGCOLOR_BALCK;
            break;
        case C2_BGCOLOR_GREEN:
            bgColor = BGCOLOR_GREEN;
            break;
        case C2_BGCOLOR_BLUE:
            bgColor = BGCOLOR_BLUE;
            break;
        case C2_BGCOLOR_RED:
            bgColor = BGCOLOR_RED;
            break;
        default:
            LOGW("unrecognized color %02X !\n", reg_ctrl2 & C2_BGCOLOR);
            break;
        }
		break;
	case 0x2003:
        reg_spr_addr = byte;
		break;
	case 0x2004:
        spr_mem[reg_spr_addr++] = byte;
		break;
	case 0x2005:
		//first write
        if(!toggle){
            reg_temp = (reg_temp&0xFFE0)|((byte&0xF8)>>3);
            offset_x = byte & 0x07;
		}else{
		//second write
            reg_temp = (reg_temp&0xFC1F)|(((u16)byte&0xF8)<<2);
            reg_temp = (reg_temp&0x8FFF)|(((u16)byte&0x07)<<12);
		}
        toggle = !toggle;
		break;
	case 0x2006:
		//first write
        if(!toggle){
            reg_temp = (reg_temp & 0x00FF) | (((u16)byte & 0x3F)<<8);
		}else{
		//second write
            reg_temp = (reg_temp & 0xFF00)|(u16)byte;
            reg_vram_addr = reg_temp;
		}
        toggle = !toggle;
		break;
	case 0x2007:
        addr = reg_vram_addr & 0x3FFF;
        if(reg_ctrl1 & C1_ADDRINC32){
            reg_vram_addr += 32;
		}else{
            reg_vram_addr += 1;
		}
		if(addr >= 0x3F00){
			byte &= 0x3F;
			//$3F00 or $3F10
            if(!(addr&0x0F)){
               bg_pattle[0] = spr_pattle[0] = byte;
            }else if(!(addr&0x10)){
               bg_pattle[addr&0x0F] = byte;
			}else{
               spr_pattle[addr&0x0F] = byte;
			}
			bg_pattle[0x04] = bg_pattle[0x08] = bg_pattle[0x0C] = bg_pattle[0];
			spr_pattle[0x04] = spr_pattle[0x08] = spr_pattle[0x0C] 
                             = spr_pattle[0x00] = bg_pattle[0];
			return;
		}
		if(addr >= 0x3000){
			//mirror
			addr &= 0xEFFF;
		}
		ppu_mem[addr>>10][addr&0x3FF] = byte;	
        break;
    default:
        LOGW("write to nonexits registers %04X !\n", addr);
        break;
	}
}

void PPU::vBlankStart(){
    reg_status |= STATUS_VBLANK;
    if(reg_ctrl1 & C1_NMI)
        nes->cpu->setNMI();
}

void PPU::vBlankEnd(){
    reg_status &= ~STATUS_VBLANK;
    reg_status &= ~STATUS_HIT0;
}

void PPU::setScanline(int line){
    currentLine = line;
}

void PPU::scanline(){
    if(reg_ctrl2 & (C2_BGVS | C2_SPVS)){
        reg_vram_addr = (reg_vram_addr&0xFBE0)|(reg_temp&0x041F);
    }
    u8 bg_info[34];
    memset(bg_info, 0, sizeof(bg_info));
    u32 *pScreenline = &screen_mem[currentLine*34*8];

    //draw background
    if(reg_ctrl2 & C2_BGVS){
        u16 addr_nt = 0x2000 + (reg_vram_addr & 0x0FFF);
        u16 addr_attr = 0x23C0 + (reg_vram_addr&0x0C00)
                        + ((reg_vram_addr&0x0380)>>4);             // tile_y/4 * 8
        u16 tile_x = reg_vram_addr & 0x1F,
           tile_y = (reg_vram_addr & 0x03E0)>>5,
           offset_y = (reg_vram_addr & 0x7000)>>12;
        u32 *pScreenloop = pScreenline + 8 - offset_x;
        u8 *pInfo = bg_info;
        if(pScreenloop >= (screen_mem+sizeof(screen_mem))){
            LOGE("screen memory out of range!\n");
        }
        ASSERT(pScreenloop < (screen_mem+sizeof(screen_mem)));
        u8 title, chr_low, chr_high, attr;
        u16 addr_title;
        for(int i=0; i<33; ++i){
            title = ppu_mem[addr_nt>>10][addr_nt&0x3FF];
            addr_title= ((reg_ctrl1&C1_BGCHRADDR)<<8) + title*16 + offset_y;
            chr_low = ppu_mem[addr_title>>10][addr_title&0x3FF];
            chr_high = ppu_mem[addr_title>>10][(addr_title&0x3FF)+8];
            ASSERT(((addr_attr&0x3FF)+(tile_x>>2))<1024);
            attr = ppu_mem[addr_attr>>10][(addr_attr&0x3FF)+(tile_x>>2)];
            attr = (attr >> ((tile_x&0x02) + ((tile_y&0x02)<<1))) & 0x03;
            *pInfo = chr_high | chr_low;
            u8* pPal = &bg_pattle[attr<<2];
            {
                u8 ch = (chr_high&0xAA)|((chr_low&0xAA)>>1),
                   cl = ((chr_high&0x55)<<1)|(chr_low&0x55);
                pScreenloop[0] = sysColorMap[ pPal[ch>>6] ];
                pScreenloop[2] = sysColorMap[ pPal[(ch>>4)&3] ];
                pScreenloop[4] = sysColorMap[ pPal[(ch>>2)&3] ];
                pScreenloop[6] = sysColorMap[ pPal[ch&3] ];
                pScreenloop[1] = sysColorMap[ pPal[cl>>6] ];
                pScreenloop[3] = sysColorMap[ pPal[(cl>>4)&3] ];
                pScreenloop[5] = sysColorMap[ pPal[(cl>>2)&3] ];
                pScreenloop[7] = sysColorMap[ pPal[cl&3] ];

//                pScreenloop[0] = sysColorMap[ BIT(*pInfo, 7) ? pPal[ch>>6] : bgColor ];
//                pScreenloop[2] = sysColorMap[ BIT(*pInfo, 5) ? pPal[(ch>>4)&3] : bgColor ];
//                pScreenloop[4] = sysColorMap[ BIT(*pInfo, 3) ? pPal[(ch>>2)&3] : bgColor ];
//                pScreenloop[6] = sysColorMap[ BIT(*pInfo, 1) ? pPal[ch&3] : bgColor ];
//                pScreenloop[1] = sysColorMap[ BIT(*pInfo, 6) ? pPal[cl>>6] : bgColor ];
//                pScreenloop[3] = sysColorMap[ BIT(*pInfo, 4) ? pPal[(cl>>4)&3] : bgColor ];
//                pScreenloop[5] = sysColorMap[ BIT(*pInfo, 2) ? pPal[(cl>>2)&3] : bgColor ];
//                pScreenloop[7] = sysColorMap[ BIT(*pInfo, 0) ? pPal[cl&3] : bgColor ];
            }
            pScreenloop += 8;
            ++pInfo;
            ++tile_x;
            if(tile_x == 32){
                tile_x = 0;
                addr_nt ^= 0x41F;
                addr_attr = (addr_nt&0x0C00) + 0x23C0 + ((addr_nt&0x0380)>>4);
            }else{
                ++addr_nt;
            }
        }
        if( (reg_vram_addr & 0x7000) == 0x7000 ) {           //if offset_y = 7
            reg_vram_addr &= 0x8FFF;                         //     offset_y = 0
            if( (reg_vram_addr & 0x03E0) == 0x03A0 ) {       //     if tile_y = 29
                reg_vram_addr ^= 0x0800;                     //         switch bit 11
                reg_vram_addr &= 0xFC1F;                     //         set tile_y = 0
            } else {
                if( (reg_vram_addr & 0x03E0) == 0x03E0 ) {   //     if tile_y = 31
                    reg_vram_addr &= 0xFC1F;                 //         set tile_y = 0
                } else {                                     //     else
                    reg_vram_addr += 0x0020;                 //         tile_y++
                }
            }
        } else {
            reg_vram_addr += 0x1000;                         // offset_y++
        }
    }

    //draw sprite
    if(currentLine > 239){
        return;
    }

    reg_status &= ~STATUS_SPCNT;
    if(reg_ctrl2 & C2_SPVS){
        Sprite *pSp = (Sprite *)spr_mem;
        u8 sp_h = (reg_ctrl1 & C1_SP16)? 0x0F : 0x07;
        u16 addr_tile = 0;
        u8 sp_y, sp_high, sp_low, sp_info;
        u8 sp_buf[8];
        u8 count = 0;
        u32 *pScreenSp;
        for(int i=0; i<63; ++i, ++pSp){
            if(!((currentLine >= (pSp->pos_y+1))&&
                ((sp_y = currentLine - (pSp->pos_y + 1))<=sp_h) ))
                continue;
            pScreenSp = pScreenline + pSp->pos_x + 8;           //skip the first column
            addr_tile = (((u16)(reg_ctrl1&C1_SPCHRADDR))<<9) + (((u16)pSp->index) << 4);
            if(!(reg_ctrl1 & C1_SP16)){
            //for 8 x 8
                if(pSp->attr & SP_VTLREV){
                    addr_tile += 7-sp_y;
                }else{
                    addr_tile += sp_y;
                }
                sp_low = ppu_mem[addr_tile>>10][addr_tile&0x3FF];
                sp_high = ppu_mem[addr_tile>>10][(addr_tile&0x3FF) + 8];
                if(pSp->attr & SP_HTLREV){
                    sp_low = revBit[sp_low];
                    sp_high = revBit[sp_high];
                }
                sp_info = sp_high | sp_low;
                u8* pPal = &spr_pattle[(pSp->attr&SP_HCOLOR) << 2];
                {
                    u8 ch = (sp_high&0xAA)|((sp_low&0xAA)>>1),
                       cl = ((sp_high&0x55)<<1)|(sp_low&0x55);
                    sp_buf[0] = pPal[ch>>6];
                    sp_buf[2] = pPal[(ch>>4)&3];
                    sp_buf[4] = pPal[(ch>>2)&3];
                    sp_buf[6] = pPal[ch&3];
                    sp_buf[1] = pPal[cl>>6];
                    sp_buf[3] = pPal[(cl>>4)&3];
                    sp_buf[5] = pPal[(cl>>2)&3];
                    sp_buf[7] = pPal[cl&3];
                }

                u8 info = sp_info;
                u8 addr_info_bg = (pSp->pos_x + (offset_x&0x07))>>3;
                u8 info_bg = ((((u16)bg_info[addr_info_bg]) << 8) | ((u16)bg_info[addr_info_bg + 1]))
                        >> (7 - ((pSp->pos_x + (offset_x&0x07)) & 0x07));
                if((i==0) && (!(reg_status&STATUS_HIT0)) && (sp_info&info_bg)){
                    reg_status |= STATUS_HIT0;
                }
                if((pSp->attr & SP_PRIORITY)){
                    info &= ~ info_bg;
                }
                for(int j=0; j<8; j++, pScreenSp++){
                    if((info>>(7-j))&1){
                        *pScreenSp = sysColorMap[ sp_buf[j] ];
                    }
                }
            }else{
            //for 8 x 16 TODO
                LOGE("unsupport tile size 8*16 !\n");
            }
            ++count;
            if(count > 8){
 //               reg_status |= STATUS_SPCNT;
 //               break;
            }
        }

    }

}

void PPU::renderTile(u8 index, u8 attr, u32 *pScreen, u8* pattle, u16 addr_base){
    u16 addr_tile = addr_base + index*16;
    u8 chr_low, chr_high;
    for(int i=0; i<8; i++){
        chr_low = ppu_mem[addr_tile>>10][addr_tile&0x3FF];
        chr_high = ppu_mem[addr_tile>>10][(addr_tile&0x3FF)+8];
        u8* pPal = &pattle[attr<<2];
        {
            u8 ch = (chr_high&0xAA)|((chr_low&0xAA)>>1),
               cl = ((chr_high&0x55)<<1)|(chr_low&0x55);
            pScreen[0] = sysColorMap[ pPal[ch>>6] ];
            pScreen[2] = sysColorMap[ pPal[(ch>>4)&3] ];
            pScreen[4] = sysColorMap[ pPal[(ch>>2)&3] ];
            pScreen[6] = sysColorMap[ pPal[ch&3] ];
            pScreen[1] = sysColorMap[ pPal[cl>>6] ];
            pScreen[3] = sysColorMap[ pPal[(cl>>4)&3] ];
            pScreen[5] = sysColorMap[ pPal[(cl>>2)&3] ];
            pScreen[7] = sysColorMap[ pPal[cl&3] ];
        }
        addr_tile += 1;
        pScreen += 8;
    }
}

// 16 x 16
void PPU::drawPattern(u8 attr, u32 *pScreen){
    u32 buf[8*8];
    u16 addr_base = ((u16) reg_ctrl1 & C1_BGCHRADDR)<<8;
    memset(buf, 0, sizeof(buf));
    int i = 0;
    for(int row=0; row<16; ++row){
        for(int col=0; col<16; ++col){
            renderTile(i, attr, buf, bg_pattle, addr_base);
            for(int j=0; j<8; ++j){
                memcpy(pScreen+j*128, buf+j*8, sizeof(u32)*8);
            }
            pScreen += 8;
            ++i;
        }
        pScreen += 128*7;
    }
}

// 8 x 8
void PPU::drawSprite(u32 *pScreen){
    u32 buf[8*8];
    u16 addr_base = ((u16) reg_ctrl1 & C1_SPCHRADDR)<<9;
    Sprite *pSp = (Sprite *)spr_mem;
    for(int i=0; i<64; ++i, ++pSp){
        renderTile(pSp->index, pSp->attr, buf, spr_pattle, addr_base);
        for(int j=0; j<8; ++j){
            memcpy(pScreen+j*64, buf+j*8, sizeof(u32)*8);
        }
        pScreen += 8;
        if((i+1)%8 == 0)
            pScreen += 7*64;
    }
}

// 512 x 480
// 256 x 240
void PPU::drawNameTable(u32 *pScreen, u8 tableIndex){
    u32 buf[8*8];
    u16 addr_nt = 0x2000 + 0x400*tableIndex, addr_attrBase = addr_nt + 0x03C0,
        addr_attr = 0, addr_base = ((u16) reg_ctrl1 & C1_BGCHRADDR)<<8;
    u8 attr = 0;
    for(int row=0; row<30; ++row){
        for(int col=0; col<32; ++col){
            addr_attr = addr_attrBase + (((u16)row&0xFC)<<1) + (col>>2);
            attr = ppu_mem[addr_attr>>10][addr_attr&0x3FF];
            attr >>= ((row&0x02)<<1) | (col&0x02);
            renderTile(ppu_mem[addr_nt>>10][addr_nt&0x3FF], (attr&0x03), buf, bg_pattle, addr_base);
            for(int j=0; j<8; ++j){
                memcpy(pScreen+j*256, buf+j*8, sizeof(u32)*8);
            }
            addr_nt ++;
            pScreen += 8;
        }
        pScreen += 7*256;
    }
}

