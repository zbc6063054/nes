#ifndef DEBUG_H
#define DEBUG_H

#include "../global.h"

#define DEBUG_CPU_OPSTART
#define DEBUG_CPU_OPEND         debug::cpu_break(this);

#define MAX_BREAK_POINTS        20

class Cpu;
namespace debug{
    void cpu_break(Cpu *cpu);
    void add_break(u16 addr);
    void remove_break(u16 addr);
    void clear_break();
    void do_breakcpu(Cpu *cpu);
    void cpu_wakeup();
    void cpu_step();
    void set_notify_func(void (* func)(void *), void *data);
    void break_cpu();
    int get_breakarr(u16 arr[]);
    bool isBreaked();
}


#endif // DEBUG_H
