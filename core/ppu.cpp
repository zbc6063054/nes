#include "memory.h"
#include "ppu.h"

#define C1_NMI 			0x80 
#define C1_BGPALADDR 	0x10
#define C1_SPPALADDR 	0x08
#define C1_ADDRINC32	0x04 
#define C1_ADDRNT		0x03

#define C2_COLOR		0xE0
#define C2_SPVS			0x10
#define C2_BGVS			0x08
#define C2_SPCLIP		0x04
#define C2_BGCLIP		0x02
#define C2_COLORTYPE	0x01

#define STATUS_VBLANK	0x80
#define STATUS_HIT0		0x40
#define STATUS_SPCNT	0x20
#define STATUS_VRAM		0x10


StructPPU ppu;
struct struct_cpu;
extern struct struct_cpu cpu;

void setNMI();

void ppu_reset(){
	ppu.toggle = false;
}

u8 ppu_read_reg(u16 addr){
	u8 byte = 0;
	switch(addr){
	case 0x2002:
		//status register
		byte = ppu.reg_status;
		//reset toggle and VBlank
		ppu.reg_status &= ~STATUS_VBLANK;
		ppu.toggle = false; 
		break;
	case 0x2004:
		byte = spr_mem[ppu.reg_spr_addr++];
		break;
	case 0x2007:
		byte = ppu.vram_buf;
		addr = ppu.reg_vram_addr & 0x3FFF;
		if(ppu.reg_ctrl1 & C1_ADDRINC32){
			ppu.reg_vram_addr += 32;
		}else{
			ppu.reg_vram_addr += 1;
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
		ppu.vram_buf = ppu_mem[addr>>10][addr&0x3FF];
		break;
	default:
		//for write only registers
		byte = 0;
		break;
	}
	return byte;
}

void ppu_write_reg(u16 addr, u8 byte){
	switch(addr){
	case 0x2000:
		ppu.reg_temp = (ppu.reg_temp&0xF3FF)|((byte&0x03)<<10);
		//if NMI is disabled before and have a VBlank currently,post a NMI
		if(byte&(!ppu.reg_ctrl1)&C1_NMI
			&&(ppu.reg_status&STATUS_VBLANK)){
			setNMI();	
		}
		ppu.reg_ctrl1 = byte;
		break;
	case 0x2001:
		ppu.reg_ctrl2 = byte;
		break;
	case 0x2003:
		ppu.reg_spr_addr = byte;
		break;
	case 0x2004:
		spr_mem[ppu.reg_spr_addr++] = byte;
		break;
	case 0x2005:
		//first write
		if(!ppu.toggle){
			ppu.reg_temp = (ppu.reg_temp&0xFFE0)|(byte&0xF8);	
			ppu.offset_x = byte & 0x07;
		}else{
		//second write
			ppu.reg_temp = (ppu.reg_temp&0xFC1F)|((byte&0xF8)<<8);
			ppu.reg_temp = (ppu.reg_temp&0x7FFF)|((byte&0x07)<<12);
		}
		ppu.toggle = !ppu.toggle;
		break;
	case 0x2006:
		//first write
		if(!ppu.toggle){
			ppu.reg_temp = (ppu.reg_temp & 0xFF) | (((u16)byte & 0x3F)<<8);
		}else{
		//second write
			ppu.reg_temp = (ppu.reg_temp & 0xFF00)|(u16)addr;
			ppu.reg_vram_addr = ppu.reg_temp;
		}
		ppu.toggle = !ppu.toggle;
		break;
	case 0x2007:
		u16 addr = ppu.reg_vram_addr & 0x3FFF;
		if(ppu.reg_ctrl1 & C1_ADDRINC32){
			ppu.reg_vram_addr += 32;
		}else{
			ppu.reg_vram_addr += 1;
		}
		if(addr >= 0x3F00){
			byte &= 0x3F;
			//$3F00 or $3F10
			if(!addr&0x0F){
				bg_pattle[0] = spr_pattle[0] = byte;
			}else if(!addr&0x10){
				bg_pattle[addr&0x0F] = byte;
			}else{
				spr_pattle[addr&0x0F] = byte;
			}
			bg_pattle[0x04] = bg_pattle[0x08] = bg_pattle[0x0C] = bg_pattle[0];
			spr_pattle[0x04] = spr_pattle[0x08] = spr_pattle[0x0C] 
							 = spr_pattle[0x0C] = bg_pattle[0];
			return;
		}
		if(addr >= 0x3000){
			//mirror
			addr &= 0xEFFF;
		}
		ppu_mem[addr>>10][addr&0x3FF] = byte;	
		break;
	}
}


#define COLNUM 		32
#define ROWNUM 		30
#define BIT(x,n)	(((x)>>(n))&0x01)
#define RGB(r,g,b) 	((r<<16)|(g<<8)|b)

u32 sys_color_map[]={
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

void writePixel(int x, int y, u32 color){
}

u8 	screen_mem[34*8*31*8];

void ppu_scanline(){
	ppu.reg_vram_addr = (ppu.reg_vram_addr&0xFBE0)|(ppu.reg_temp&0x041F);
	u16 addr_nt = 0x2000 + ppu.reg_vram_addr & 0x0FFF;
	u16 addr_attr = 0x23C0 + ppu.reg_vram_addr&0x0C00
					+ (ppu.reg_vram_addr&0x0380)>>4;
	u8 title_x = ppu.reg_vram_addr & 0x1F, 
	   title_y = (ppu.reg_vram_addr & 0x03E0)>>5,
	   offset_y = (ppu.reg_vram_addr & 0x7000)>>12;
	u8 *lpScreen = &screen_mem[(ppu.reg_vram_addr&0x03E0|offset_y)*34*8
								+8-ppu.offset_x];
	u8 title, chr_low, chr_high, attr;
	u16 addr_title;
	for(int i=0; i<33; ++i){
		title = ppu_mem[addr_nt>>10][addr_nt&0x3FF];
		addr_title= (ppu.reg_ctrl1&C1_BGPALADDR)<<8 + title*16 + title_y;
		chr_low = ppu_mem[addr_title>>10][addr_title&0x3FF];
		chr_high = ppu_mem[addr_title>>10][addr_title&0x3FF+8];
		attr = ppu_mem[addr_attr>>10][addr_attr&0x3FF+title_x>>2];
		attr = (attr >> (((title_x&0x20)>>1) | (title_y&0x20))) & 0x03;
		u8* lpPal = &bg_pattle[attr<<2];
		{
			u8 ch = (chr_high&0xAA)|((chr_low&0xAA)>>1),
			   cl = ((chr_high&0x55)<<1)|(chr_low&0x55);
			lpScreen[0] = lpPal[ch>>6];
			lpScreen[2] = lpPal[(ch>>4)&3];
			lpScreen[4] = lpPal[(ch>>2)&3];
			lpScreen[6] = lpPal[ch&3];
			lpScreen[1] = lpPal[cl>>6];
			lpScreen[3] = lpPal[(cl>>4)&3];
			lpScreen[5] = lpPal[(cl>>2)&3];
			lpScreen[7] = lpPal[cl&3];
		}
		lpScreen += 8;
		++title_x;
		if(title_x == 32){
			title_x = 0;
			addr_nt ^= 0x41F;
		}else{
			++addr_nt;
		}
	}
	if(++title_y == 30){
		ppu.reg_vram_addr ^=0x0800;
		ppu.reg_vram_addr &=0xFC1F;
	}

}

int cpu_run(int);

void ppu_render_frame(){
	if(ppu.reg_ctrl2&(C2_SPVS|C2_BGVS)){
		ppu.reg_vram_addr = ppu.reg_temp;
	}
	int line = 0;
	while(true){
		if(line<240){
			cpu_run(CYCLES_SCANLINE);		
			ppu_scanline();	
		}
		++line;
		if(line == 241){
			ppu.reg_status |= STATUS_VBLANK;
		}
		if(line == TOTAL_SCAMLINE-1){
			ppu.reg_status &= ~STATUS_VBLANK;
			break;
		}
	}
}
