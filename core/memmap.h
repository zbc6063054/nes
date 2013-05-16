#ifndef MEMMAP_H
#define MEMMAP_H

#include "memory.h"


enum AccessType{
    MEMREAD = 0,
    MEMWRITE = 1
};

enum MemMapType{
    MEMMAP_FUNCTION = 0,
    MEMMAP_ADDRESS = 1
};

class MemMap;

typedef void (* MemAccessFunc)(MemMap *map, u16 addr,
                    u8& byte,AccessType type);

//range from [addr_start, addr_end]
typedef struct {
    u16 addr_start;
    u16 addr_end;
    MemMapType type;
    void* data;
} StructMemMap;

class Nes;

class MemMap
{
public:
    MemMap(Nes *parent);

private:
    Nes *nes;

    static StructMemMap mapper[];

private:
    void memAccessControl(u16 addr, u8& byte, AccessType type);
    static void cpu_access_func(MemMap *map, u16 addr, u8& byte, AccessType type);
    static void ppu_access_func(MemMap *map, u16 addr, u8& byte, AccessType type);
    static void reg_access_func(MemMap *map, u16 addr, u8& byte, AccessType type);
    static void ctl_access_func(MemMap *map, u16 addr, u8& byte, AccessType type);
    static void rom_access_func(MemMap *map, u16 addr, u8& byte, AccessType type);

public:
    u8 readByte(u16);
    u16 readWord(u16);
    void writeByte(u16, u8);
    void writeWord(u16, u16);
    void init();
    void reset();


};

#endif // MEMMAP_H
