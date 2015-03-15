#include "global.h"

static struct {
	int error_code;
	const char *error_str;
} error_string[] = {
	NES_ERROR,					"unknow error!",
	NES_ERROR_OK,				"",
	NES_ERROR_OPENFILE,			"failed to open file",
	NES_ERROR_FILEFORMAT,		"file format error!",
	NES_ERROR_UNSUPPORTMAPPER,	"unsupport mapper type!",
	NES_ERROR_LOADFILE,			"load file error!"
};

const char* GetErrorString(int error_code){
	int size = sizeof(error_string) / sizeof(error_string[0]);
	for (int i=0; i<size; ++i)
	{
		if(error_string[i].error_code == error_code){
			return error_string[i].error_str;
		}
	}
	return NULL;
}
