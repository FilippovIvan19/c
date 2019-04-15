#include <stdio.h>
#include <assert.h>
//#include <sys\stat.h>
#include <string.h>
#include <stdlib.h>
#include <TXlib.h>


#include <iostream>
#define $(x) std::cout<<#x<<" = "<<(x)<<"\n";


#define Assert(x) \
    if (!(x))\
    {\
        $(s);\
        assert(x);\
    }


const double EPS = 0.0000001;
const int MaxArr = 100;
const int MaxStr = 100;
const int Max_func_count = 10;

const char* s = NULL;
int str_num = 1;


const int l_branch = 0;
const int r_branch = 1;

const int DONT_ADD_ROOT = 0;
const int      ADD_ROOT = 1;

const int CONST_ = 0;
const int VALUE = 1;
const int OPERATION = 2;

const int OP_plus = 1;
const int OP_minus = 2;
const int OP_mul = 3;
const int OP_div = 4;
const int OP_sin = 5;
const int OP_cos = 6;
const int OP_step = 7;
const int OP_ln = 8;


#define TREE_ASSERT(tree) if(!TreeOk(tree)) Assert(0);
#define NODE_ASSERT(node) if(!NodeOk(node)) Assert(0);


struct Node
{
    int type;
    double info;
    Node* left;
    Node* right;
    Node* parent;
    int child_count;
};

struct Tree
{
    Node* root;
    int count;
};


Node* N_poison = (Node*)-1;


int tree_count(Node* node);
bool TreeOk(const Tree* tree);
bool NodeOk(const Node* node);
int NodeCtor(Node* node, int type, const double info);
int NodeDtor(Node* node);
int TreeCtor(Tree* tree, Node* root);
int TreeDtor(Tree* tree);

//int PasteNode(Tree* tree, Node* node, Node* node_par, const int branch, int add_root);
Node* oper_node_create(Node* node1, Node* node2, char oper);
Node** read_code(const char* in_name);
int print_node(FILE* out_file, const Node* node);
int print_tree(FILE* out_file, const Node* node);
int print_code(FILE* out_file, Node** big_root);
int skipspaces();

Node* GetG(const char* str);
Node* GetE();
Node* GetT();
Node* GetP();
Node* GetS();
Node* GetD();
Node* GetVal();
Node* GetFunc();

Node* take_copy(Node* node);
Node* take_diff(Node* node);

int tree_count(Node* node)
{
    if (node == nullptr)
        return 0;
    return tree_count(node->left) + tree_count(node->right) + 1;
}

bool TreeOk(const Tree* tree)
{
    Assert(tree != nullptr);
    return 1;
}

bool NodeOk(const Node* node)
{
    Assert(node != nullptr && node != N_poison);
    return (node->type == CONST_
         || node->type == VALUE
         || node->type == OPERATION);
}

int NodeCtor(Node* node, int type, const double info)
{
    Assert(node != nullptr);
    Assert(type == CONST_
        || type == VALUE
        || type == OPERATION);

    node->type = type;
    node->info = info;
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;

    NODE_ASSERT(node)
    return 0;
}

int NodeDtor(Node* node)
{
    if (node == nullptr || node == N_poison)
        return 0;

    NodeDtor(node->left);
    NodeDtor(node->right);

    node->left =   N_poison;
    node->right =  N_poison;
    node->parent = N_poison;

    free(node);

    return 0;
}

int TreeCtor(Tree* tree, Node* root)
{
    tree->root = root;
    tree->count = 0;

    TREE_ASSERT(tree)
    return 0;
}

int TreeDtor(Tree* tree)
{
    TREE_ASSERT(tree)

    NodeDtor(tree->root);

    return 0;
}



Node* GetG(const char* str)
{
    s = str;
    Node* root = GetE();
    assert(*s == '\0');

    return root;
}

///      +-
Node* GetE()
{
    skipspaces();
    Node* node1 = GetT();
    skipspaces();

    Node* node = nullptr;

    while (*s == '-' || *s == '+')
    {
        int op = *s;
        s++;

        skipspaces();
        Node* node2 = GetT();
        skipspaces();

        if (op == '+')
        {
            node = oper_node_create(node1, node2, '+');
            node1 = node;
        }
        else
        {
            node = oper_node_create(node1, node2, '-');
            node1 = node;
        }
    }

    skipspaces();
    return node1;
}

///      */
Node* GetT()
{
    skipspaces();
    Node* node1 = GetS();
    skipspaces();

    Node* node = nullptr;

    while (*s == '*' || *s == '/')
    {
        int op = *s;
        s++;

        skipspaces();
        Node* node2 = GetS();
        skipspaces();

        if (op == '*')
        {
            node = oper_node_create(node1, node2, '*');
            node1 = node;
        }
        else
        {
            node = oper_node_create(node1, node2, '/');
            node1 = node;
        }
    }

    skipspaces();
    return node1;
}

Node* GetS()
{
    skipspaces();
    Node* node1 = GetFunc();
    skipspaces();

    if (*s == '^')
    {
        s++;

        skipspaces();
        Node* node2 = GetFunc();
        skipspaces();

        Node* node = oper_node_create(node1, node2, '^');
        node1 = node;
    }

    skipspaces();
    return node1;
}

Node* GetFunc()
{
    skipspaces();
    Node* node = (Node*)calloc(1, sizeof(Node));


    if      ( !strncmp(s, "sin", 3) )
    {
        s += 3;
        NodeCtor(node, OPERATION, OP_sin);
    }
    else if ( !strncmp(s, "cos", 3) )
    {
        s += 3;
        NodeCtor(node, OPERATION, OP_cos);
    }
    else if ( !strncmp(s, "ln",  2) )
    {
        s += 2;
        NodeCtor(node, OPERATION, OP_ln);
    }
    else
    {
        free(node);
        return GetP();
    }


    skipspaces();
    Node* node2 = GetP();
    skipspaces();

    node->right = node2;
    if (node2 != nullptr)
        node2->parent = node;

    return node;
}

///      (E) || Val || D
Node* GetP()
{
    skipspaces();

    if (*s == '(')
    {
        s++;

        skipspaces();
        Node* node = GetE();
        skipspaces();

        Assert(*s == ')');
        s++;
        skipspaces();

        return node;
    }
    else if (*s == 'x')
        return GetVal();
    else
        return GetD();
}

///      double
Node* GetD()
{
    skipspaces();

    double val = 0;
    int n;
    sscanf(s, "%lf%n", &val, &n);
    s += n;

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, CONST_, val);

    skipspaces();
    return node;
}

Node* GetVal()
{
    skipspaces();

    Assert(*s == 'x');
    s++;

    skipspaces();

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, VALUE, 0);

    return node;
}



int skipspaces()
{
    while (*s == ' ' || *s == '\n' || *s == '\t')
    {
        if (*s == '\n')
            str_num++;
        s++;
    }
    return 0;
}

int print_node(FILE* out_file, const Node* node)
{
    Assert(out_file != nullptr);

    if (node == nullptr)
        return 0;

    switch (node->type)
    {
        case CONST_:
            fprintf(out_file, "%.2lf", node->info);
            break;

        case OPERATION:
            switch ( (int)(node->info + 0.1) )
            {
                case OP_plus:
                    fprintf(out_file, " + ");
                    break;
                case OP_minus:
                    fprintf(out_file, " - ");
                    break;
                case OP_mul:
                    fprintf(out_file, " * ");
                    break;
                case OP_div:
                    fprintf(out_file, " / ");
                    break;
                case OP_step:
                    fprintf(out_file, "^");
                    break;
                case OP_sin:
                    fprintf(out_file, "sin");
                    break;
                case OP_cos:
                    fprintf(out_file, "cos");
                    break;
                case OP_ln:
                    fprintf(out_file, "ln");
                    break;
                default: Assert(0);
            }
            break;

        case VALUE:
            fprintf(out_file, "x");
            break;

        default: $(node->info); Assert(0);

    }

    return 0;
}

int print_tree(FILE* out_file, const Node* node)
{
    Assert(out_file != nullptr);


    if (node == nullptr)
        return 0;

    fprintf(out_file, "(");
    print_tree(out_file, node->left);
    print_node(out_file, node);
    print_tree(out_file, node->right);
    fprintf(out_file, ")");

    return 0;
}

#define cL take_copy(node->left)
#define cR take_copy(node->right)

Node* take_copy(Node* node)
{
    if (node == nullptr)
        return nullptr;

    Node* node_copy = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node_copy, node->type, node->info);

    node_copy->left  = cL;
    node_copy->right = cR;

    if (cL != nullptr)
        cL->parent = node_copy;

    if (cR != nullptr)
        cR->parent = node_copy;

    return node_copy;
}

Node* oper_node_create(Node* node1, Node* node2, char oper)
{

    Node* node = (Node*)calloc(1, sizeof(Node));

    switch (oper)
    {
        case '*':
            NodeCtor(node, OPERATION, OP_mul);
            break;
        case '/':
            NodeCtor(node, OPERATION, OP_div);
            break;
        case '+':
            NodeCtor(node, OPERATION, OP_plus);
            break;
        case '-':
            NodeCtor(node, OPERATION, OP_minus);
            break;
        case '^':
            NodeCtor(node, OPERATION, OP_step);
            break;
        default: Assert(0);
    }

    node->left  = node1;
    node->right = node2;

    if (node1 != nullptr)
        node1->parent = node;
    if (node2 != nullptr)
        node2->parent = node;

    return node;
}

Node* simplify(Node* node)
{
    if (node == nullptr)
        return nullptr;

    Node* sim_node;

    if (node->type == OPERATION)
    {
        switch ( (int)(node->info + 0.1) )
        {
            case OP_plus:
                     if (node->left->type  == CONST_ && fabs(node->left->info) < EPS)
                    sim_node = simplify(node->right);

                else if (node->right->type == CONST_ && fabs(node->right->info) < EPS)
                    sim_node = simplify(node->left);

                else if (node->right->type == CONST_ && node->left->type == CONST_)
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, CONST_, node->left->info + node->right->info);
                }
                else
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, node->type, node->info);

                    sim_node->left  = simplify(node->left);
                    sim_node->right = simplify(node->right);
                }
                break;

            case OP_minus:
                     if (node->left->type  == CONST_ && fabs(node->left->info) < EPS)
                     {
                        sim_node = (Node*)calloc(1, sizeof(Node));
                        NodeCtor(sim_node, OPERATION, OP_mul);

                        sim_node->left = (Node*)calloc(1, sizeof(Node));
                        NodeCtor(sim_node->left, CONST_, -1);

                        sim_node->right = simplify(node->right);
                     }

                else if (node->right->type == CONST_ && fabs(node->right->info) < EPS)
                    sim_node = simplify(node->left);

                else if (node->right->type == CONST_ && node->left->type == CONST_)
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, CONST_, node->left->info - node->right->info);
                }
                else
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, node->type, node->info);

                    sim_node->left  = simplify(node->left);
                    sim_node->right = simplify(node->right);
                }
                break;

            case OP_mul:
                     if ((node->left->type  == CONST_ && fabs(node->left->info)  < EPS)
                     ||  (node->left->right == CONST_ && fabs(node->right->info) < EPS))
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, CONST_, 0);
                }
                else if (node->left->type  == CONST_ && fabs(node->left->info - 1) < EPS)
                    sim_node = simplify(node->right);
                else if (node->right->type == CONST_ && fabs(node->right->info - 1) < EPS)
                    sim_node = simplify(node->left);
                else
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, node->type, node->info);
                    sim_node->left  = simplify(node->left);
                    sim_node->right = simplify(node->right);
                }
                break;

            case OP_div:
                     if (node->left->type  == CONST_ && fabs(node->left->info)  < EPS)
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, CONST_, 0);
                }
                else if (node->right->type == CONST_ && fabs(node->right->info - 1) < EPS)
                    sim_node = simplify(node->left);
                else
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, node->type, node->info);
                    sim_node->left  = simplify(node->left);
                    sim_node->right = simplify(node->right);
                }
                break;

            default:
                sim_node = (Node*)calloc(1, sizeof(Node));
                NodeCtor(sim_node, node->type, node->info);

                sim_node->left  = simplify(node->left);
                sim_node->right = simplify(node->right);




        }


    }
    else
        sim_node = take_copy(node);


    return sim_node;
}



#define dL take_diff(node->left)
#define dR take_diff(node->right)
#define ONC(l, r, op) oper_node_create(l, r, op)


Node* take_diff(Node* node)
{
    NODE_ASSERT(node)


    Node* diff_node = nullptr;
    Node* node_f = nullptr;
    Node* node_g = nullptr;
    Node* node_h = nullptr;

    switch (node->type)
    {
        case CONST_:
            diff_node = (Node*)calloc(1, sizeof(Node));
            NodeCtor(diff_node, CONST_, 0);
            break;

        case VALUE:
            diff_node = (Node*)calloc(1, sizeof(Node));
            NodeCtor(diff_node, CONST_, 1);
            break;

        case OPERATION:
            switch ( (int)(node->info + 0.1) )
            {
                case OP_plus:
                    return ONC(dL, dR, '+');

                case OP_minus:
                    return ONC(dL, dR, '-');

                case OP_mul:
                    return ONC( ONC(dL, cR, '*'), ONC(cL, dR, '*'), '+' );

                case OP_div:
                    node_f = ONC( ONC(dL, cR, '*'), ONC(cL, dR, '*'), '-' );

                    node_h = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(node_h, CONST_, 2);

                    node_g = ONC(cR, node_h, '^');

                    return ONC(node_f, node_g, '/');

                case OP_sin:
                    node_h = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(node_h, OPERATION, OP_cos);

                    node_h->right = cR;
                    node_h->right->parent = node_h;

                    return ONC(node_h, dR, '*');

                case OP_cos:
                    node_h = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(node_h, OPERATION, OP_sin);

                    node_h->right = cR;
                    node_h->right->parent = node_h;

                    node_f = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(node_f, CONST_, -1);

                    return ONC( ONC(node_h, dR, '*'), node_f, '*' );

                case OP_ln:
                    return ONC(dR, cR, '/');

                case OP_step:
                    node_f = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(node_f, CONST_, 1);

                    node_g = ONC(cR, node_f, '-');

                    return ONC( ONC( ONC(cL, node_g, '^'), cR, '*' ), dL, '*');

            }
            break;
    }

    return diff_node;

}


Node* big_simp(Node* node)
{
    Node* node1 = take_copy(node);
    for (int i = 0; i < 10; i++)
    {
        Node* node2 = simplify(node1);
        free(node1);
        node1 = node2;
    }

    return node1;
}


int main()
{
    char in_name[MaxStr] = "f(x).txt";
    char out_name[MaxStr] = "f'(x).txt";

    FILE* in_file = fopen(in_name, "r");
    char str[MaxStr] = "";
    fgets(str, MaxStr, in_file);
    fclose(in_file);

    str[strlen(str)] = '\0';
    Node* root = GetG(str);

    Node* diff_root = take_diff(root);
    Node* simp_root = big_simp(diff_root);

    FILE* out_file = fopen(out_name, "w");
    print_tree(out_file, diff_root);
    fclose(out_file);

    print_tree(stdout, root);
    printf("\n\n");
    print_tree(stdout, diff_root);
    printf("\n\n");
    print_tree(stdout, simp_root);



}


