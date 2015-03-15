#include <stdio.h>
#include <time.h>
#include "log.h"
#include <stdarg.h>

#define _DEBUG 1

#define LOG_FILE "log.txt" 


void log(LogType type, const char *format, ...){
    time_t t;
    time(&t);
    struct tm *tp;
    tp = localtime(&t);

    va_list list;
    va_start(list, format);

#if _DEBUG
    printf("%d/%d/%d %d:%d:%d: ", 1900 + tp->tm_year, 1 + tp->tm_mon, tp->tm_mday,
           tp->tm_hour, tp->tm_min, tp->tm_sec);
    vprintf(format, list);
#endif

    FILE *fp = fopen(LOG_FILE, "a+");
    fprintf(fp, "%d/%d/%d %d:%d:%d: ", 1900 + tp->tm_year, 1 + tp->tm_mon, tp->tm_mday,
           tp->tm_hour, tp->tm_min, tp->tm_sec);
    vfprintf(fp, format, list);
    fclose(fp);
    va_end(list);
}
