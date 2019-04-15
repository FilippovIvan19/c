
#include <TXlib.h>
#include <stdio.h>
//#include <assert.h>
//#include <iostream.h>
#include <stdlib.h>

#define List_t double
#define p_Poison (List_t*)(-1)

#define LISTASSERT(ls) \
    if (!ListOK(ls))\
        {\
            ListDump(ls, "dump.txt");\
            assert(0);\
        }

#define MEOW ;




#ifdef MEOW
    #include <iostream>

    #define CPU_ASSERT(x)\
        if (!CPUOk(x))\
        {\
            CPUDump(x, "assertstat.txt", 0);\
            assert(0);\
        }


    #define assert( check )\
        if(!(check))\
        {\
            printf("OOPS: %s in %s, line %d, function %s\n", #check, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
            abort();\
            /*exit(1)*/\
        }

    #define $(x) std::cout<<#x<<" = "<<(x)<<"\n";
#else
    #define $(x) ;
    #define CPU_ASSERT(x) ;
#endif


//#define $(x) std::cout<<#x<<" = "<<(x)<<"\n";

const int List_size = 10;
const int Free_prev = -1;
const int i_Poison = -666;

struct List
{
    List_t* info[List_size];
    int next[List_size];
    int prev[List_size];
    int free;
    int head;
    int tail;
    int count;
};



//---------------------------------------------------------------------
//! check if list is available to use
//!
//! @param in [ls]          pointer to list
//!
//! @returns bool value is list ok or not
//---------------------------------------------------------------------
bool ListOK(List* ls);

//---------------------------------------------------------------------
//! prints information about list
//!
//! @param in [ls]          pointer to list
//! @param in [f_name]      name of file to output
//!
//! @returns 0
//---------------------------------------------------------------------
int ListDump(const List* ls, const char* f_name);

//---------------------------------------------------------------------
//! create list
//!
//! @param in [ls]          pointer to list
//!
//! @returns 0
//---------------------------------------------------------------------
int ListCtor(List* ls);

//---------------------------------------------------------------------
//! destroy list
//!
//! @param in [ls]          pointer to list
//!
//! @returns 0
//---------------------------------------------------------------------
int ListDtor(List* ls);

//---------------------------------------------------------------------
//! delete element from list
//!
//! @param in [ls]          pointer to list
//! @param in [position]    position of deleting element
//!
//! @returns 0
//---------------------------------------------------------------------
int ListDel(List* ls, int position);

//---------------------------------------------------------------------
//! add element to list
//!
//! @param in [ls]          pointer to list
//! @param in [val_p]       element that will be added
//!
//! @returns 0
//---------------------------------------------------------------------
int ListAdd(List* ls, List_t* val_p);

//---------------------------------------------------------------------
//! test list
//!
//! @param in [ls]          pointer to list
//! @param in [arr]         massive of elements
//!
//! @returns 0
//---------------------------------------------------------------------
int unittest(List* ls, List_t* arr);

bool ListOK(List* ls)
{
    bool ok = 1;

    ok =   (ls->count >= 0) && (ls->count != i_Poison)
        && (ls->free  >  0) && (ls->free  != i_Poison)
        && (ls->head  >= 0) && (ls->head  != i_Poison)
        && (ls->tail  >= 0) && (ls->tail  != i_Poison);
    if (!ok) return ok;


    int i = ls->head;
    int j = 0;
    for (; i != ls->tail && j < ls->count;)
    {
        ok = (ls->next[i] > 0) && (ls->next[i] != i_Poison)
            && (ls->prev[i] > 0 || (ls->prev[i] == 0 && i == ls->head)) && (ls->prev[i] != i_Poison)
            && (ls->info[i] != p_Poison);

        if (!ok) return ok;



        i = ls->next[i];
        j++;
    }


    ok = ((i == ls->tail && j == ls->count - 1) || (0 == ls->tail && ls->count == 0));

    if (!ok) return ok;

    ok = (ls->next[i] == 0)
        && (ls->prev[i] > 0 || (ls->prev[i] == 0 && i == ls->head)) && (ls->prev[i] != i_Poison)
        && (ls->info[i] != p_Poison);
    if (!ok) return ok;

    i = ls->free;
    j = 0;
    for (; i != 0 && j < List_size;)
    {
        ok = (ls->next[i] >= 0) && (ls->next[i] != i_Poison)
            && (ls->prev[i] == Free_prev);

        if (!ok) return ok;


        i = ls->next[i];
        j++;
    }

    return ok;
}

int ListCtor(List* ls)
{
    assert(ls != NULL);

    for (int i = 0; i < List_size; i++)
    {
        ls->info[i] = NULL;
        ls->next[i] = i + 1;
        ls->prev[i] = Free_prev;
    }

    ls->next[0] = 0;
    ls->prev[0] = 0;
    ls->next[List_size - 1] = 0;

    ls->count = 0;
    ls->free = 1;
    ls->head = 0;
    ls->tail = 0;

    LISTASSERT(ls)
    return 0;
}

int ListDtor(List* ls)
{
    LISTASSERT(ls)

    for (int i = 0; i < List_size; i++)
    {
        ls->info[i] = p_Poison;
        ls->next[i] = i_Poison;
        ls->prev[i] = i_Poison;
    }

    ls->count = i_Poison;
    ls->free = i_Poison;
    ls->head = i_Poison;
    ls->tail = i_Poison;

    return 0;
}

int ListDel(List* ls, int position)
{
    LISTASSERT(ls)

    assert(position > 0);
    assert(position < List_size);
    assert(ls->prev[position] != Free_prev);

    ls->next[ls->prev[position]] = ls->next[position];
    ls->prev[ls->next[position]] = ls->prev[position];

    ls->info[position] = NULL;
    ls->count--;

    if (ls->head == position)
        ls->head = ls->next[position];

    if (ls->tail == position)
        ls->tail = ls->prev[position];

    ls->prev[position] = Free_prev;
    ls->next[position] = ls->free;

    ls->free = position;


    LISTASSERT(ls)
    return 0;
}

int ListAdd(List* ls, List_t* val_p)
{
    LISTASSERT(ls)

    ls->info[ls->free] = val_p;
    ls->prev[ls->free] = ls->tail;
    if (ls->tail != 0)
        ls->next[ls->tail] = ls->free;

    ls->tail = ls->free;
    if (ls->count == 0)
        ls->head = ls->tail;

    ls->count++;

    int lsf = ls->free;
    ls->free = ls->next[ls->free];

    ls->next[lsf] = 0;

    LISTASSERT(ls)
    return 0;
}

int ListDump(const List* ls, const char* f_name)
{
    assert(ls     != NULL);
    assert(f_name != NULL);

    FILE* out_f = fopen(f_name, "w");

    fprintf(out_f, "info:\n");
    for(int i = 0; i < List_size; i++)
        if (ls->info[i] != NULL)
            fprintf(out_f, "%lf ", *(ls->info[i]) );
        else
            fprintf(out_f, "0 ");
    fprintf(out_f, "\n");

    fprintf(out_f, "next:\n");
    for(int i = 0; i < List_size; i++)
        fprintf(out_f, "%d ", ls->next[i]);
    fprintf(out_f, "\n");

    fprintf(out_f, "prev:\n");
    for(int i = 0; i < List_size; i++)
        fprintf(out_f, "%d ", ls->prev[i]);
    fprintf(out_f, "\n");


    fprintf(out_f, "count: %d\n", ls->count);
    fprintf(out_f, "free: %d\n", ls->free);
    fprintf(out_f, "head: %d\n", ls->head);
    fprintf(out_f, "tail: %d\n", ls->tail);

    fclose(out_f);

    return 0;
}

int unittest(List* ls, List_t* arr)
{

    ListAdd(ls, &arr[0]);
    ListAdd(ls, &arr[1]);
    ListAdd(ls, &arr[2]);
    ListDel(ls, 1);
    ListDump(ls, "1.txt");
    ListAdd(ls, &arr[4]);
    ListAdd(ls, &arr[3]);
    ListDel(ls, 4);
    ListDel(ls, 2);
    ListAdd(ls, &arr[2]);
    ListDel(ls, 1);
    ListDel(ls, 3);
    ListDel(ls, 2);

    ListDump(ls, "2.txt");


    return 0;
}


/*int main()
{
    List ls = {};
    ListCtor(&ls);

    FILE* in_f = fopen("in.txt", "r");

    int n = -1;
    fscanf(in_f, "%d", &n);



    List_t arr[n];//calloc
    for(int i = 0; i < n; i++)
        fscanf(in_f, "%lf", &arr[i]);


    fclose(in_f);

    unittest(&ls, arr);

    //assert(0);

    system("start 1.txt");

    return 0;
}*/
