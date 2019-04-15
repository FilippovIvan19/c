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

#define st_t int


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
int StackCap(const Stack *st);

//---------------------------------------------------------------------
//! returns stack size
//!
//! @param in [st]           pointer to stack
//!
//! @returns stack size
//---------------------------------------------------------------------
int StackSize(const Stack *st);

//---------------------------------------------------------------------
//! returns top stack element
//!
//! @param in [st]           pointer to stack
//!
//! @returns top stack element
//---------------------------------------------------------------------
st_t StackPop(Stack *st);

//---------------------------------------------------------------------
//! check if stack works right
//!
//! @param in [st]           pointer to stack
//!
//! @returns 0 if stack works right
//---------------------------------------------------------------------
int UnitTest(Stack *st);


int main()
{
    setlocale(LcAll, "");

    Stack st = {};
    UnitTest(&st);

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
        fprintf(out_f, "[%d] = %d\n", i, st->data[i]);

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

    if (st->siz == st->capacity){
        StackCap_x2(st);
    StackStat(st, "2.txt");}

    st->siz++;
    st->data[st->siz - 1] = x;

    ST_ASSERT(st);

    return 0;
}

int StackCap_x2(Stack *st)
{
    ST_ASSERT(st);
    st->data = (st_t*)realloc(st->data, 2 * st->capacity * sizeof(st_t));
    st->capacity *= 2;
    ST_ASSERT(st);
    //StackStat(st, "2.txt");
    return 0;
}

int StackCap(const Stack *st)
{
    return st->capacity;
}

int StackSize(const Stack *st)
{
    return st->siz;
}

st_t StackPop(Stack *st)
{
    ST_ASSERT(st);
    st->siz--;
    ST_ASSERT(st);

    st_t x = st->data[st->siz];

    ST_ASSERT(st);
    return x;
}

int UnitTest(Stack *st)
{

    StackCtor(st, 10);
    StackPush(st, 4);
    StackPush(st, 5);
    StackPush(st, 666);
    assert(StackPop(st) == 666);
    assert(StackPop(st) == 5);
    assert(StackPop(st) == 4);
    StackDtor(st);

    StackCtor(st, 10);
    StackPush(st, 4);
    assert(StackPop(st) == 4);
    StackPush(st, 5);
    assert(StackPop(st) == 5);
    StackPush(st, 666);
    assert(StackPop(st) == 666);
    StackDtor(st);

    StackCtor(st, 270);
    StackPush(st, 4);
    StackPush(st, 748);
    StackPush(st, -3443);
    StackPush(st, 202);
    assert(StackPop(st) == 202);
    assert(StackPop(st) == -3443);
    assert(StackPop(st) == 748);
    StackPush(st, 11);
    StackPush(st, 7);
    assert(StackPop(st) == 7);
    assert(StackPop(st) == 11);
    assert(StackPop(st) == 4);
    assert(StackSize(st) == 0);
    StackDtor(st);

    StackCtor(st, 2);
    StackPush(st, 4);
    StackPush(st, 748);
    StackPush(st, -3443);
    StackPush(st, 203);
    StackPush(st, 202);
    assert(StackPop(st) == 202);
    assert(StackPop(st) == 203);
    assert(StackSize(st) == 3);
    assert(StackCap(st) == 8);
    StackDtor(st);

    StackCtor(st, 3);
    StackPush(st, 4);
    StackPush(st, 748);
    StackPush(st, -3443);
    StackPush(st, 203);
    StackPush(st, 202);
    StackPush(st, 4);
    StackPush(st, 748);
    StackPush(st, -3443);
    StackPush(st, 203);
    StackPush(st, 202);
    assert(StackSize(st) == 10);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 202);
    assert(StackSize(st) == 9);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 203);
    assert(StackSize(st) == 8);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == -3443);
    assert(StackSize(st) == 7);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 748);
    assert(StackSize(st) == 6);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 4);
    assert(StackSize(st) == 5);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 202);
    assert(StackSize(st) == 4);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 203);
    assert(StackSize(st) == 3);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == -3443);
    assert(StackSize(st) == 2);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 748);
    assert(StackSize(st) == 1);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 4);
    assert(StackSize(st) == 0);
    assert(StackCap(st) == 12);
    StackDtor(st);

    StackCtor(st, 3);
    StackPush(st, 4);
    StackPush(st, 748);
    StackPush(st, -3443);
    StackPush(st, 203);
    StackPush(st, 202);
    assert(StackSize(st) == 5);
    assert(StackCap(st) == 6);
    assert(StackPop(st) == 202);
    assert(StackSize(st) == 4);
    StackPush(st, 747);
    assert(StackSize(st) == 5);
    assert(StackCap(st) == 6);
    assert(StackPop(st) == 747);
    assert(StackSize(st) == 4);
    StackPush(st, 19);
    assert(StackSize(st) == 5);
    assert(StackCap(st) == 6);
    assert(StackPop(st) == 19);
    assert(StackSize(st) == 4);
    StackPush(st, 199);
    StackPush(st, 1999);
    StackPush(st, 19999);
    assert(StackSize(st) == 7);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 19999);
    assert(StackSize(st) == 6);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 1999);
    assert(StackSize(st) == 5);
    assert(StackCap(st) == 12);
    assert(StackPop(st) == 199);
    assert(StackSize(st) == 4);
    StackDtor(st);


    return 0;
}
