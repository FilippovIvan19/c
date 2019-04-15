/// @file onegin.cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <assert.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys\stat.h>

const int LcAll = 0;
const int MaxStr = 1000;

//------------------------------------------
//! reads file
//!
//! @param in [in_f]            file for reading
//! @param in [list_size]       size of list
//!
//! @returns pointer to the file
//------------------------------------------
char* read_file(char* in_f, char** original_text);

//------------------------------------------
//! prints from list of pointers into file
//!
//! @param in [str_list]        list of pointers to string
//! @param in [list_size]       size of list
//! @param in [out_f]           file for printing
//!
//! @returns 0
//------------------------------------------
int file_output(char** str_list, int list_size, char* out_f);

//------------------------------------------
//! check if name of file is available
//!
//! @param in [str]        string that needs to be checked
//!
//! @returns 0
//------------------------------------------
int norm_file(char* str);

//------------------------------------------
//! make list of pointers to readen strings
//!
//! @param out [str_list]    list of pointers to string
//! @param in [file_copy]    file that cutting to strings
//!
//! @returns size of list
//------------------------------------------
int form_list(char* file_copy, char** str_list);

//------------------------------------------
//! compares strings
//!
//! @param in [s1]    1st string
//! @param in [s2]    2nd string
//!
//! @returns 1 or -1
//------------------------------------------
int compare_from_end(const void* s1, const void* s2);

//------------------------------------------
//! compares strings
//!
//! @param in [s1]    1st string
//! @param in [s2]    2nd string
//!
//! @returns 1 or -1
//------------------------------------------
int compare_from_begin(const void* s1, const void* s2);

//------------------------------------------
//! counts chars c in string str
//!
//! @param in [c]     char for search
//! @param in [str]   string
//!
//! @returns number of char c in string str
//------------------------------------------
int c_in_str_count(char c, char* str);

//------------------------------------------
//! creturn size of file in bytes
//!
//! @param in [in_f]  name of file
//!
//! @returns 1 or -1
//------------------------------------------
int size_of_file(char* in_f);

//------------------------------------------
//! reads file and make array of pointers to its strings
//!
//! @param in  [in_f]  name of file
//! @param out [in_f]  pinter to text from file in one string
//! @param out [in_f]  pinter to array of pointers to strings
//!
//! @returns 1 or -1
//------------------------------------------
int list_readen_file(char* in_f, char** original_text, char*** str_list);


int main()
{
    setlocale(LcAll, "");

    char in_f[MaxStr + 1] = "";
    printf("enter input file name\n");
    scanf("%s", in_f);
    while (!norm_file(in_f))
    {
        printf("unavaible name of file, try again\n");
        scanf("%s", in_f);
    }

    char* original_text = "";
    char** str_list = {};
    int list_size = list_readen_file(in_f, &original_text, &str_list);

    qsort(str_list, list_size, sizeof(char*), compare_from_begin);
    file_output(str_list, list_size, "e1.txt");
    qsort(str_list, list_size, sizeof(char*), compare_from_end);
    file_output(str_list, list_size, "e2.txt");

    char* original_text_foroutputting[1] = {original_text};
    file_output(original_text_foroutputting, 1, "e_orig.txt");

    free(original_text);
    return 0;
}


int compare_from_end(const void* s1, const void* s2)
{
    assert(s1 != NULL);
    assert(s2 != NULL);
    int s1_len = strlen(*(char**)s1);
    int s2_len = strlen(*(char**)s2);
    int i1 = s1_len - 1;
    while (i1 >= 0 && !isalpha((*(char**)s1)[i1]))
        i1--;
    int i2 = s2_len - 1;
    while (i2 >= 0 && !isalpha((*(char**)s2)[i2]))
        i2--;

    while(i1 >= 0 && i2 >= 0 && (*(char**)s1)[i1] == (*(char**)s2)[i2])
    {
        i1--;
        i2--;
    }
    if (i1 < 0)
        return -1;
    else if (i2 < 0)
        return 1;

    return ((*(char**)s1)[i1] > (*(char**)s2)[i2])? 1: -1;
}

int compare_from_begin(const void* s1, const void* s2)
{
    assert(*(char**)s1 != NULL);
    assert(*(char**)s2 != NULL);
    return stricmp(*(char**)s1, *(char**)s2);
}

int form_list(char* file_copy, char** str_list)
{
    assert(file_copy != NULL);
    str_list[0] = file_copy;
    int i = 0;
    char* current_position = file_copy - 1;
    while (current_position = strchr(current_position + 1, '\n'))
    {
        str_list[++i] = current_position + 1;
        *current_position = '\0';
    }
    return i;
}

int norm_file(char* str)
{
    return str[strlen(str) - 4] == '.' && str[strlen(str) - 3] == 't' && str[strlen(str) - 2] == 'x' && str[strlen(str) - 1] == 't';
}

char* read_file(char* in_f, char** original_text)
{
    assert(in_f != NULL);
    assert(*in_f != NULL);
    FILE* in_file = fopen(in_f, "r");

    if (in_file == nullptr)
        return nullptr;

    int file_len = size_of_file(in_f);

    *original_text = (char*)calloc(file_len, sizeof(char*));
    assert(*original_text != NULL);
    int real_file_len = fread(*original_text, sizeof(char), file_len, in_file);
    assert(real_file_len != file_len);

    fclose(in_file);
    return *original_text;
}

int file_output(char** str_list, int list_size, char* out_f)
{
    assert(str_list != NULL);
    assert(list_size >= 0);
    assert(out_f != NULL);

    FILE* out_file = fopen(out_f, "w");

    for (int i = 0; i < list_size; i++)
    {
        fputs(str_list[i], out_file);
        fputs("\n", out_file);
    }

    fclose(out_file);

    return 0;
}

int size_of_file(char* in_f)
{
    assert(in_f != nullptr);
    struct stat file_stat;
    stat(in_f, &file_stat);

    return file_stat.st_size;
}

int list_readen_file(char* in_f, char** original_text, char*** str_list)
{
    read_file(in_f, original_text);
    char* text_copy = strdup(*original_text);
    assert(text_copy != NULL);
    *str_list = (char**)calloc(c_in_str_count('\n', text_copy), sizeof(char*));
    int list_size = form_list(text_copy, *str_list);

    free(text_copy);
    return list_size;
}

int c_in_str_count(char c, char* str)
{
    assert(c != NULL);
    assert(str != nullptr);

    int i = 0, n = 0;

    while (str[i] != '\0')
    {
        if (str[i] == c)
            n++;
        i++;
    }

    return n;
}
