#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include "assert.h"

#ifdef linux
#define LOG(type, arg...)    log(type, arg)
#define LOGI(arg...)         LOG(LOG_INFO, arg)
#define LOGW(arg...)         LOG(LOG_WARN, arg)
#define LOGE(arg...)         LOG(LOG_ERROR, arg)
#define LOG_ON(b, arg...)	 if(b) { LOGW(arg) ;}
#define ASSERT(arg...)       assert(arg)
#endif //linux

#ifdef WIN32
#define LOG(type, ...)		log(type, __VA_ARGS__)
#define LOGI(...)			LOG(LOG_INFO, __VA_ARGS__)
#define LOGW(...)			LOG(LOG_WARN, __VA_ARGS__)
#define LOGE(...)			LOG(LOG_ERROR, __VA_ARGS__)
#define LOG_ON(b, ...)		if(b) { LOGW(__VA_ARGS__) ;}
#define ASSERT(...)			assert(__VA_ARGS__)
#endif



enum LogType{
    LOG_INFO = 0,
    LOG_WARN = 1,
    LOG_ERROR = 2
};

void log(LogType type, const char *format, ...);




#endif // LOG_H
