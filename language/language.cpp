#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
//#include <TXlib.h>




const long MaxArr = 100;
const long MaxStr = 100;
const long Max_func_count = 10;

const char* s = NULL;
long str_num = 1;


const long l_branch = 0;
const long r_branch = 1;

const long DONT_ADD_ROOT = 0;
const long      ADD_ROOT = 1;

const long CONST_ = 0;
const long VALUE = 1;
const long OPERATION = 2;
const long OPERATOR = 3;
const long OPER_IF = 4;
const long OPER_A = 5;
const long OPER_VALUE_create = 6;
const long OPER_SRAWN = 7;
const long FUNC_create = 8;
const long OPER_OUT = 9;
const long OPER_IN = 10;
const long FUNC_DO = 11;
const long OPER_RET = 12;
const long VOID = 13;

const long OP_plus = 1;
const long OP_minus = 2;
const long OP_mul = 3;
const long OP_div = 4;
const long OP_sqrt = 5;

const long SRAWN_AbEq = 1;
const long SRAWN_Ab   = 2;
const long SRAWN_BeEq = 3;
const long SRAWN_Be   = 4;
const long SRAWN_Eq   = 5;
const long SRAWN_NoEq = 6;


#define TREE_Assert(tree) if(!TreeOk(tree)) Assert(0);
#define NODE_Assert(node) if(!NodeOk(node)) Assert(0);


struct Node
{
    long type;
    double info;
    Node* left;
    Node* right;
    Node* parent;
    long child_count;
};

struct Tree
{
    Node* root;
    long count;
};


Node* N_poison = (Node*)-1;


long tree_count(Node* node);
bool TreeOk(const Tree* tree);
bool NodeOk(const Node* node);
long NodeCtor(Node* node, long type, const double info);
long NodeDtor(Node* node);
long TreeCtor(Tree* tree, Node* root);
long TreeDtor(Tree* tree);

long PasteNode(Tree* tree, Node* node, Node* node_par, const long branch, long add_root);
Node* oper_node_create(Node* node, Node* node1, Node* node2, char oper);
Node** read_code(const char* in_name);
long print_node(FILE* out_file, const Node* node);
long print_tree(FILE* out_file, const Node* node);
long print_code(FILE* out_file, Node** big_root);
long skipspaces();

Node** GetG(const char* str);
Node* GetE();
Node* GetT();
Node* GetP();
Node* GetS();
Node* GetA();
Node* GetOP();
Node* GetIF();
Node* GetD();
Node* GetVal();
Node* GetVal_create();
Node* GetSrawn();
Node* GetFunc_create();
Node* GetFunc_DO();
Node* GetIN();
Node* GetOUT();
Node* GetRET();



long tree_count(Node* node)
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
         || node->type == OPERATION
         || node->type == OPERATOR
         || node->type == OPER_IF
         || node->type == OPER_A
         || node->type == OPER_VALUE_create
         || node->type == OPER_SRAWN
         || node->type == FUNC_create
         || node->type == OPER_OUT
         || node->type == OPER_IN
         || node->type == OPER_RET
         || node->type == FUNC_DO
         || node->type == VOID);
}

long NodeCtor(Node* node, long type, const double info)
{
    Assert(node != nullptr);
    Assert(type == CONST_
        || type == VALUE
        || type == OPERATION
        || type == OPERATOR
        || type == OPER_IF
        || type == OPER_A
        || type == OPER_VALUE_create
        || type == OPER_SRAWN
        || type == FUNC_create
        || type == OPER_OUT
        || type == OPER_IN
        || type == OPER_RET
        || type == FUNC_DO
        || type == VOID);

    node->type = type;
    node->info = info;
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;

    NODE_Assert(node)
    return 0;
}

long NodeDtor(Node* node)
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

long TreeCtor(Tree* tree, Node* root)
{
    tree->root = root;
    tree->count = 0;

    TREE_Assert(tree)
    return 0;
}

long TreeDtor(Tree* tree)
{
    TREE_Assert(tree)

    NodeDtor(tree->root);

    return 0;
}

long PasteNode(Tree* tree, Node* node, Node* node_par, const long branch, long add_root)
{
    NODE_Assert(node)
    Assert(branch == l_branch || branch == r_branch);

    if (tree != nullptr && add_root)
        if (tree->root == nullptr)
            tree->root = node;

    node->parent = node_par;


    if (node_par != nullptr)
        switch (branch)
        {
            case l_branch:
                if (tree != nullptr)
                    tree->count -= tree_count(node_par->left);
                node_par->left = node;
                break;
            case r_branch:
                if (tree != nullptr)
                    tree->count -= tree_count(node_par->right);
                node_par->right = node;
                break;

            default: Assert(0);
        }

    if (tree != nullptr)
        tree->count += tree_count(node);

    return 0;

}

Node* oper_node_create(Node* node, Node* node1, Node* node2, char oper)
{

    node = (Node*)calloc(1, sizeof(Node));

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
        default: Assert(0);
    }

    node->left  = node1;
    node->right = node2;

    node1->parent = node;
    node2->parent = node;

    return node;
}



Node** GetG(const char* str)
{
    s = str;
    Node** root = (Node**)calloc(Max_func_count, sizeof(Node*));
    root[0] = GetFunc_create();

    skipspaces();

    for (long i = 1; *s != '\0'; i++)
    {
        root[i] = GetFunc_create();
        skipspaces();
    }


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
        long op = *s;
        s++;

        skipspaces();
        Node* node2 = GetT();
        skipspaces();

        if (op == '+')
        {
            node = oper_node_create(node, node1, node2, '+');
            node1 = node;
        }
        else
        {
            node = oper_node_create(node, node1, node2, '-');
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
        long op = *s;
        s++;

        skipspaces();
        Node* node2 = GetS();
        skipspaces();

        if (op == '*')
        {
            node = oper_node_create(node, node1, node2, '*');
            node1 = node;
        }
        else
        {
            node = oper_node_create(node, node1, node2, '/');
            node1 = node;
        }
    }

    skipspaces();
    return node1;
}

///      @
Node* GetS()
{
    skipspaces();

    Node* node = nullptr;
    Node* node1 = nullptr;
    Node* node2 = nullptr;


    if (*s == '@')
    {
        node = (Node*)calloc(1, sizeof(Node));
        NodeCtor(node, OPERATION, OP_sqrt);

        s++;

        node1 = node;
        node2 = nullptr;

        while (*s == '@')
        {
            node2 = (Node*)calloc(1, sizeof(Node));
            NodeCtor(node2, OPERATION, OP_sqrt);

            s++;

            node->right = node2;
            node2->parent = node;
            node = node2;

            skipspaces();
        }

        skipspaces();
        node2 = GetP();
        skipspaces();

        node->right = node2;
        node2->parent = node;

        return node1;
    }
    else
    {
        skipspaces();
        return GetP();
    }
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
    else if ('a' <= *s && *s <= 'z')
        return GetVal();
    else if (*s == 'D')
        return GetFunc_DO();
    else
        return GetD();
}

///      double
Node* GetD()
{
    skipspaces();

    double val = 0;
    long n;
    sscanf(s, "%lf%ln", &val, &n);
    s += n;

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, CONST_, val);

    skipspaces();
    return node;
}

Node* GetVal_create()
{
    skipspaces();

    char command[MaxStr] = "";
    sscanf(s, "%s", command);
    Assert( !strcmp(command, "CREATE") );
    s += 7;

    skipspaces();

    char* name = (char*)calloc(MaxStr, sizeof(char));
    long len = 0;
    sscanf(s, "%[a-z_]%ln", name, &len);
    s += len;

    skipspaces();

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, OPER_VALUE_create, (long)name);

    return node;
}

Node* GetSrawn()
{
    skipspaces();

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, OPER_SRAWN, 0);

    node->left = GetE();
    node->left->parent = node;

    skipspaces();

    switch (*s)
    {
        case '>':
            s++;
            if (*s == '=')
            {
                s++;
                node->info = SRAWN_AbEq;
            }
            else
                node->info = SRAWN_Ab;
            break;

        case '<':
            s++;
            if (*s == '=')
            {
                s++;
                node->info = SRAWN_BeEq;
            }
            else
                node->info = SRAWN_Be;
            break;

        case '=':
            s++;
                node->info = SRAWN_Eq;
            break;

        case '#':
            s++;
                node->info = SRAWN_NoEq;
            break;

        default: Assert(0);

    }

    skipspaces();

    node->right = GetE();
    node->right->parent = node;


    skipspaces();
    return node;
}

Node* GetIF()
{
    skipspaces();

    char command[MaxStr] = "";
    sscanf(s, "%s", command);
    Assert( !strcmp(command, "IF") );
    s += 3;
    skipspaces();

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, OPER_IF, 0);

    skipspaces();
    node->left = GetSrawn();
    node->left->parent = node;
    skipspaces();

    node->right = GetOP();
    node->right->parent = node;


    skipspaces();
    return node;
}

Node* GetVal()
{
    skipspaces();

    char* name = (char*)calloc(MaxStr, sizeof(char));
    long len = 0;
    sscanf(s, "%[a-z_]%ln", name, &len);
    s += len;

    skipspaces();

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, VALUE, (long)name);

    skipspaces();
    return node;
}

Node* GetA()
{

    skipspaces();

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, OPER_A, 0);

    node->left = GetVal();
    node->left->parent = node;

    skipspaces();

    Assert(*s == '=');
    s++;
    skipspaces();

    node->right = GetE();
    node->right->parent = node;

    skipspaces();
    return node;
}

Node* GetOUT()
{
    skipspaces();

    char command[MaxStr] = "";
    sscanf(s, "%s", command);
    Assert( !strcmp(command, "OUT") );
    s += 4;

    skipspaces();

    char* name = (char*)calloc(MaxStr, sizeof(char));
    long len = 0;
    sscanf(s, "%[a-z_]%ln", name, &len);
    s += len;

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, OPER_OUT, (long)name);


    skipspaces();
    return node;
}

Node* GetIN()
{
    skipspaces();

    char command[MaxStr] = "";
    sscanf(s, "%s", command);
    Assert( !strcmp(command, "IN") );
    s += 3;

    skipspaces();

    char* name = (char*)calloc(MaxStr, sizeof(char));
    long len = 0;
    sscanf(s, "%[a-z_]%ln", name, &len);
    s += len;

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, OPER_IN, (long)name);


    skipspaces();
    return node;
}

Node* GetOP()
{
    skipspaces();

    char command[MaxStr] = "";
    sscanf(s, "%s", command);

    Node* node = nullptr;
    skipspaces();

    if (*s == '{')
    {
        s++;
        Assert(*s != '}');

        skipspaces();
        node = GetOP();
        skipspaces();
        Node* node1 = node;
        while (*s != '}')
        {
            Node* node2 = GetOP();
            node1->right = node2;
            node2->parent = node1;
            node1 = node2;
            skipspaces();
        }

        Assert(*s == '}');
        s++;
        skipspaces();
    }
    else if ( !strcmp(command, "IF") )
    {
        node = (Node*)calloc(1, sizeof(Node));
        NodeCtor(node, OPERATOR, 0);
        node->left = GetIF();
        node->left->parent = node;
    }
    else if ( !strcmp(command, "CREATE") )
    {
        node = (Node*)calloc(1, sizeof(Node));
        NodeCtor(node, OPERATOR, 0);
        node->left = GetVal_create();
        node->left->parent = node;
    }
    else if ( !strcmp(command, "OUT") )
    {
        node = (Node*)calloc(1, sizeof(Node));
        NodeCtor(node, OPERATOR, 0);
        node->left = GetOUT();
        node->left->parent = node;
    }
    else if ( !strcmp(command, "IN") )
    {
        node = (Node*)calloc(1, sizeof(Node));
        NodeCtor(node, OPERATOR, 0);
        node->left = GetIN();
        node->left->parent = node;
    }else if ( !strcmp(command, "RET") )
    {
        node = (Node*)calloc(1, sizeof(Node));
        NodeCtor(node, OPERATOR, 0);
        node->left = GetRET();
        node->left->parent = node;
    }
    else
    {
        node = (Node*)calloc(1, sizeof(Node));
        NodeCtor(node, OPERATOR, 0);
        node->left = GetA();
        node->left->parent = node;
    }

    skipspaces();

    if (*s == ';')
        s++;
    else
    {
        const char* s_ = s - 1;
        while (*s_ == ' ' || *s_ == '\n' || *s_ == '\t')
            s_--;
        if (*s_ != ';' && *s_ != '}')
            Assert(0);
    }

    skipspaces();

    return node;

}

Node* GetFunc_create()
{
    skipspaces();

    char command[MaxStr] = "";
    sscanf(s, "%s", command);

    Assert( !strcmp(command, "FUNC") );
    s += 5;

    skipspaces();

    char* name = (char*)calloc(MaxStr, sizeof(char));
    long len = 0;
    sscanf(s, "%[a-z_]%ln", name, &len);
    s += len;

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, FUNC_create, (long)name);

    skipspaces();

    Node* node1 = GetVal();
    Node* node3 = node1;

    skipspaces();

    while ('a' <= *s && *s <= 'z')
    {
        skipspaces();
        Node* node2 = GetVal();

        node3->left = node2;
        node2->parent = node3;
        node3 = node2;
    }

    node->left = node1;
    node1->parent = node;

    skipspaces();

    node->right = GetOP();
    node->right->parent = node;

    skipspaces();
    return node;

}

Node* GetFunc_DO()
{
    skipspaces();

    char command[MaxStr] = "";
    sscanf(s, "%s", command);

    Assert( !strcmp(command, "DO") );
    s += 3;

    skipspaces();

    char* name = (char*)calloc(MaxStr, sizeof(char));
    long len = 0;
    sscanf(s, "%[a-z_]%ln", name, &len);
    s += len;

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, FUNC_DO, (long)name);

    skipspaces();

    Node* node1 = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node1, VOID, 0);
    node1->right = GetP();
    node1->right->parent = node1;
    Node* node3 = node1;

    skipspaces();

    while ('a' <= *s && *s <= 'z' || '0' <= *s && *s <= '9' || *s == ')' || *s == '(')
    {
        skipspaces();
        Node* node2 = (Node*)calloc(1, sizeof(Node));
        NodeCtor(node2, VOID, 0);
        node2->right = GetP();
        node2->right->parent = node2;

        node3->left = node2;
        node2->parent = node3;
        node3 = node2;
    }

    node->left = node1;
    node1->parent = node;

    skipspaces();
    return node;

}

Node* GetRET()
{
    skipspaces();

    char command[MaxStr] = "";
    sscanf(s, "%s", command);
    Assert( !strcmp(command, "RET") );
    s += 4;

    skipspaces();

    Node* node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node, OPER_RET, 0);

    node->right = GetE();
    node->right->parent = node;

    skipspaces();
    return node;
}


Node** read_code(const char* in_name)
{
    FILE* in_file = fopen(in_name, "r");

    struct stat file_stat;
    stat(in_name, &file_stat);
    long file_len = file_stat.st_size;

    char* text = (char*)calloc(file_len, sizeof(char));
    fread(text, sizeof(char), file_len, in_file);

    fclose(in_file);

    Node** rez = GetG(text);
    free(text);

    return rez;
}

long skipspaces()
{
    while (*s == ' ' || *s == '\n' || *s == '\t')
    {
        if (*s == '\n')
            str_num++;
        s++;
    }
    return 0;
}

long print_node(FILE* out_file, const Node* node)
{
    Assert(out_file != nullptr);

    if (node == nullptr)
        return 0;

    switch (node->type)
    {
        case CONST_:
            fprintf(out_file, "%.2lf", node->info);
            break;

        case OPERATOR:
            fprintf(out_file, ";\n");
            break;

        case OPERATION:
            switch ( (long)(node->info + 0.1) )
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
                case OP_sqrt:
                    fprintf(out_file, " @ ");
                    break;
                default: Assert(0);
            }
            break;

        case VALUE:
            fprintf(out_file, "%s", (char*)(long)(node->info + 0.1));
            if (node->parent && (
                   node->parent->type == VALUE
                || node->parent->type == FUNC_create
                || node->parent->type == VOID
                )
            )
                fprintf(out_file, " ");
            break;

        case OPER_VALUE_create:
            fprintf(out_file, "CREATE %s", (char*)(long)(node->info + 0.1));
            break;

        case OPER_A:
            fprintf(out_file, " = ");
            break;

        case OPER_IF:
            break;

        case VOID:
            break;

        case OPER_SRAWN:
            switch ( (long)(node->info + 0.1) )
            {
                case SRAWN_Ab:
                    fprintf(out_file, " > ");
                    break;
                case SRAWN_AbEq:
                    fprintf(out_file, " >= ");
                    break;
                case SRAWN_Be:
                    fprintf(out_file, " < ");
                    break;
                case SRAWN_BeEq:
                    fprintf(out_file, " <= ");
                    break;
                case SRAWN_Eq:
                    fprintf(out_file, " = ");
                    break;
                case SRAWN_NoEq:
                    fprintf(out_file, " # ");
                    break;
                default: Assert(0);
            }
            break;

        case FUNC_create:
            break;

        case OPER_OUT:
            fprintf(out_file, "OUT %s", (char*)(long)(node->info + 0.1));
            break;

        case OPER_IN:
            fprintf(out_file, "IN %s", (char*)(long)(node->info + 0.1));
            break;

        case FUNC_DO:
            break;

        case OPER_RET:
            fprintf(out_file, "RET ");
            break;

        default: Assert(0);

    }

    return 0;
}

long print_tree(FILE* out_file, const Node* node)
{
    Assert(out_file != nullptr);


    if (node == nullptr)
        return 0;

    if (node->type == OPERATOR)
    {
        if (node->parent != nullptr)
        {
            if (node->parent->type != OPERATOR)
                fprintf(out_file, "\n{\n");
        }
        else
            fprintf(out_file, "{\n");
    }

    if (node->type == OPER_IF)
        fprintf(out_file, "IF ");

    if (node->type == FUNC_create)
    {
        fprintf(out_file, "FUNC ");
        fprintf(out_file, "%s ", (char*)(long)(node->info + 0.1));
    }

    if (node->type == FUNC_DO)
    {
        fprintf(out_file, "DO ");
        fprintf(out_file, "%s ", (char*)(long)(node->info + 0.1));
    }

    print_tree(out_file, node->left);
    print_node(out_file, node);
    print_tree(out_file, node->right);

    if (node->type == OPERATOR && node->right == nullptr)
        fprintf(out_file, "}");


    return 0;
}

long print_code(FILE* out_file, Node** big_root)
{

    for (long i = 0; big_root[i] != nullptr && i < Max_func_count; i++)
    {
        print_tree(out_file, big_root[i]);
        fprintf(out_file, "\n\n\n");
    }

    return 0;
}

