#ifndef MAPPER1_H
#define MAPPER1_H

#include "mapper.h"
#include "../../global.h"

#define REG0_VHMIRROR 0x01
#define REG0_ONERSCREENMIRROR 0x02
#define REG0_PRGROMPOS8000 0x04
#define REG0_PRGROM16KSIZE 0x08
#define REG0_VROM4KSIZE 0x10

#define FLAG_RESET 0x80

class Nes;
class Mapper1 : public Mapper
{
private:
    u8 reg0;
    u8 buf;
    u8 offset;
    u8 reg1;
    u8 reg2;

public:
    Mapper1(Nes *parent);
    virtual ~Mapper1();

    virtual void reset();
    virtual void writeByte(u16 addr, u8 byte);
};

#endif // MAPPER1_H
