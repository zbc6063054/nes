#pragma once

#include "../global.h"

extern BYTE mem[];

enum AccessType{
	MEMREAD = 0,
	MEMWRITE = 1
};

typedef void (* MemAccessFunc)(u16 addr,
					u8& byte,AccessType type);

//range from [addr_start, addr_end]
typedef struct {
	u16 addr_start;
	u16 addr_end;
	MemAccessFunc access_func;
} StructMemMap;

u8 mem_read_byte(u16);
u16 mem_read_word(u16);

void mem_write_byte(u16, BYTE);
void mem_write_word(u16, u16);

void mem_dump(u16 addr);
void mem_dump(u16 addr_start, u16 addr_end);

