#ifndef MAPPER0_H
#define MAPPER0_H
#include "mapper.h"

class Mapper0:public Mapper
{
public:
    Mapper0(Nes *parent);
    virtual ~Mapper0();

    virtual void reset();
    virtual void writeByte(u16 addr, u8 byte);
};

#endif // MAPPER0_H
