#pragma once

extern BYTE mem[];

u8 mem_read_byte(u16);
u16 mem_read_word(u16);

void mem_write_byte(u16 , BYTE);
void mem_write_word(u16 , u16);

void mem_dump(u16 addr);

