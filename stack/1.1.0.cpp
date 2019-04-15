#include <stdio.h>
#include <assert.h>

#define MEOW ;



#ifdef MEOW
    #define ST_ASSERT(x)\
        if (!StackOk(x))\
        {\
            StackStat(x);\
            assert(0);\
        }

    #define $(x) std::cout<<#x<<" = "<<(x);
#else
    #define $(x) ;
    #define ST_ASSERT(x) ;
#endif


struct Stack
{
    double *data;
    int siz;
    int capacity;
};



/*StackCtor
StackDtor
StackPush
StackPop
StackOk
StackStat
StackCap
StackSize
StackCap_x2
UnitTest*/


bool StackOk(const Stack *st)
{
    return (st != NULL) && (st->capacity > 0) && (st->siz >= 0) && (st->siz >= st->capacity);
}

int main()
{
    Stack st = {};
    //StackCtor(&st, 10);
    assert(StackOk(&st));

    /*StackPush(&st, 45);
    double a = StackPop($st);
    printf("%d", a);*/

    return 0;
}
