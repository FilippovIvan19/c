#include <stdio.h>
#include <stdlib.h>
#include <TXlib.h>
#include <string.h>
#include <assert.h>
#include <locale.h>


const double EPS = 0.0000001;
const int l_branch = 0;
const int r_branch = 1;
const int MaxStr = 100;
const int bufsize = 10;
const int DONT_ADD_ROOT = 0;
const int      ADD_ROOT = 1;
const int CONST_ = 0;
const int VALUE = 1;
const int OPER = 2;
const int OP_plus = 1;
const int OP_minus = 2;
const int OP_mul = 3;
const int OP_div = 4;
const int OP_sin = 5;
const int OP_cos = 6;

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



#define TREE_ASSERT(tree) if(!TreeOk(tree)) assert(0);
#define NODE_ASSERT(node) if(!NodeOk(node)) assert(0);

//#define TREE_ASSERT(tree) ;
//#define NODE_ASSERT(node) ;



Node* take_copy(Node* node);




int tree_count(Node* node)
{
    if (node == nullptr)
        return 0;
    return tree_count(node->left) + tree_count(node->right) + 1;
}

bool TreeOk(const Tree* tree)
{
    assert(tree != nullptr);
    //return (tree->count == tree_count(tree->root));
    return 1;
}

bool NodeOk(const Node* node)
{
    assert(node != nullptr);
    return (node->type == CONST_ || node->type == VALUE || node->type == OPER);
}

int NodeCtor(Node* node, int type, const int info)
{
    assert(node != nullptr);
    assert(type == CONST_ || type == VALUE || type == OPER);

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
    if (node == nullptr || node == (Node*)-1)
        return 0;

    NodeDtor(node->left);
    NodeDtor(node->right);

    node->left =   (Node*)-1;
    node->right =  (Node*)-1;
    node->parent = (Node*)-1;

    free(node);

    return 0;
}

int TreeCtor(Tree* tree, const Node* root)
{
    tree->root = (Node*)root;
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

int print_node(const FILE* out_file, const Node* node)
{
    assert(out_file != nullptr);
    assert(node != nullptr);

    switch (node->type)
    {
        case CONST_:
            fprintf((FILE*)out_file, "%.2lf", node->info);
            break;
        case VALUE:
            fprintf((FILE*)out_file, "x");
            break;
        case OPER:
            switch ( (int)(node->info + 0.1) )
            {
                case OP_plus:
                    fprintf((FILE*)out_file, "+");
                    break;
                case OP_minus:
                    fprintf((FILE*)out_file, "-");
                    break;
                case OP_mul:
                    fprintf((FILE*)out_file, "*");
                    break;
                case OP_div:
                    fprintf((FILE*)out_file, "/");
                    break;
                /*case OP_sin:
                    fprintf((FILE*)out_file, "s");
                    break;*/
            }
            break;
    }

    return 0;
}

int print_tree(const FILE* out_file, const Node* node)
{
    assert(out_file != nullptr);

    if (node == nullptr)
        return 0;


    fprintf((FILE*)out_file, "(");
    print_tree((FILE*)out_file, node->left);

    print_node((FILE*)out_file, node);

    print_tree((FILE*)out_file, node->right);
    fprintf((FILE*)out_file, ")");


    return 0;
}

int PasteNode(Tree* tree, Node* node, Node* node_par, const int branch, int add_root)
{
    NODE_ASSERT(node)
    assert(branch == l_branch || branch == r_branch);

    if (tree != nullptr && add_root)
        if (tree->root == nullptr)
            tree->root = node;

    node->parent = node_par;


    if (node_par != nullptr)
        switch (branch)
        {
            case l_branch:
                tree->count -= tree_count(node_par->left);
                node_par->left = node;
                break;
            case r_branch:
                tree->count -= tree_count(node_par->right);
                node_par->right = node;
                break;
        }

    tree->count += tree_count(node);

    return 0;

}

int read_tree(const char* tree_str, Tree* tree, const Node* node_par, const int branch, int add_root)
{
    assert(tree_str != nullptr);
    assert(tree != nullptr);
    assert(branch == l_branch || branch == r_branch);

    Node* node = (Node*)calloc(1, sizeof(Node));


    char simbol = ' ';
    char oper_simbol = ' ';
    char* cur_pos = (char*)tree_str;
    int length = 0;
    int d_length;

    sscanf(cur_pos, "%c", &simbol);
    cur_pos++;
    length++;
    //printf("%cqwer\n\n", simbol);
    assert(simbol == '(');


    double num = 0;
    if (sscanf(cur_pos, "%lf%n", &num, &d_length))
    {
        NodeCtor(node, CONST_, num);

        length  += d_length;
        cur_pos += d_length;
    }
    else
    {
        sscanf(cur_pos, "%c", &simbol);
        switch (simbol)
        {
            case 'x':
                NodeCtor(node, VALUE, 0);
                cur_pos++;
                length++;
                break;

            case 's'://sin
                NodeCtor(node, OPER, OP_sin);
                d_length = read_tree(cur_pos, tree, node, l_branch, ADD_ROOT);
                length  += d_length;
                cur_pos += d_length;
                break;

            case '(':
                Node  node_pre_cop;
                Node* node_pre_copy = &node_pre_cop;
                NodeCtor(node_pre_copy, CONST_, 0);

                d_length = read_tree(cur_pos, tree, node_pre_copy, l_branch, DONT_ADD_ROOT);
                length  += d_length;
                cur_pos += d_length;

                sscanf(cur_pos, "%c", &oper_simbol);
                cur_pos++;
                length++;

                switch (oper_simbol)
                {
                    case '+':
                        NodeCtor(node, OPER, OP_plus);
                        break;
                    case '-':
                        NodeCtor(node, OPER, OP_minus);
                        break;
                    case '*':
                        NodeCtor(node, OPER, OP_mul);
                        break;
                    case '/':
                        NodeCtor(node, OPER, OP_div);
                        break;

                }

                node->left = take_copy(node_pre_copy->left);
                NodeDtor(node_pre_copy);


                d_length = read_tree(cur_pos, tree, node, r_branch, DONT_ADD_ROOT);
                length  += d_length;
                cur_pos += d_length;
                break;
        }
    }


    sscanf(cur_pos, "%c", &simbol);
    cur_pos++;
    length++;
    assert(simbol == ')');

    PasteNode(tree, node, (Node*)node_par, branch, add_root);
    tree->count++;


    return length;
}



#define NL diff_node->left
#define NR diff_node->right
#define dL take_diff(tree, node->left)
#define dR take_diff(tree, node->right)
#define cL take_copy(tree, node->left)
#define cR take_copy(tree, node->right)


Node* n_PLUS(Tree* tree, Node* node_l, Node* node_r)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(new_node, OPER, OP_plus);
    PasteNode(tree, node_l, new_node, l_branch, DONT_ADD_ROOT);
    PasteNode(tree, node_r, new_node, r_branch, DONT_ADD_ROOT);
    //new_node->left  = node_l;
    //new_node->right = node_r;


    return new_node;
}

Node* n_MINUS(Tree* tree, Node* node_l, Node* node_r)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(new_node, OPER, OP_minus);
    PasteNode(tree, node_l, new_node, l_branch, DONT_ADD_ROOT);
    PasteNode(tree, node_r, new_node, r_branch, DONT_ADD_ROOT);
    //new_node->left  = node_l;
    //new_node->right = node_r;


    return new_node;
}

Node* n_MUL(Tree* tree, Node* node_l, Node* node_r)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(new_node, OPER, OP_mul);
    PasteNode(tree, node_l, new_node, l_branch, DONT_ADD_ROOT);
    PasteNode(tree, node_r, new_node, r_branch, DONT_ADD_ROOT);
    //new_node->left  = node_l;
    //new_node->right = node_r;


    return new_node;
}

Node* n_DIV(Tree* tree, Node* node_l, Node* node_r)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    NodeCtor(new_node, OPER, OP_div);
    PasteNode(tree, node_l, new_node, l_branch, DONT_ADD_ROOT);
    PasteNode(tree, node_r, new_node, r_branch, DONT_ADD_ROOT);
    //new_node->left  = node_l;
    //new_node->right = node_r;


    return new_node;
}

Node* take_copy(Tree* tree, Node* node)
{
    if (node == nullptr)
        return nullptr;

    Node* node_copy = (Node*)calloc(1, sizeof(Node));
    NodeCtor(node_copy, node->type, node->info);

    //node_copy->left  = cL;
    //node_copy->right = cR;
    PasteNode(tree, cL, node_copy, l_branch, DONT_ADD_ROOT);
    PasteNode(tree, cR, node_copy, r_branch, DONT_ADD_ROOT);

    return node_copy;
}

Node* take_diff(Tree* tree, Node* node)
{
    NODE_ASSERT(node)


    Node* diff_node = (Node*)calloc(1, sizeof(Node));

    switch (node->type)
    {
        case CONST_:
            NodeCtor(diff_node, CONST_, 0);
            break;
        case VALUE:
            NodeCtor(diff_node, CONST_, 1);
            break;
        case OPER:
            switch ( (int)(node->info + 0.1) )
            {
                case OP_plus:
                    NodeCtor(diff_node, OPER, OP_plus);
                    NL = dL;
                    NR = dR;
                    break;

                case OP_minus:
                    NodeCtor(diff_node, OPER, OP_minus);
                    NL = dL;
                    NR = dR;
                    break;

                case OP_mul:
                    NodeCtor(diff_node, OPER, OP_plus);
                    NL = n_MUL(tree, dL, cR);
                    NR = n_MUL(tree, cL, dR);
                    break;

                case OP_div:
                    NodeCtor(diff_node, OPER, OP_div);
                    NL = n_MINUS( tree, n_MUL(tree, dL, cR), n_MUL(tree, cL, dR) );
                    NR = n_MUL(tree, cR, cR);

                    break;


            }

            break;



    }

    return diff_node;

}

Node* simplify(Tree* tree, Node* node)
{
    NODE_ASSERT(node)



    Node* sim_node;

    if (node->type == OPER)
    {
        switch ( (int)(node->info + 0.1) )
        {
            case OP_plus:
                     if (node->left->type  == CONST_ && fabs(node->left->info) < EPS)
                    sim_node = simplify(tree, node->right);
                else if (node->right->type == CONST_ && fabs(node->right->info) < EPS)
                    sim_node = simplify(tree, node->left);
                else if (node->right->type == CONST_ && node->left->type == CONST_)
                {
                    Node* sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, CONST_, node->left->info + node->right->info);
                }
                else
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, node->type, node->info);

                    sim_node->left  = simplify(tree, node->left);
                    sim_node->right = simplify(tree, node->right);
                }
                break;

            case OP_minus:
                     if (node->left->type  == CONST_ && fabs(node->left->info) < EPS)
                    sim_node = simplify(tree, node->right);
                else if (node->right->type == CONST_ && fabs(node->right->info) < EPS)
                    sim_node = simplify(tree, node->left);
                else if (node->right->type == CONST_ && node->left->type == CONST_)
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, CONST_, node->left->info - node->right->info);
                }
                else
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, node->type, node->info);
                    sim_node->left  = simplify(tree, node->left);
                    sim_node->right = simplify(tree, node->right);
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
                    sim_node = simplify(tree, node->right);
                else if (node->right->type == CONST_ && fabs(node->right->info - 1) < EPS)
                    sim_node = simplify(tree, node->left);
                else
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, node->type, node->info);
                    sim_node->left  = simplify(tree, node->left);
                    sim_node->right = simplify(tree, node->right);
                }
                break;

            case OP_div:
                     if (node->left->type  == CONST_ && fabs(node->left->info)  < EPS)
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, CONST_, 0);
                }
                else if (node->right->type == CONST_ && fabs(node->right->info - 1) < EPS)
                    sim_node = simplify(tree, node->left);
                else
                {
                    sim_node = (Node*)calloc(1, sizeof(Node));
                    NodeCtor(sim_node, node->type, node->info);
                    sim_node->left  = simplify(tree, node->left);
                    sim_node->right = simplify(tree, node->right);
                }
                break;



        }


    }
    else
        sim_node = take_copy(tree, node);





    return sim_node;
}



int main()
{
    Tree tr;
    TreeCtor(&tr, nullptr);

    FILE* fd = fopen("f(x).txt", "r");
    char str[MaxStr] = "";
    fscanf(fd, "%s", str);
    read_tree(str, &tr, tr.root, l_branch, ADD_ROOT);
    fclose(fd);

    printf("%d\n", tr.count);
    printf("\n\nreading completed\n\n");

    print_tree(stdout, tr.root);
    printf("\n\nprinting completed\n\n");

    Tree diff_tr;
    TreeCtor(&diff_tr, nullptr);
    take_diff(&diff_tr, tr.root);

    Tree sim_tr;
    TreeCtor(&sim_tr, simplify(diff_tr.root));

    print_tree(stdout, diff_tr.root);
    printf("\n\n\n\n");
    print_tree(stdout, sim_tr.root);

    fd = fopen("f'(x).txt", "w");
    print_tree(fd, diff_tr.root);
    fclose(fd);

    printf("\n\nprinting completed\n\n");


    /*char wer[100] = "12345.2345kjdfkjfdj";
    double q;
    int i = 8;
    sscanf(wer, "%lf%n", &q, &i);
    printf("%d", i);*/


    //printf("\n\n%d", tr.root->left->type);
    //printf("\n\n%d", tr.root->right->type);
    //printf("\n\n%d", tr.root->parent->type);

    return 0;
}
