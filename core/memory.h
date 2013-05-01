#pragma once

#include "../global.h"

enum AccessType{
	MEMREAD = 0,
	MEMWRITE = 1
};

enum NtMirrorType{
	MIRROR_HORIZONTAL = 0,
	MIRROR_VERTICAL = 1,
	MIRROR_SINGLE = 2,
	MIRROR_FOUR = 3,
};

enum MemMapType{
	MEMMAP_FUNCTION = 0,
	MEMMAP_ADDRESS = 1,
};

typedef void (* MemAccessFunc)(u16 addr,
					u8& byte,AccessType type);

//range from [addr_start, addr_end]
typedef struct {
	u16 addr_start;
	u16 addr_end;
	MemMapType type;
	void* data;
} StructMemMap;

extern u8 cpu_mem[];
extern u8 save_ram[];
extern u8 prg_rom[];
extern u8 ex_rom[];

extern u8 chr_rom[];
extern u8 nt_ram[];				//name table
extern u8 spr_mem[];				//256 bytes sprite memory
extern u8 spr_pattle[];
extern u8 bg_pattle[];				//

extern u8* ppu_mem[];				//1k block

u8 mem_read_byte(u16);
u16 mem_read_word(u16);

void set_nt_mirror(NtMirrorType type);

void mem_write_byte(u16, BYTE);
void mem_write_word(u16, u16);

void mem_dump(u16 addr);
void mem_dump(u16 addr_start, u16 addr_end);

