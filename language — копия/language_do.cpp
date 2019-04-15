

#include <iostream>
#define $(x) std::cout<<#x<<" = "<<(x)<<"\n";


#define Assert(x) \
    if (!(x))\
    {\
        $(str_num);\
        $(s);\
        assert(x);\
    }


#include "language.cpp"
#include "stack.cpp"
//#include <TXlib.h>


double memory[MaxArr] = {};
int BP = 0;
double RAX = 0;
const int Max_names_count = 10;


int val_count(Node* node);
int make_name_table(Node* node, char** name_table, int* cur_num);
int take_adress(Node* node, char** name_table);
double calculate(Node** big_root, Node* node, char** name_table);
bool calc_bool(Node** big_root, Node* node, char** name_table);
int do_oper_node(Node** big_root, Node* node, char** name_table);
int do_func(Node** big_root, Node* node, Stack* st);
int start_func(Node** big_root, Node* node, char** name_table);



int val_count(Node* node)
{
    if (node == nullptr)
        return 0;

    if (node->type == OPER_VALUE_create)
        return val_count(node->left) + val_count(node->right) + 1;
    else
        return val_count(node->left) + val_count(node->right);
}

int make_name_table(Node* node, char** name_table, int* cur_num)
{
    if (node == nullptr)
        return 0;

    if (node->type == OPER_VALUE_create)
    {
        name_table[*cur_num] = (char*)(int)(node->info + 0.1);
        *cur_num += 1;

    }

    make_name_table(node->left,  name_table, cur_num);
    make_name_table(node->right, name_table, cur_num);

    return 0;
}

int take_adress(Node* node, char** name_table)
{
    int i = 1;
    while ( strcmp( name_table[i], (char*)(int)(node->info + 0.1) ) )
        i++;

    return i;
}

double calculate(Node** big_root, Node* node, char** name_table)
{
    if (node == nullptr)
        return 0;

    if (node->type == OPERATION)
        switch ((int)node->info)
        {
            case OP_plus:
                return calculate(big_root, node->left,  name_table)
                     + calculate(big_root, node->right, name_table);

            case OP_minus:
                return calculate(big_root, node->left,  name_table)
                     - calculate(big_root, node->right, name_table);

            case OP_mul:
                return calculate(big_root, node->left,  name_table)
                     * calculate(big_root, node->right, name_table);

            case OP_div:
                return calculate(big_root, node->left,  name_table)
                     / calculate(big_root, node->right, name_table);

            case OP_sqrt:
                return sqrt(calculate(big_root, node->right, name_table));

            default:
                Assert(0);
        }

    else if (node->type == CONST_)
        return node->info;

    else if (node->type == VALUE)
        return memory[BP - take_adress(node, name_table)];

    else if (node->type == FUNC_DO)
    {
        start_func(big_root, node, name_table);
        return RAX;
    }

    else Assert(0);



    return 0;
}

bool calc_bool(Node** big_root, Node* node, char** name_table)
{
    Assert(node != nullptr);
    Assert(node->type == OPER_SRAWN);

    double left_calc  = calculate(big_root, node->left,  name_table);
    double right_calc = calculate(big_root, node->right, name_table);

    switch ( (int)(node->info + 0.1) )
    {
        case SRAWN_AbEq:
            return left_calc > right_calc || fabs(left_calc - right_calc) < EPS;

        case SRAWN_Ab:
            return left_calc > right_calc;

        case SRAWN_Be:
            return left_calc < right_calc;

        case SRAWN_BeEq:
            return left_calc < right_calc || fabs(left_calc - right_calc) < EPS;

        case SRAWN_Eq:
            return fabs(left_calc - right_calc) < EPS;

        case SRAWN_NoEq:
            return fabs(left_calc - right_calc) >= EPS;

        default:
            Assert(0);
    }

    return 0;
}

int do_oper_node(Node** big_root, Node* node, char** name_table)
{
    if (node == nullptr)
        return 0;

    if (node->type == OPERATOR)
    {
        do_oper_node(big_root, node->left,  name_table);
        do_oper_node(big_root, node->right, name_table);
        return 0;
    }

    int adress = -1;
    double qwer = 0;

    switch (node->type)
    {
        case OPER_VALUE_create:
            adress = take_adress(node, name_table);
            memory[BP - adress] = 0;
            break;

        case OPER_OUT:
            adress = take_adress(node, name_table);
            printf("%.2lf\n", memory[BP - adress]);
            break;

        case OPER_IN:
            adress = take_adress(node, name_table);
            scanf("%lf", &memory[BP - adress]);
            break;

        case OPER_A:
            adress = take_adress(node->left, name_table);
            memory[BP - adress] = calculate(big_root, node->right, name_table);
            break;

        case OPER_IF:
            if (calc_bool(big_root, node->left, name_table))
                do_oper_node(big_root, node->right, name_table);
            break;

        case OPER_RET:
            RAX = calculate(big_root, node->right, name_table);
            break;

        default: Assert(0);
    }


    return 0;
}

int do_func(Node** big_root, Node* node, Stack* st)
{
    char* name_table[Max_names_count] = {};


    Node* arg_node = node->left;

    int i = 0;

    if (st != nullptr)
        while (arg_node != nullptr)
        {
            memory[BP + i] = StackPop(st);
            arg_node = arg_node->left;
            i++;
        }

    int value_count = val_count(node);

    int dBP = value_count + i;

    BP += dBP;


    if (st != nullptr)
    {
        i = 1 + value_count;
        arg_node = node->left;
        while (arg_node != nullptr)
        {
            name_table[i] = (char*)(int)(arg_node->info + 0.1);
            arg_node = arg_node->left;
            i++;
        }
    }

    int start_num = 1;
    make_name_table(node, name_table, &start_num);

    do_oper_node(big_root, node->right, name_table);

    BP -= dBP;


    return 0;
}

int start_func(Node** big_root, Node* node, char** name_table)
{
    Stack st;
    StackCtor(&st, 4);
    Node* arg_node = node->left;

    while (arg_node != nullptr)
    {
        char* name = (char*)(int)(arg_node->info + 0.1);
        int i = 1;

        while ( strcmp(name_table[i], name) )
            i++;

        StackPush(&st, memory[BP - i]);
        arg_node = arg_node->left;
    }

    int i = 1;

    while ( strcmp( (char*)(int)((big_root[i])->info + 0.1), (char*)(int)(node->info + 0.1) ) )
        i++;


    do_func(big_root, big_root[i], &st);


    return 0;
}

int main()
{

    char in_name[MaxStr] = "fact.cpp";
    char out_name[MaxStr] = "recode.txt";

    printf("enter in_name\n");
    scanf("%s", in_name);

    Node** big_root = read_code(in_name);

    do_func(big_root, big_root[0], nullptr);


    return 0;
}
