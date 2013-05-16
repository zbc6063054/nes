#include "threadutil.h"

#ifdef linux
#include <unistd.h>
static void* thread_run_linux(void *arg){
    int *arg_arr = (int *)arg;
    void *data =(void *) *(arg_arr+1);
    if(data == NULL){
        void ( *func)()=(void ( *)())(*arg_arr);
        func();
    }else{
        func_run func = (func_run)(*arg_arr);
        return func(data);
    }
}
#endif // linux

#ifdef WIN32
static DWORD WINAPI thread_run_win(void *arg){
    void **data = (void **)arg;
    func_run func = (func_run)data[0];
    void *func_arg = data[1];
    delete[] data;
    func(func_arg);
    return 0;
}
#endif

int createThread(thread_handle &handle, func_run func, void *data){
    int ret = 0;

#ifdef linux
    ret = pthread_create(&handle, NULL, func, data);
#endif // linux
#ifdef WIN32
    void **arg = new void*[2];
    arg[0] = (void *)func;
    arg[1] = data;
    handle = CreateThread(NULL, NULL, thread_run_win, (void *)arg, 0, NULL);
    if(handle == NULL){
        ret = GetLastError();
    }
#endif // WIN32

    return ret;
}

int threadSleep(int ms){
#ifdef linux
    usleep(ms * 1000);
#endif
#ifdef WIN32
    Sleep(ms);
#endif
    return 0;
}
