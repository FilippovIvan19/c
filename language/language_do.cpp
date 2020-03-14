#include <iostream>
// #define $(x) std::cout<<#x<<" = "<<(x)<<"\n";


#define Assert(x) \
    if (!(x))\
    {\
        $(str_num);\
        $(s);\
        assert(x);\
    }


#include "stack.cpp"
#include "language.cpp"
//#include <TXlib.h>


double memory[MaxArr] = {};
long BP = 0;
double RAX = 0;
const long Max_names_count = 10;


long val_count(Node* node);
long make_name_table(Node* node, char** name_table, long* cur_num);
long take_adress(Node* node, char** name_table);
double calculate(Node** big_root, Node* node, char** name_table);
bool calc_bool(Node** big_root, Node* node, char** name_table);
long do_oper_node(Node** big_root, Node* node, char** name_table);
long do_func(Node** big_root, Node* node, Stack* st);
long start_func(Node** big_root, Node* node, char** name_table);



long val_count(Node* node)
{
    if (node == nullptr)
        return 0;

    if (node->type == OPER_VALUE_create)
        return val_count(node->left) + val_count(node->right) + 1;
    else
        return val_count(node->left) + val_count(node->right);
}

long make_name_table(Node* node, char** name_table, long* cur_num)
{
    if (node == nullptr)
        return 0;

    if (node->type == OPER_VALUE_create)
    {
        name_table[*cur_num] = (char*)(long)(node->info + 0.1);
        *cur_num += 1;

    }

    make_name_table(node->left,  name_table, cur_num);
    make_name_table(node->right, name_table, cur_num);

    return 0;
}

long take_adress(Node* node, char** name_table)
{
    long i = 1;
    while ( strcmp( name_table[i], (char*)(long)(node->info + 0.1) ) )
        i++;

    return i;
}

double calculate(Node** big_root, Node* node, char** name_table)
{
    if (node == nullptr)
        return 0;

    if (node->type == OPERATION)
        switch ((long)node->info)
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

    switch ( (long)(node->info + 0.1) )
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

long do_oper_node(Node** big_root, Node* node, char** name_table)
{
    if (node == nullptr)
        return 0;

    if (node->type == OPERATOR)
    {
        do_oper_node(big_root, node->left,  name_table);
        do_oper_node(big_root, node->right, name_table);
        return 0;
    }

    long adress = -1;
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

long do_func(Node** big_root, Node* node, Stack* st)
{
    char* name_table[Max_names_count] = {};


    Node* arg_node = node->left;

    long i = 0;

    if (st != nullptr)
        while (arg_node != nullptr)
        {
            memory[BP + i] = StackPop(st);
            arg_node = arg_node->left;
            i++;
        }

    long value_count = val_count(node);

    long dBP = value_count + i;

    BP += dBP;


    if (st != nullptr)
    {
        i = 1 + value_count;
        arg_node = node->left;
        while (arg_node != nullptr)
        {
            name_table[i] = (char*)(long)(arg_node->info + 0.1);
            arg_node = arg_node->left;
            i++;
        }
    }

    long start_num = 1;
    make_name_table(node, name_table, &start_num);

    do_oper_node(big_root, node->right, name_table);

    BP -= dBP;


    return 0;
}

long start_func(Node** big_root, Node* node, char** name_table)
{
    Stack st;
    StackCtor(&st, 4);
    Node* arg_node = node->left;

    while (arg_node != nullptr)
    {
        char* name = (char*)(long)(arg_node->info + 0.1);
        long i = 1;

        /*while ( strcmp(name_table[i], name) )
            i++;

        StackPush(&st, memory[BP - i]);
        arg_node = arg_node->left;*/

        //while ( strcmp(name_table[i], name) )
          //  i++;

        StackPush(&st, calculate(big_root, arg_node->right, name_table));
        arg_node = arg_node->left;
    }

    long i = 1;

    while ( strcmp( (char*)(long)((big_root[i])->info + 0.1), (char*)(long)(node->info + 0.1) ) )
        i++;


    do_func(big_root, big_root[i], &st);


    return 0;
}

long bigDtor(Node** big_root)
{
    long i = 0;
    while (big_root[i] != nullptr)
    {
        NodeDtor(big_root[i]);
        i++;
    }

    return 0;
}

int main()
{
    char in_name[MaxStr] = "code.txt";
    printf("enter in_name\n");
    scanf("%s", in_name);

    Node** big_root = read_code(in_name);
    do_func(big_root, big_root[0], nullptr);

    // char out_name[MaxStr] = "recode.txt";
    // FILE* out_file = fopen(out_name, "w");
    // print_code(out_file, big_root);
    // fclose(out_file);
    // print_code(stdout, big_root);

    bigDtor(big_root);
    return 0;
}
