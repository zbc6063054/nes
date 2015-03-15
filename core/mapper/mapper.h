#ifndef MAPPER_H
#define MAPPER_H
#include "../../global.h"
#include "../rom.h"

class Nes;
class Mapper
{
protected:
    enum Bank{
        BANK_LOW = 0,
        BANK_HIGH = 1,
        BANK_ALL = 2
    };
public:
    Mapper(Nes *parent);
    virtual ~Mapper();
    void setFileHeader(const FileHeader& header){
        this->header = header;
    }
    inline u8* getBankAddr(int id, int banksize, u8* startAddr){
        return startAddr + id * banksize;
    }
    void setChrBank(Bank bank, int bankNum);
    void setPrgBank(Bank bank, int bankNum);

    virtual void reset() = 0;
    virtual void writeByte(u16 addr, u8 byte) = 0;

    static Mapper* getMapper(Nes *nes, int code);

protected:
    Nes *nes;
    FileHeader header;
};

#endif // MAPPER_H
