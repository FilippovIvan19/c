/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <TXlib.h>
#include "comm.h"

const int MaxStrLen = 100;*/


#define DEF_CMD(name) \
    case name##_c:\
        uncomp_##name(in_file, str_out);\
        break;

#define DEF_CMD_1(name) \
    case name##_c:\
        uncomp_##name(str_out);\
        break;





//---------------------------------------------------------------------
//! convert number of rx to rx
//!
//! @param in  [number]               number of rx
//! @param out [str_out]              string with rx
//!
//! @returns string with rx
//---------------------------------------------------------------------
char* num_to_rx(int number, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles plus
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_plus(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles minus
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_minus(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles mul
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_mul(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles div
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_div(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles sqrt
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_sqrt(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles sin
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_sin(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles cos
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_cos(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles tg
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_tg(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles ctg
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_ctg(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles end
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_end(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles in
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_in(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles out
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_out(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles ret
//!
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_ret(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles pop
//!
//! @param in  [in_file]              name of file from what uncompilation is going
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_pop(FILE* in_file, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles push
//!
//! @param in  [in_file]              name of file from what uncompilation is going
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_push(FILE* in_file, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles call
//!
//! @param in  [in_file]              name of file from what uncompilation is going
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_call(FILE* in_file, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles jmp
//!
//! @param in  [in_file]              name of file from what uncompilation is going
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_jmp(FILE* in_file, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles ja
//!
//! @param in  [in_file]              name of file from what uncompilation is going
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_ja(FILE* in_file, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles jae
//!
//! @param in  [in_file]              name of file from what uncompilation is going
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_jae(FILE* in_file, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles jb
//!
//! @param in  [in_file]              name of file from what uncompilation is going
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_jb(FILE* in_file, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles jbe
//!
//! @param in  [in_file]              name of file from what uncompilation is going
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_jbe(FILE* in_file, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles je
//!
//! @param in  [in_file]              name of file from what uncompilation is going
//! @param out [str_out]              uncompiled string
//!
//! @returns uncompiled string
//---------------------------------------------------------------------
char* uncomp_je(FILE* in_file, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! uncompiles file
//!
//! @param in [in_name]              name of file from what uncompilation is going
//! @param in [out_name]             name of file into what uncompilation is going
//!
//! @returns 0
//---------------------------------------------------------------------
int uncompile(char* in_name, char* out_name);




char* num_to_rx(int number, char str_out[MaxStrLen])
{

    switch (number)
        {
            case 1:
                strcpy(str_out, "rax");
                break;
            case 2:
                strcpy(str_out, "rbx");
                break;
            case 3:
                strcpy(str_out, "rcx");
                break;
            case 4:
                strcpy(str_out, "rdx");
                break;
            default: assert(0);
        }
    return str_out;
}


char* uncomp_plus(char str_out[MaxStrLen])
{
    strcpy(str_out, "plus");
    return str_out;
}

char* uncomp_minus(char str_out[MaxStrLen])
{
    strcpy(str_out, "minus");
    return str_out;
}

char* uncomp_mul(char str_out[MaxStrLen])
{
    strcpy(str_out, "mul");
    return str_out;
}

char* uncomp_div(char str_out[MaxStrLen])
{
    strcpy(str_out, "div");
    return str_out;
}

char* uncomp_sin(char str_out[MaxStrLen])
{
    strcpy(str_out, "sin");
    return str_out;
}

char* uncomp_cos(char str_out[MaxStrLen])
{
    strcpy(str_out, "cos");
    return str_out;
}

char* uncomp_tg(char str_out[MaxStrLen])
{
    strcpy(str_out, "tg");
    return str_out;
}

char* uncomp_ctg(char str_out[MaxStrLen])
{
    strcpy(str_out, "ctg");
    return str_out;
}

char* uncomp_sqrt(char str_out[MaxStrLen])
{
    strcpy(str_out, "sqrt");
    return str_out;
}

char* uncomp_end(char str_out[MaxStrLen])
{
    strcpy(str_out, "end");
    return str_out;
}

char* uncomp_out(char str_out[MaxStrLen])
{
    strcpy(str_out, "out");
    return str_out;
}

char* uncomp_in(char str_out[MaxStrLen])
{
    strcpy(str_out, "in");
    return str_out;
}

char* uncomp_ret(char str_out[MaxStrLen])
{
    strcpy(str_out, "ret");
    return str_out;
}

char* uncomp_pop(FILE* in_file, char str_out[MaxStrLen])
{

    strcpy(str_out, "pop ");

    int pop_type;
    fscanf(in_file, "%d", &pop_type);

    char val_s[MaxStrLen];
    fscanf(in_file, "%s", val_s);

    int val;
    sscanf(val_s, "%d", &val);

    char rx_str[MaxStrLen] = "";

    switch (pop_type)
    {
        case 2:
            num_to_rx(val, rx_str);
            strcat(str_out, rx_str);
            break;

        case 3:
            strcat(str_out, "[");
            strcat(str_out, val_s);
            strcat(str_out, "]");
            break;

        case 4:
            strcat(str_out, "[");
            num_to_rx(val, rx_str);
            strcat(str_out, rx_str);
            strcat(str_out, "]");
            break;

        default: assert(0);
    }

    return str_out;
}

char* uncomp_push(FILE* in_file, char str_out[MaxStrLen])
{
    strcpy(str_out, "push ");

    int push_type;
    fscanf(in_file, "%d", &push_type);

    char val_s[MaxStrLen];
    fscanf(in_file, "%s", val_s);

    int val;
    sscanf(val_s, "%d", &val);

    char rx_str[MaxStrLen] = "";

    switch (push_type)
    {
        case 1:
            strcat(str_out, val_s);
            break;

        case 2:
            num_to_rx(val, rx_str);
            strcat(str_out, rx_str);
            break;

        case 3:
            strcat(str_out, "[");
            strcat(str_out, val_s);
            strcat(str_out, "]");
            break;

        case 4:
            strcat(str_out, "[");
            num_to_rx(val, rx_str);
            strcat(str_out, rx_str);
            strcat(str_out, "]");
            break;

        default: assert(0);
    }

    return str_out;
}

char* uncomp_jmp(FILE* in_file, char str_out[MaxStrLen])
{
    strcpy(str_out, "jmp ");

    int adress;
    fscanf(in_file, "%d", &adress);

    sprintf(str_out + 4, "%d", adress);
    return str_out;
}

char* uncomp_jae(FILE* in_file, char str_out[MaxStrLen])
{
    strcpy(str_out, "jae ");

    int adress;
    fscanf(in_file, "%d", &adress);

    sprintf(str_out + 4, "%d", adress);
    return str_out;
}

char* uncomp_jbe(FILE* in_file, char str_out[MaxStrLen])
{
    strcpy(str_out, "jbe ");

    int adress;
    fscanf(in_file, "%d", &adress);

    sprintf(str_out + 4, "%d", adress);
    return str_out;
}

char* uncomp_ja(FILE* in_file, char str_out[MaxStrLen])
{
    strcpy(str_out, "ja ");

    int adress;
    fscanf(in_file, "%d", &adress);

    sprintf(str_out + 3, "%d", adress);
    return str_out;
}

char* uncomp_jb(FILE* in_file, char str_out[MaxStrLen])
{
    strcpy(str_out, "jb ");

    int adress;
    fscanf(in_file, "%d", &adress);

    sprintf(str_out + 3, "%d", adress);
    return str_out;
}

char* uncomp_je(FILE* in_file, char str_out[MaxStrLen])
{
    strcpy(str_out, "je ");

    int adress;
    fscanf(in_file, "%d", &adress);

    sprintf(str_out + 3, "%d", adress);
    return str_out;
}

char* uncomp_call(FILE* in_file, char str_out[MaxStrLen])
{
    strcpy(str_out, "call ");

    int adress;
    fscanf(in_file, "%d", &adress);

    sprintf(str_out + 5, "%d", adress);
    return str_out;
}


int uncompile(char* in_name, char* out_name)
{
    FILE* in_file = fopen(in_name, "r");
    FILE* out_file = fopen(out_name, "w");

    char str_out[MaxStrLen] = "";
    int command;
    double d_cmd;

    while (fscanf(in_file, "%lf", &d_cmd) == 1)
    {

        command = (int)(d_cmd + 0.1);
        switch (command)
        {


            DEF_CMD(push)//"case" is here
            DEF_CMD(pop)
            DEF_CMD(jmp)
            DEF_CMD(ja)
            DEF_CMD(jb)
            DEF_CMD(jae)
            DEF_CMD(jbe)
            DEF_CMD(je)
            DEF_CMD(call)

            DEF_CMD_1(in)
            DEF_CMD_1(out)
            DEF_CMD_1(end)
            DEF_CMD_1(plus)
            DEF_CMD_1(minus)
            DEF_CMD_1(mul)
            DEF_CMD_1(div)
            DEF_CMD_1(sin)
            DEF_CMD_1(cos)
            DEF_CMD_1(tg)
            DEF_CMD_1(ctg)
            DEF_CMD_1(sqrt)
            DEF_CMD_1(ret)

            default: assert(0);
        }


        fprintf(out_file, "%s\n", str_out);
    }


    fclose(in_file);
    fclose(out_file);
    return 0;
}

#undef DEF_CMD
#undef DEF_CMD_1

/*int main()
{

    uncompile("comp.txt", "uncomp.txt");

    return 0;
}*/
