#include "debug.h"
#include "log.h"
#include "cpu.h"
#include "threadutil.h"
#include <string.h>

struct {
    u16 break_points[MAX_BREAK_POINTS];
    u8 break_num;
    bool isBreaked;
    bool isMustBreak;
    void (* func)(void *);
    void *notifyData;
} debug_info;

static u16 lastPc = 0;

void debug::cpu_break(Cpu *cpu){
    if(debug_info.isMustBreak){
        debug_info.isMustBreak = false;
        do_breakcpu(cpu);
    }else{
        for(int i=0; i<debug_info.break_num; ++i){
            if(cpu->reg_pc == debug_info.break_points[i]){
                do_breakcpu(cpu);
            }
        }
    }
}

void debug::add_break(u16 addr){
    if(debug_info.break_num == MAX_BREAK_POINTS){
        LOGE("reach max break points!\n");
        return;
    }
    for(int i=0; i<debug_info.break_num; ++i){
        if(debug_info.break_points[i] == addr){
            LOGW("got a repeat break point %04X !\n", addr);
            return;
        }
    }
    debug_info.break_points[debug_info.break_num++] = addr;
}

void debug::remove_break(u16 addr){
    bool isFind = false;
    for(int i=0; i<debug_info.break_num; ++i){
        if(!isFind){
            if(addr == debug_info.break_points[i]){
                isFind = true;
            }
        }else{
            debug_info.break_points[i-1] = debug_info.break_points[i];
        }
    }
    if(isFind){
        debug_info.break_num--;
    }else{
        LOGE("can not find break points %04X !\n", addr);
    }
}

void debug::clear_break(){
    debug_info.break_num = 0;
}

void debug::do_breakcpu(Cpu *cpu){
    LOGI("cpu breaked at %04X !\n", cpu->reg_pc);
    debug_info.isBreaked = true;

    if(debug_info.func != NULL){
        debug_info.func(debug_info.notifyData);
    }
    while(debug_info.isBreaked){
        threadSleep(500);
    }
}

void debug::cpu_wakeup(){
    if(debug_info.isBreaked){
        debug_info.isBreaked = false;
    }
}

void debug::cpu_step(){
    if(debug_info.isBreaked){
        debug_info.isMustBreak = true;
        cpu_wakeup();
    }
}

void debug::set_notify_func(void (*func)(void *), void *data){
    debug_info.func = func;
    debug_info.notifyData = data;
}

void debug::break_cpu(){
    if(!debug_info.isBreaked && !debug_info.isMustBreak){
        debug_info.isMustBreak = true;
    }
}

int debug::get_breakarr(u16 arr[]){
    memcpy(arr, debug_info.break_points, debug_info.break_num*sizeof(u16));
    return debug_info.break_num;
}

bool debug::isBreaked(){
    return debug_info.isBreaked;
}

void debug::setLastPc(u16 addr){
    lastPc = addr;
}

u16 debug::getLastPc(){
    return lastPc;
}
