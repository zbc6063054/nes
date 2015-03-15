#ifndef MAPPER4_H
#define MAPPER4_H

#include "mapper.h"

class Mapper4 : public Mapper
{
private:
    u8 reg_crtl;
    u8 reg_cmd[8];

    void setupChrRom();
    void setupPrgRom();

public:
    Mapper4(Nes *parent);

    virtual void reset();
    virtual void writeByte(u16 addr, u8 byte);
};

#endif // MAPPER4_H
