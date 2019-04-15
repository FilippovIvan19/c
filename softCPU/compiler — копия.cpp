/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <TXlib.h>
#include "comm.h"

const int MaxStrLen = 100;
const int Lab_count = 100;

#define $(x) std::cout<<#x<<" = "<<(x)<<"\n";*/


#define DEF_CMD(name) \
    else if (!strcmp(command, #name))\
        comp_##name(str, str_out, &code_num);

#define DEF_CMD_1(name) \
    else if (!strcmp(command, #name))\
        comp_##name(str_out, &code_num);

#define DEF_JMP_CMD(name) \
    else if (!strcmp(command, #name))\
        comp_##name(str, str_out, jmp_arr, &code_num);



//---------------------------------------------------------------------
//! convert rx to number of rx in string
//!
//! @param in  [rx]                   number of rx
//! @param out [str_out]              string with rx
//!
//! @returns string with rx
//---------------------------------------------------------------------
char* rx_to_num(char* rx, char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! compiles push
//!
//! @param in  [str]                  string of arguments
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_push(char* str, char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles pop
//!
//! @param in  [str]                  string of arguments
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_pop(char* str, char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles in
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_in(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles out
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_out(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles end
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_end(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles plus
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_plus(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles minus
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_minus(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles mul
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_mul(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles div
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_div(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles sqrt
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_sqrt(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles sin
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_sin(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles cos
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_cos(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles tg
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_tg(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles ctg
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_ctg(char str_out[MaxStrLen], int* code_num);

//---------------------------------------------------------------------
//! compiles ret
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_ret(char str_out[MaxStrLen]);

//---------------------------------------------------------------------
//! compiles call
//!
//! @param out [str_out]              compiled string
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_call(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num);

//---------------------------------------------------------------------
//! makes array of labels for jumps
//!
//! @param in  [str]                  string of arguments
//! @param out [str_out]              compiled string
//! @param out [jmp_arr]              array of labels for jumps
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_jmp_arr(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num);

//---------------------------------------------------------------------
//! compiles jmp
//!
//! @param in  [str]                  string of arguments
//! @param out [str_out]              compiled string
//! @param in  [jmp_arr]              array of labels for jumps
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_jmp(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num);

//---------------------------------------------------------------------
//! compiles ja
//!
//! @param in  [str]                  string of arguments
//! @param out [str_out]              compiled string
//! @param in  [jmp_arr]              array of labels for jumps
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_ja(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num);

//---------------------------------------------------------------------
//! compiles jae
//!
//! @param in  [str]                  string of arguments
//! @param out [str_out]              compiled string
//! @param in  [jmp_arr]              array of labels for jumps
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_jae(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num);

//---------------------------------------------------------------------
//! compiles jb
//!
//! @param in  [str]                  string of arguments
//! @param out [str_out]              compiled string
//! @param in  [jmp_arr]              array of labels for jumps
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_jb(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num);

//---------------------------------------------------------------------
//! compiles jbe
//!
//! @param in  [str]                  string of arguments
//! @param out [str_out]              compiled string
//! @param in  [jmp_arr]              array of labels for jumps
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_jbe(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num);

//---------------------------------------------------------------------
//! compiles je
//!
//! @param in  [str]                  string of arguments
//! @param out [str_out]              compiled string
//! @param in  [jmp_arr]              array of labels for jumps
//! @param in  [code_num]             number of code at this moment
//!
//! @returns compiled string
//---------------------------------------------------------------------
char* comp_je(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num);

//---------------------------------------------------------------------
//! compiles file
//!
//! @param in [in_name]              name of file from what compilation is going
//! @param in [out_name]             name of file into what compilation is going
//! @param in [jmp_arr]              array of labels for jumps
//!
//! @returns 0
//---------------------------------------------------------------------
int compile(char* in_name, char* out_name, int jmp_arr[Lab_count]);




char* rx_to_num(char* rx, char str_out[MaxStrLen])
{

    if      (!strcmp(rx, "rax"))
        strcpy(str_out, "1");

    else if (!strcmp(rx, "rbx"))
        strcpy(str_out, "2");

    else if (!strcmp(rx, "rcx"))
        strcpy(str_out, "3");

    else if (!strcmp(rx, "rdx"))
        strcpy(str_out, "4");

    return str_out;
}


char* comp_push(char* str, char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", push_c);

    double val;
    char val_s[MaxStrLen] = "";
    char* str_5 = str + 5;

    if (sscanf(str_5, "%lf", &val) == 1)
    {
        sscanf(str_5, "%s", val_s);
        strcat(str_out, " 1 ");
        strcat(str_out, val_s);
    }
    else if (str_5[0] != '[')
    {
        strcat(str_out, " 2 ");
        str_5[strlen(str_5) - 1] = '\0';

        char rx_str[MaxStrLen] = "";
        rx_to_num(str_5, rx_str);
        strcat(str_out, rx_str);
    }
    else
    {
        if (sscanf(str_5 + 1, "%lf", &val) == 1)
        {
            sscanf(str_5 + 1, "%s", val_s);
            val_s[strlen(val_s) - 1] = '\0';
            strcat(str_out, " 3 ");
            strcat(str_out, val_s);
        }
        else
        {
            sscanf(str_5 + 1, "%s", val_s);
            val_s[strlen(val_s) - 1] = '\0';

            strcat(str_out, " 4 ");
            char rx_str[MaxStrLen] = "";
            rx_to_num(str_5, rx_str);
            strcat(str_out, rx_str);
        }

    }
    (*code_num) += 3;
    return str_out;
}

char* comp_in(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", in_c);
    (*code_num)++;
    return str_out;
}

char* comp_pop(char* str, char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", pop_c);

    double val;
    char val_s[MaxStrLen] = "";
    char* str_4 = str + 4;


    if (str_4[0] != '[')
    {
        strcat(str_out, " 2 ");
        str_4[strlen(str_4) - 1] = '\0';

        char rx_str[MaxStrLen] = "";
        rx_to_num(str_4, rx_str);
        strcat(str_out, rx_str);
    }
    else
    {
        if (sscanf(str_4 + 1, "%lf", &val) == 1)
        {
            sscanf(str_4 + 1, "%s", val_s);
            val_s[strlen(val_s) - 1] = '\0';
            strcat(str_out, " 3 ");
            strcat(str_out, val_s);
        }
        else
        {
            sscanf(str_4 + 1, "%s", val_s);
            val_s[strlen(val_s) - 1] = '\0';

            strcat(str_out, " 4 ");
            char rx_str[MaxStrLen] = "";
            rx_to_num(str_4, rx_str);
            strcat(str_out, rx_str);
        }

    }
    (*code_num) += 3;
    return str_out;
}

char* comp_out(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", out_c);
    (*code_num)++;
    return str_out;
}

char* comp_end(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", end_c);
    (*code_num)++;
    return str_out;
}

char* comp_plus(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", plus_c);
    (*code_num)++;
    return str_out;
}

char* comp_minus(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", minus_c);
    (*code_num)++;
    return str_out;
}

char* comp_mul(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", mul_c);
    (*code_num)++;
    return str_out;
}

char* comp_div(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", div_c);
    (*code_num)++;
    return str_out;
}

char* comp_sqrt(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", sqrt_c);
    (*code_num)++;
    return str_out;
}

char* comp_sin(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", sin_c);
    (*code_num)++;
    return str_out;
}

char* comp_cos(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", cos_c);
    (*code_num)++;
    return str_out;
}

char* comp_tg(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", tg_c);
    (*code_num)++;
    return str_out;
}

char* comp_ctg(char str_out[MaxStrLen], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", ctg_c);
    (*code_num)++;
    return str_out;
}

char* comp_jmp(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d%c", jmp_c, ' ');
    char* str_5 = str + 5;

    int adress;
    sscanf(str_5, "%d", &adress);

    adress = jmp_arr[adress];
    sprintf(str_out + 3, "%d", adress);

    $(str_out)

    (*code_num) += 2;
    return str_out;
}

char* comp_ja(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d%c", ja_c, ' ');
    char* str_4 = str + 4;

    int adress;
    sscanf(str_4, "%d", &adress);

    adress = jmp_arr[adress];
    sprintf(str_out + 3, "%d", adress);

    (*code_num) += 2;
    return str_out;
}

char* comp_jae(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d%c", jae_c, ' ');
    char* str_5 = str + 5;

    int adress;
    sscanf(str_5, "%d", &adress);

    adress = jmp_arr[adress];
    sprintf(str_out + 3, "%d", adress);

    (*code_num) += 2;
    return str_out;
}

char* comp_jb(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d%c", jb_c, ' ');
    char* str_4 = str + 4;

    int adress;
    sscanf(str_4, "%d", &adress);

    adress = jmp_arr[adress];
    sprintf(str_out + 3, "%d", adress);

    (*code_num) += 2;
    return str_out;
}

char* comp_jbe(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d%c", jbe_c, ' ');
    char* str_5 = str + 5;

    int adress;
    sscanf(str_5, "%d", &adress);

    adress = jmp_arr[adress];
    sprintf(str_out + 3, "%d", adress);

    (*code_num) += 2;
    return str_out;
}

char* comp_je(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d%c", je_c, ' ');
    char* str_4 = str + 4;

    int adress;
    sscanf(str_4, "%d", &adress);

    adress = jmp_arr[adress];
    sprintf(str_out + 3, "%d", adress);

    (*code_num) += 2;
    return str_out;
}

char* comp_call(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num)
{
    strcpy(str_out, "");
    sprintf(str_out, "%d%c", call_c, ' ');
    char* str_6 = str + 6;

    int adress;
    sscanf(str_6, "%d", &adress);

    adress = jmp_arr[adress];
    sprintf(str_out + 3, "%d", adress);

    (*code_num) += 2;
    return str_out;
}

char* comp_jmp_arr(char* str, char str_out[MaxStrLen], int jmp_arr[Lab_count], int* code_num)
{
    strcpy(str_out, "");
    char* str_1 = str + 1;

    int adress;
    sscanf(str_1, "%d", &adress);
    jmp_arr[adress] = *code_num;

    return str_out;
}

char* comp_ret(char str_out[MaxStrLen])
{
    strcpy(str_out, "");
    sprintf(str_out, "%d", ret_c);

    return str_out;
}


int compile(char* in_name, char* out_name, int jmp_arr[Lab_count])
{
    FILE* in_file = fopen(in_name, "r");
    FILE* out_file = fopen(out_name, "w");

    char str[MaxStrLen] = "";
    char str_out[MaxStrLen] = "";
    char command[MaxStrLen] = "";
    int code_num = 0;

    while (fgets(str, MaxStrLen, in_file) != NULL)
    {
        sscanf(str, "%s", command);

        if (!strcmp(str, "\n"))
            continue;

        if (command[0] == ':')
            comp_jmp_arr(str, str_out, jmp_arr, &code_num);

        DEF_JMP_CMD(jmp)//"else if" is here
        DEF_JMP_CMD(call)
        DEF_JMP_CMD(ja)
        DEF_JMP_CMD(jae)
        DEF_JMP_CMD(jb)
        DEF_JMP_CMD(jbe)
        DEF_JMP_CMD(je)

        DEF_CMD(push)
        DEF_CMD(pop)
        DEF_CMD_1(in)
        DEF_CMD_1(out)
        DEF_CMD_1(end)
        DEF_CMD_1(plus)
        DEF_CMD_1(minus)
        DEF_CMD_1(mul)
        DEF_CMD_1(div)
        DEF_CMD_1(sqrt)
        DEF_CMD_1(sin)
        DEF_CMD_1(cos)
        DEF_CMD_1(tg)
        DEF_CMD_1(ctg)

        else if (!strcmp(command, "ret"))
            comp_ret(str_out);


        else assert(0);


        if (command[0] != ':')
            fprintf(out_file, "%s ", str_out);
    }


    fclose(in_file);
    fclose(out_file);
    return 0;
}
#undef DEF_CMD
#undef DEF_CMD_1
#undef DEF_JMP_CMD

/*int main()
{
    int jmp_arr[Lab_count];
    for(int i = 0; i < Lab_count; i++)
        jmp_arr[i] = -1;

    compile("2.txt", "comp.txt", jmp_arr);
    //$(jmp_arr[1]);
    compile("2.txt", "comp.txt", jmp_arr);



    return 0;
}*/

