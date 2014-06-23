#pragma once

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char BYTE;

#define NULL 0
#define NES_ERROR   -1
#define NES_ERROR_OK 0
#define NES_ERROR_OPENFILE 1
#define NES_ERROR_FILEFORMAT 2
#define NES_ERROR_UNSUPPORTMAPPER 3
#define NES_ERROR_LOADFILE 4

#define BIT(b, x) (((b) & (1<<(x))) > 0)

const char* GetErrorString(int error_code);
