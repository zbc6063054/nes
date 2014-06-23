#include <string.h>
#include "rom.h"
#include "nes.h"
#include "log.h"
#include "memory.h"
#include "mapper/mapper.h"

#define FREEPTR(x) if((x) != NULL) \
                   { delete (x); x = NULL; }
#define FREEPTRARRAY(x) if((x) != NULL) \
                   { delete[] (x); x = NULL; }

Rom::Rom(Nes *parent)
    :nes(parent)
{
    memset(&header, 0, sizeof(header));
    isLoadFile = false;
    mapper = NULL;
    pPrgRom = NULL;
    pChrRom = NULL;
}

Rom::~Rom(){
    if(isLoadFile){
        deleteRom();
    }
}

void Rom::reset(){
    if(mapper != NULL){
        mapper->reset();
    }
}

void Rom::deleteRom(){
    FREEPTR(mapper);
    FREEPTRARRAY(pPrgRom);
    FREEPTRARRAY(pChrRom);
	isLoadFile = false;
}

//load the prg_rom and chr_rom to memory
int Rom::readFile(const char *file){
    if(isLoadFile){
        deleteRom();
    }

    FILE* fp;
    if(!(fp = fopen(file, "rb"))){                          // failed to open file
        LOGI("open file [%s] error!\n", file);
        return NES_ERROR_OPENFILE;
    }

    int res = getHead(fp, header);
    if(res != NES_ERROR_OK){
		fclose(fp);
        return res;
    }
    mapper = Mapper::getMapper(nes, header.mapper_type);
    if(mapper == NULL){
        LOGE("unsupport mapper type %02X!\n", header.mapper_type);
		fclose(fp);
        return NES_ERROR_UNSUPPORTMAPPER;
    }

    if( !(pPrgRom = loadPrg(fp, header)) ){
		fclose(fp);
        return NES_ERROR_LOADFILE;
    }
    if( !(pChrRom = loadChr(fp, header)) ){
        FREEPTRARRAY(pPrgRom);
		fclose(fp);
        return NES_ERROR_LOADFILE;
    }

    set_nt_mirror(header.mirror_type);
    mapper->setFileHeader(header);
    mapper->setPrgRom(pPrgRom);
    mapper->setChrRom(pChrRom);
    mapper->reset();

    isLoadFile = true;
    fclose(fp);
    return NES_ERROR_OK;
}

int Rom::getHead(FILE* fp, FileHeader& header){
    fseek(fp, 0, SEEK_SET);
    u8 buffer[16];
    if(fread(buffer, sizeof(u8), 16, fp) != 16 * sizeof(u8)){  //failed to get enough bytes
        LOGE("read file header error:unexpected file end!\n");
        return NES_ERROR_FILEFORMAT;
    }
    if(!(buffer[0] == 0x4e && buffer[1] == 0x45
            && buffer[2] == 0x53 && buffer[3] == 0x1a)){			//file format error
        LOGE("file format error!\n");
        return NES_ERROR_FILEFORMAT;
    }

    header.rom_prg_num = buffer[4];
    header.rom_chr_num = buffer[5];

    header.mirror_type = BIT(buffer[6], 0)? MIRROR_VERTICAL : MIRROR_HORIZONTAL;
    header.battary_backed = BIT(buffer[6], 1);
    header.have_trainer = BIT(buffer[6], 2);
    header.four_screen_layout = BIT(buffer[6], 3);

    header.mapper_type = (buffer[6]>>4) | (buffer[7]&0xF0);

    return NES_ERROR_OK;
}

u8* Rom::loadPrg(FILE* fp, const FileHeader& header){
    int offset = 16;
    if(header.have_trainer){
        offset += 512;
    }
    fseek(fp, offset, SEEK_SET);
    int size = header.rom_prg_num*0x4000;
    u8 *pPrg = new u8[size];

    int len = fread(pPrg, sizeof(u8), size, fp);
    if(len != size){                                // file is too short
        FREEPTRARRAY(pPrg);
        LOGE("load prg rom error: unexpected file end!\n");
        return NULL;
    }
    return pPrg;
}

u8* Rom::loadChr(FILE* fp, const FileHeader& header){
    int offset = 16 + header.rom_prg_num*0x4000;
    if(header.have_trainer){
        offset += 512;
    }
    fseek(fp, offset, SEEK_SET);

    int size = header.rom_chr_num*0x2000;
	if(size != 0){
		u8 *pChr = new u8[size];

		int len = fread(pChr, sizeof(u8), size, fp);
		if(len != size){                                // file is too short
			FREEPTRARRAY(pChr);
			LOGE("load chr rom error: unexpected file end!\n");
			return NULL;
		}
		return pChr;
	}
	return new u8[0x2000];
}
