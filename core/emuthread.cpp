#include "emuthread.h"
#include "nes.h"

#define TIMEPERFRAME 17
#define TIMEPAUSE    400

void EmuThread::thread_run(void *data){
    ((EmuThread *)data)->run();
}

EmuThread::EmuThread(Nes *parent)
    :nes(parent)
{
    isRunning = false;
    isPause = false;
    handle = NULL;
    func_data = NULL;
    func_refresh = NULL;
}

void EmuThread::start(){
    isRunning = true;
    if(handle == NULL){
        if(createThread(handle, (func_run)thread_run, (void *)this)){
            LOGE("create thread faliled!\n");
            isRunning = false;
        }
    }
}

void EmuThread::stop(){
    if(isRunning == true){
        isRunning = false;
        if(handle != NULL){
            waitForThread(handle);
            handle = NULL;
        }
    }
}

void EmuThread::run(){
    LOGI("Emulation Start!\n");
    while(isRunning){
        if(!isPause){
            timemark start, stop;
            getTimeMark(&start);

            nes->exeFrame();
            if(func_refresh != NULL){
                func_refresh(func_data);
            }

            getTimeMark(&stop);
            int ms = diffTimeMark(start, stop);
            if(ms < 0){
                LOGE("thread time error!\n");
                this->stop();
            }
            if(TIMEPERFRAME > ms)
                threadSleep(TIMEPERFRAME - ms);
        }else{
            //pause
            threadSleep(TIMEPAUSE);
        }
    }
    LOGI("Emulation Stoped!\n");
#ifdef WIN32
	CloseHandle(handle);
#endif
}

void EmuThread::pause(){
    if(!isPause){
        isPause = true;
    }
}

void EmuThread::setRefreshFunc(void (*func)(void *), void *data){
    func_refresh = func;
    func_data = data;
}

void EmuThread::resume(){
    if(isPause){
        isPause = false;
    }
}

bool EmuThread::getRunState(){
    return !isPause;
}

void EmuThread::reset(){
    stop();
    isRunning = false;
    isPause = false;
}
