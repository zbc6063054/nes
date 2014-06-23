#include <stdio.h>
#include "log.h"
#include <stdarg.h>

#define LOG_FILE "log.txt" 

void log(LogType type, const char *format, ...){
	va_list list;
	va_start(list, format);
#if _DEBUG
	vprintf(format, list);
#endif
	FILE *fp = fopen(LOG_FILE, "a+");
	vfprintf(fp, format, list);
	fclose(fp);
	va_end(list);
}
