#ifndef MAPPER2_H
#define MAPPER2_H

#include "mapper.h"
class Nes;
class Mapper2 : public Mapper
{
public:
    Mapper2(Nes *parent);
    virtual ~Mapper2();

    virtual void reset();
    virtual void writeByte(u16 addr, u8 byte);
};

#endif // MAPPER2_H
