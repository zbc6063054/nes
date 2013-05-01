#include <stdio.h>
#include <string.h>

#include "../global.h"
#include "memory.h"
#include "file.h"

#define ERROR -1
#define BIT(b, x) ((b) & (1<<(x)) > 0)

int file_get_head(FILE* fp, file_header& header){
	fseek(fp, 0, SEEK_SET);
	u8 buffer[16];
	if(fread(buffer, sizeof(u8), 16, fp) != 16 * sizeof(u8)){  //failed to get enough bytes
		return ERROR;
	}
	if(!(buffer[0]==0x4e && buffer[1]==0x45 
			&& buffer[2] ==0x53 && buffer[3]==0x1a)){			//file format error
		return ERROR;
	}

	header.rom_prg_num = buffer[4];
	header.rom_chr_num = buffer[5];

	header.mirror_type = BIT(buffer[6],0)? VERTICAL_MIRROR : HORIZONTAL_MIRROR;
	header.battary_backed = BIT(buffer[6], 1);
	header.have_trainer = BIT(buffer[6], 2);
	header.four_screen_layout = BIT(buffer[6], 3);

	header.mapper_type = (buffer[6]>>4) & buffer[7];

	return 0;
}

//load the prg_rom and chr_rom to memory
int file_load(char* filename){
	FILE* fp;
	if(!(fp = fopen(filename, "rb"))){						// failed to open file
		return ERROR;
	}

	file_header header;
	int res = file_get_head(fp, header);
	if(res == 0){											//read head successfully
		file_load_prg(fp, header);	
	}

	fclose(fp);
	return res;
}

void file_load_prg(FILE* fp, const file_header& header){
	int offset = 16;
	if(header.have_trainer){
		offset += 256;
	}
	fseek(fp, offset, SEEK_SET);
	if(header.rom_prg_num == 1){ 							//one 8K prg_rom
		fread(prg_rom, sizeof(u8), 0x4000, fp);	        	//load to 0x8000
		memcpy(prg_rom, prg_rom + 0x4000, 
				sizeof(u8) * 0x4000);						//simplely copy it to 0xC000
	}else if(header.rom_prg_num == 2){
		fread(prg_rom, sizeof(u8), 0x8000, fp);
	}
}

void file_load_chr(FILE* fp, const file_header& header){
    int offset = 16 + header.rom_prg_num*0x4000;
    if(header.have_trainer){
        offset += 256;
    }
    fseek(fp, offset, SEEK_SET);
    if(header.rom_chr_num == 1){
        fread(chr_rom, sizeof(u8), 0x1000, fp);
    }else if(header.rom_chr_num == 2){
        fread(chr_rom, sizeof(u8), 0x2000, fp);
    }
}
