
#include <stdio.h>
#include <string.h>

#include "../global.h"
#include "memory.h"
#include "cpu.h"
#include "nes.h"
#include "debug.h"
#include "log.h"

#define CMD_SIZE 20

typedef void  (* cmd_func)(char  argv[][CMD_SIZE], int);

extern Nes nes;

void mem_test(){

}

void stack_test(){


}

void file_test(){

}

void op_test(){
    nes.cpu->reg_flag = 0;
    nes.cpu->setFlag(FLAG_N, 1);
    ASSERT(nes.cpu->getFlag(FLAG_V) == 0);
    ASSERT(nes.cpu->getFlag(FLAG_N) == 1);

    nes.cpu->reg_a = 10;
    nes.cpu->setFlag(FLAG_C, 1);
    nes.cpu->op_SBC(2);
    LOGI("%d\n", nes.cpu->reg_a);

    nes.cpu->reg_a = 0x11;
    nes.cpu->setFlag(FLAG_C, 1);
    nes.cpu->reg_a = nes.cpu->op_ROR(nes.cpu->reg_a);
    LOGI("%d\n", nes.cpu->reg_a);
}

int test(){
    printf("test start!\n");
    op_test();
    mem_test();
    stack_test();
    file_test();
    return 0;
}

char * str_trim(char* str){
    while(*str == ' ')
        ++ str;
    return str;
}

int dbg_get_input(char str[][CMD_SIZE],int len){
    char input[80];
    int i = 0;
    fgets(input, 80, stdin);
    input[strlen(input)-1]='\0';					//remove the last '\n'
    char * temp = str_trim(input);
    while(sscanf(temp, "%[^ ]", &str[i][0]) > 0){
        temp += strlen(str[i]);
        ++i;
        temp = str_trim(temp);
        if(*temp == '\0') break;
    }
    return i;
}

void dbg_op_input(char argv[][CMD_SIZE], int len){
    char input[80];
    char opcode[20][CMD_SIZE];
    u32 opcode_hex[20];
    u16 addr = 0;
    while(true){
        printf("--");
        //get a line
        int len = dbg_get_input(opcode, 20);
        //if input nothing,stop loop
        if(len == 0) break;
        //
        int i = 0;
        for(; i<len; ++i){
            if(!(sscanf(opcode[i], "%x", &opcode_hex[i]) > 0)){
                printf("%s is not a number!\n", opcode[i]);
                break;
            }
            if(opcode_hex[i] > 0xff){
                printf("%s is out of range 00~ff!\n", opcode[i]);
                break;
            }
        }
        //error,do not write to memory
        if(i < len) continue;
        //wirte to memory
        for(int j=0; j<i; ++j){
//			mem_write_byte(addr, (u8)opcode_hex[j]);
            addr += 1;
        }
    }
}

void dbg_dump_mem(char argv[][CMD_SIZE], int len){
    if(len >= 4){
        printf("error:except 2 arguments,but receive %d arguments!\n", len-1);
        return;
    }
    u32 addr[2] = { 0, 0 };
    for(int i=1; i<len; i++){
        if(!(sscanf(argv[i], "%x", &addr[i-1]) > 0)){
            printf("%s is not a number!\n", argv[i]);
            return;
        }
    }
    if(len <= 2){
        addr[1] = addr[0] + 0x40;
    }
 //   mem_dump((u16)addr[0], (u16)addr[1]);
}

void dbg_read_state(char argv[][CMD_SIZE], int len){
    //cpu_dump();

}

void dbg_next_code(char argv[][CMD_SIZE], int len){
}

void dbg_open_file(char argv[][CMD_SIZE], int len){
    if(len == 1){
        printf("error:please specify file name\n");
        return;
    }
    int res = 0;
//	res = file_load(argv[1]);
    if(res != 0){
        printf("error:load file \"%s\" error!\n", argv[1]);
        return;
    }
}

struct struct_cmd{
    char argc[10];
    cmd_func func;
} cmd_arr[]= {
    "a", dbg_op_input,
    "d", dbg_dump_mem,
    "r", dbg_read_state,
    "t", dbg_next_code,
    "f", dbg_open_file
};

int debug2(){
    while(true){
        printf("-");
        char cmd[10][20];
        int len = dbg_get_input(cmd, 10);
        if(len > 0){
            if(!strcmp(cmd[0], "q")){				//quit
                break;
            }
            else{
                int i = 0;
                for(i=0; i < sizeof(cmd_arr)/sizeof(cmd_arr[0]); ++i){
                    if(!strcmp(cmd[0], cmd_arr[i].argc)){
                        cmd_arr[i].func(cmd, len);
                        break;
                    }
                }
                if(i == sizeof(cmd_arr)/sizeof(cmd_arr[0]))
                    printf("unknow command:%s\n", cmd[0]);
            }
        }
    }
    //printf("-");
    return 0;
}

int main1(){
//	test();
//    debug();
    return 0;
}
