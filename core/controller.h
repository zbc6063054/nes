#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../global.h"

#define CTL1_CON    0x01
#define CTL2_CON    0x02

class Nes;

class Controller
{
public:
    enum Key{
        KEY_A = 0,
        KEY_B = 1,
        KEY_SELECT = 2,
        KEY_START = 3,
        KEY_UP = 4,
        KEY_DOWN = 5,
        KEY_LEFT = 6,
        KEY_RIGHT = 7
    };

public:
    Controller(Nes *parent);

private:
    Nes *nes;
    int readIndex;
    bool writeToggle;
    bool keyStates[8];
    bool ctl1_conn, ctl2_conn;

public:
    void init();
    void reset();

    void writeByte(u16 addr, u8 byte);
    u8 readByte(u16 addr);

    void setKeyState(Key key, bool state);

};

#endif // CONTROLLER_H
