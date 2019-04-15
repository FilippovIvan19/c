#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <locale.h>
#include <iostream>

#define MEOW ;



#ifdef MEOW
    #define ST_ASSERT(x)\
        if (!StackOk(x))\
        {\
            StackStat(x, "assertstat.txt");\
            assert(0);\
        }

    #define $(x) std::cout<<#x<<" = "<<(x)<<"\n";
#else
    #define $(x) ;
    #define ST_ASSERT(x) ;
#endif

#define st_t double



const int LcAll = 0;


struct Stack
{
    st_t *data;
    int siz;
    int capacity;
};

//---------------------------------------------------------------------
//! check if stack is available to use
//!
//! @param in [st]          pointer to stack
//!
//! @returns bool value is stack ok or not
//---------------------------------------------------------------------
bool StackOk(const Stack *st);

//---------------------------------------------------------------------
//! create stack
//!
//! @param in [st]           pointer to stack
//! @param in [len]          length of creating stack
//!
//! @returns pointer to stack
//---------------------------------------------------------------------
Stack *StackCtor(Stack *st, int len);

//---------------------------------------------------------------------
//! prints stack information
//!
//! @param in [st]           pointer to stack
//!
//! @returns 0
//---------------------------------------------------------------------
int StackStat(const Stack *st, const char *out_file_name);

//---------------------------------------------------------------------
//! deletes stack
//!
//! @param in [st]           pointer to stack
//!
//! @returns 0
//---------------------------------------------------------------------
int StackDtor(Stack *st);

//---------------------------------------------------------------------
//! adds new element into stack
//!
//! @param in [st]           pointer to stack
//! @param in [x]            new element
//!
//! @returns 0
//---------------------------------------------------------------------
int StackPush(Stack *st, st_t x);

//---------------------------------------------------------------------
//! makes stack twise bigger
//!
//! @param in [st]           pointer to stack
//!
//! @returns 0
//---------------------------------------------------------------------
int StackCap_x2(Stack *st);

//---------------------------------------------------------------------
//! returns stack capacity
//!
//! @param in [st]           pointer to stack
//!
//! @returns stack capacity
//---------------------------------------------------------------------
int StackCap(Stack *st);

//---------------------------------------------------------------------
//! returns stack size
//!
//! @param in [st]           pointer to stack
//!
//! @returns stack size
//---------------------------------------------------------------------
int StackSize(Stack *st);


/*StackCtor===
StackDtor===
StackPush===
StackPop
StackOk===
StackStat===
StackCap===
StackSize===
StackCap_x2===
UnitTest*/



int main()
{
    setlocale(LcAll, "");

    Stack st = {};
    StackCtor(&st, 10);
    ST_ASSERT(&st);

    //StackDtor(&st);

    StackPush(&st, 45);
    StackPush(&st, 47);
//    double a = StackPop(&st);
//    printf("%d", a);
    ST_ASSERT(&st);
    StackStat(&st, "1.txt");

    return 0;
}

bool StackOk(const Stack *st)
{
    return (st != NULL) && (st->capacity > 0) && (st->siz >= 0) && (st->siz <= st->capacity);
}

Stack *StackCtor(Stack *st, int len)
{
    st->data = (st_t*)calloc(len, sizeof(st_t));
    st->capacity = len;
    st->siz = 0;

    ST_ASSERT(st);

    return st;
}

int StackStat(const Stack *st, const char *out_file_name)
{
    FILE *out_f = fopen(out_file_name, "w");
    fprintf(out_f, "capacity = %d\n", st->capacity);
    fprintf(out_f, "size     = %d\n", st->siz);
    for (int i = 0; i < st->capacity; i++)
        fprintf(out_f, "[%d] = %f %c\n", i, st->data[i], (i >= st->siz)? '@':' ');

    fclose(out_f);
    return 0;
}

int StackDtor(Stack *st)
{

    ST_ASSERT(st);
    st->siz = -1;
    st->capacity = -1;
    free(st);

    return 0;
}

int StackPush(Stack *st, st_t x)
{
    ST_ASSERT(st);

    if (st->siz == st->capacity)
        StackCap_x2(st);

    st->siz++;
    st->data[st->siz - 1] = x;

    ST_ASSERT(st);

    return 0;
}

int StackCap_x2(Stack *st)
{
    ST_ASSERT(st);
    st->data = (st_t*)realloc(st->data, 2 * st->siz);
    st->capacity *= 2;
    ST_ASSERT(st);
    return 0;
}

int StackCap(Stack *st)
{
    return st->capacity;
}

int StackSize(Stack *st)
{
    return st->siz;
}
