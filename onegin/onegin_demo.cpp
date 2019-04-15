#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

const int LcAll = 0;
const int MaxStr = 100;
const int MaxListSize = 10000;

char* maxstr (char* s1, char* s2);
int form_list(char* str_list[MaxListSize], char in_f[]);     //create list of strings from file
int file_output(char* str_list[MaxListSize], int list_size, char out_f[]);
int ListLineSwap(char** a, char** b);
int str_sort(char* str_list[MaxListSize], int list_size);
int add_str(char* str_list[], char str[], int n);


int main()
{
    char* str_list[MaxListSize] = {};
    setlocale(LcAll, "");

    char in_f[MaxStr + 1] = "";
    printf("enter input file name\n");
    scanf("%s", in_f);
    int list_size = form_list(str_list, in_f);

    str_sort(str_list, list_size);

    printf("enter output file name\n");
    char out_f[MaxStr + 1] = "";
    scanf("%s", out_f);

    file_output(str_list, list_size, out_f);

    return 0;
}



char* maxstr (char* s1, char* s2)
{
    int i = 0;
    int s1_len = strlen(s1), s2_len =strlen(s2);
    while (i < s1_len && i < s2_len && s1[i] == s2[i])
        i++;
    if (s1[i] > s2[i])
        return s1;
    else
        return s2;

}

int add_str(char* str_list[MaxListSize], char str[MaxStr], int n)
{
    int str_len = strlen(str);

    str_list[n] = (char*) calloc(sizeof(str) + 1, sizeof(char));        //writing from n == 0 to n-1
    for (int j = 0; j < str_len; j++)
        *(str_list[n] + j) = *(str + j);
        printf("%d %s", n, str_list[n]);
    //if (n<50 && n > 40)

    return 0;
}

int form_list(char* str_list[MaxListSize], char in_f[])     //create list of strings from file
{
    char str[MaxStr + 1] = "";
    FILE* in_file = fopen(in_f, "r");
    str_list[MaxListSize] = {};

    int n = -1;
    int str_len = -1;
    for (;;)
    {
        if (fgets(str, MaxStr, in_file) == 0) break;
        n++;

        str_len = strlen(str);

        //add_str(str_list, str, n);
        str_list[n] = (char*) calloc(sizeof(str) + 1, sizeof(char));        //writing from n == 0 to n-1
        for (int j = 0; j < str_len; j++)
            *(str_list[n] + j) = *(str + j);
        //if (n<150 && n > 145) printf("%s", str_list[n]);             //str_list[n] = strdup(str); alternative way
    }

    if (*(str_list[n] + str_len - 1) != '\n')
    {
        *(str_list[n] + str_len) = '\n';
        *(str_list[n] + str_len + 1) = '\0';
    }

    fclose(in_file);
    return n + 1;     //returns size of readed file
}

int file_output(char* str_list[MaxListSize], int list_size, char out_f[])
{

    FILE* out_file = fopen(out_f, "w");

    for (int i = 0; i < list_size; i++)
        fputs(str_list[i], out_file);

    fclose(out_file);

    //for (int i = 40; i < 50; i++)
    //    printf("%s", str_list[i]);

    return 0;
}

int ListLineSwap(char** a, char** b)
{
    char* c = *b;
    *b = *a;
    *a = c;

    return 0;
}

int str_sort(char* str_list[MaxListSize], int list_size)
{
    for (int i = 1; i < list_size; i++)
    for (int j = 1; j < list_size - i + 1; j++)
        if (maxstr(str_list[j], str_list[j - 1]) == str_list[j - 1])
            ListLineSwap(&str_list[j], &str_list[j - 1]);

    return 0;
}



