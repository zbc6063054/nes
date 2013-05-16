#include "controller.h"
#include <string.h>
#include "log.h"
#include "debug.h"

Controller::Controller(Nes *parent)
    :nes(parent)
{
    readIndex = 0;
    writeToggle = false;
    ctl1_conn = true;
    ctl2_conn = false;
    memset(keyStates, 0, sizeof(keyStates));
}

void Controller::init(){
    reset();
}

void Controller::reset(){
    readIndex = 0;
    writeToggle = false;
    memset(keyStates, 0, sizeof(keyStates));
}

u8 Controller::readByte(u16 addr){
    u8 byte = 0x40;
    switch(addr){
    case 0x4016:                            //first controller
        if(readIndex < 8){
            byte = keyStates[readIndex] | 0x40; //(ctl1_conn ? 2 : 0);
        }else if((readIndex >= 8)&&(readIndex < 16)){
            byte = 0x40;
        }else if((readIndex >= 16)&&(readIndex < 20)){
            byte = 0x40;
            if(readIndex == 19){
                byte = 0x40;
            }
        }else if(readIndex < 24){
            byte = 0x40;
        }else{
            LOGW("control read index %d overflow!\n", readIndex);
        }
        readIndex = (++readIndex)%24;
        break;
    case 0x4017:

        break;
    default:
        LOGW("read nonexists controller %04X !\n", addr);
        break;
    }
    return byte;
}

void Controller::writeByte(u16 addr, u8 byte){
    switch(addr){
    case 0x4016:                            //first controller
        if((byte == 1) && (!writeToggle)){
            writeToggle = true;
        }else if((byte == 0) && (writeToggle)){
            readIndex = 0;
            writeToggle = false;
        }else{
 //           LOGW("control recieved unrecognize byte %02X !\n", byte);
            writeToggle = false;
        }
        break;
    case 0x4017:

        break;
    default:
        LOGW("write nonexists controller %04X !\n", addr);
        break;
    }
}

void Controller::setKeyState(Key key, bool state){
    if((int)key > 8){
        LOGW("can not find key %d \n!", key);
    }
    keyStates[key] = state;
}
