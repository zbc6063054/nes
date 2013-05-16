#pragma once

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef unsigned char BYTE;

#define ERROR   -1
#define ERROR_OK 0

#define BIT(b, x) (((b) & (1<<(x))) > 0)
