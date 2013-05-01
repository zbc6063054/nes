#pragma once

enum MirrorType{
	HORIZONTAL_MIRROR = 0,
	VERTICAL_MIRROR = 1
};

typedef struct struct_file_header{
	int rom_prg_num;
	int rom_chr_num;
	MirrorType mirror_type;
	bool battary_backed;
	bool have_trainer;
	bool four_screen_layout;
	int mapper_type;

} file_header;

int file_get_head(FILE* fp, file_header& header);
int file_load(char* filename);
void file_load_prg(FILE* fp, const file_header& header);

