#ifndef THREADUTIL_H
#define THREADUTIL_H

#include "log.h"

typedef void* (* func_run)(void *data);

#ifdef linux

#include <sys/time.h>
#include <pthread.h>
typedef pthread_t thread_handle;
typedef timeval   timemark;

#endif // linux

#ifdef WIN32

#include <Windows.h>
#include <sys/timeb.h>
typedef _timeb   timemark;
typedef HANDLE   thread_handle;

#endif // WIN32

int createThread(thread_handle &handle, func_run func, void *data);
int threadSleep(int ms);

inline void getTimeMark(timemark *mark){
#ifdef linux
    gettimeofday(mark, NULL);
#endif
#ifdef WIN32
    _ftime_s(mark);
#endif
}

inline int diffTimeMark(timemark &start, timemark &stop){
#ifdef linux
    return (stop.tv_sec-start.tv_sec)*1000 + (stop.tv_usec-start.tv_usec)/1000;
#endif
#ifdef WIN32
    return (stop.time - start.time)*1000 + (stop.millitm - start.millitm);
#endif
}

inline void waitForThread(thread_handle handle){
#ifdef linux
	pthread_join(handle, NULL);
#endif
#ifdef WIN32
	if(WaitForSingleObject(handle, 1000) != WAIT_OBJECT_0){
		LOGE("Thread wait error!\n");
	}
#endif
}
#endif // THREADUTIL_H
