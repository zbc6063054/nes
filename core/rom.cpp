#include <string.h>
#include "rom.h"
#include "nes.h"
#include "log.h"
#include "memory.h"

Rom::Rom(Nes *parent)
    :nes(parent)
{
    memset(&header, 0, sizeof(header));
    isLoadFile = false;
}

//load the prg_rom and chr_rom to memory
int Rom::readFile(const char *file){
    FILE* fp;
    if(!(fp = fopen(file, "rb"))){                          // failed to open file
        LOGI("open file [%s] error!\n", file);
        return ERROR;
    }

    int res = getHead(fp, header);
    if(res != ERROR_OK){
        //left blank
    }else if(header.mapper_type != 0){
        LOGE("unsupport mapper type %02X!\n", header.mapper_type);
        res = ERROR;
    }else{
        set_nt_mirror(header.mirror_type);
        loadPrg(fp, header);
        loadChr(fp, header);
        isLoadFile = true;
    }

    fclose(fp);
    return res;
}

int Rom::getHead(FILE* fp, FileHeader& header){
    fseek(fp, 0, SEEK_SET);
    u8 buffer[16];
    if(fread(buffer, sizeof(u8), 16, fp) != 16 * sizeof(u8)){  //failed to get enough bytes
        LOGE("failed to get enough bytes!\n");
        return ERROR;
    }
    if(!(buffer[0]==0x4e && buffer[1]==0x45
            && buffer[2] ==0x53 && buffer[3] == 0x1a)){			//file format error
        LOGE("file format error!\n");
        return ERROR;
    }

    header.rom_prg_num = buffer[4];
    header.rom_chr_num = buffer[5];

    header.mirror_type = BIT(buffer[6],0)? MIRROR_VERTICAL : MIRROR_HORIZONTAL;
    header.battary_backed = BIT(buffer[6], 1);
    header.have_trainer = BIT(buffer[6], 2);
    header.four_screen_layout = BIT(buffer[6], 3);

    header.mapper_type = (buffer[6]>>4) & buffer[7];

    return ERROR_OK;
}

void Rom::loadPrg(FILE* fp, const FileHeader& header){
    int offset = 16;
    if(header.have_trainer){
        offset += 256;
    }
    fseek(fp, offset, SEEK_SET);
    if(header.rom_prg_num == 1){ 							//one 8K prg_rom
        fread(prg_rom, sizeof(u8), 0x4000, fp);             //load to 0x8000
        memcpy(prg_rom + 0x4000, prg_rom,
                sizeof(u8) * 0x4000);						//simplely copy it to 0xC000
    }else if(header.rom_prg_num == 2){
        fread(prg_rom, sizeof(u8), 0x8000, fp);
    }
}

void Rom::loadChr(FILE* fp, const FileHeader& header){
    int offset = 16 + header.rom_prg_num*0x4000;
    if(header.have_trainer){
        offset += 256;
    }
    fseek(fp, offset, SEEK_SET);
    if(header.rom_chr_num == 1){
        fread(chr_rom, sizeof(u8), 0x2000, fp);
    }else if(header.rom_chr_num == 2){
        fread(chr_rom, sizeof(u8), 0x2000, fp);
    }
}
