#ifndef ROM_H
#define ROM_H

#include <stdio.h>
#include "memory.h"

typedef struct struct_file_header{
    int rom_prg_num;        //16k size prg bank
    int rom_chr_num;        //8k size chr bank
    NtMirrorType mirror_type;
    bool battary_backed;
    bool have_trainer;
    bool four_screen_layout;
    int mapper_type;
} FileHeader;

class Mapper;
class Nes;

class Rom
{
public:
    Rom(Nes *parent);
    ~Rom();

private:
    FileHeader header;
    Nes *nes;
    Mapper* mapper;
    u8 *pPrgRom;
    u8 *pChrRom;
    bool isLoadFile;

private:
    int getHead(FILE *fp, FileHeader& head);
    u8* loadPrg(FILE *fp, const FileHeader &head);
    u8* loadChr(FILE *fp, const FileHeader &head);

public:
    int readFile(const char *file);
    void deleteRom();
    void reset();
    inline bool getLoadState(){
        return isLoadFile;
    }
    inline Mapper* getMapper(){
        return mapper;
    }
};

#endif // ROM_H
