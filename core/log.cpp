#include <stdio.h>
#include "log.h"
#include <stdarg.h>

#define LOG_FILE "log.txt" 

void log(LogType type, const char *format, ...){
	va_list list;
	va_start(list, format);
#ifdef _DEBUG
	vprintf(format, list);
#else // Release
	FILE *fp = fopen(LOG_FILE, "a+");
	vfprintf(fp, format, list);
	fclose(fp);
#endif // _DEBUG
	va_end(list);
}
