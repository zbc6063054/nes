#ifndef ROM_H
#define ROM_H

#include <stdio.h>
#include "memory.h"

typedef struct struct_file_header{
    int rom_prg_num;
    int rom_chr_num;
    NtMirrorType mirror_type;
    bool battary_backed;
    bool have_trainer;
    bool four_screen_layout;
    int mapper_type;
} FileHeader;

class Nes;

class Rom
{
public:
    Rom(Nes *parent);

private:
    FileHeader header;
    Nes *nes;
    bool isLoadFile;

private:
    int getHead(FILE *fp, FileHeader& head);
    void loadPrg(FILE *fp, const FileHeader &head);
    void loadChr(FILE *fp, const FileHeader &head);

public:
    int readFile(const char *file);
    inline bool getLoadState(){
        return isLoadFile;
    }
};

#endif // ROM_H
