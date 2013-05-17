#ifndef EMUTHREAD_H
#define EMUTHREAD_H

#include "../global.h"
#include "threadutil.h"

class Nes;

class EmuThread
{
public:
    EmuThread(Nes *parent);

private:
    Nes *nes;
    volatile bool isRunning;
    bool isPause;
    thread_handle handle;
    void (* func_refresh)(void * data);
    void *func_data;

private:
    void run();
    static void thread_run(void *data);

public:
    void setRefreshFunc(void (* func)(void *data), void *data);
    void start();
    void stop();
    void pause();
    void resume();
    void reset();
    bool getRunState();
};

#endif // EMUTHREAD_H
