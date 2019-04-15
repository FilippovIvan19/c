#include <stdio.h>
#include <stdlib.h>
//#include <TXlib.h>
#include <string.h>
#include <assert.h>


const int LcAll = 0;
const int l_branch = 0;
const int r_branch = 1;
const int MaxStr = 100;

struct Node
{
    char* info;
    Node* left;
    Node* right;
    Node* parent;
};

struct Tree
{
    Node* root;
    int count;
};

#include "stack.cpp"

#define TREE_ASSERT(tree) if(!TreeOk(tree)) assert(0);
#define NODE_ASSERT(node) if(!NodeOk(node)) assert(0);

int tree_count(Node* node)
{
    if (node == nullptr || node == (Node*)-1)
        return 0;
    return tree_count(node->left) + tree_count(node->right) + 1;
}

bool TreeOk(const Tree* tree)
{
    assert(tree != nullptr);
    return (tree->count == tree_count(tree->root));
}

bool NodeOk(const Node* node)
{
    assert(node != nullptr);
    return node->info != nullptr;
}

int NodeCtor(Node* node, const char* info)
{
    assert(info != nullptr);

    node->info = (char*)calloc(strlen(info), sizeof(char));
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;
    strcpy(node->info, info);
    NODE_ASSERT(node)

    return 0;
}

int NodeDtor(Node* node)
{
    if (node == nullptr)
        return 0;

    free(node->info);
    node->info = nullptr;

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
    tree->count = -1;

    return 0;
}


int PasteNode(Tree* tree, Node* node, Node* node_par, const int branch)
{
    TREE_ASSERT(tree)
    NODE_ASSERT(node)
    assert(branch == l_branch || branch == r_branch);

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

int read_tree(const FILE* in_file, Tree* tree, const Node* node_par, const int branch)
{
    assert(tree     != nullptr);
    assert(in_file  != nullptr);
    assert(branch == l_branch || branch == r_branch);

    char str[MaxStr] = "";
    fscanf((FILE*)in_file, "%s", str);
    char* str_p = str;

    while (*str_p == '\t' || *str_p == ' ')
        str_p++;
    assert(!strcmp(str_p, "{"));

    fscanf((FILE*)in_file, "%s", str);
    str_p = str;
    while (*str_p == '\t' || *str_p == ' ')
        str_p++;

    if (strcmp(str_p, "nil"))
    {

        Node* node = (Node*)calloc(1, sizeof(Node));
        NodeCtor(node, str_p);

        PasteNode(tree, node, (Node*)node_par, branch);

        read_tree((FILE*)in_file, tree, node, l_branch);
        read_tree((FILE*)in_file, tree, node, r_branch);

    }

    fscanf((FILE*)in_file, "%s", str);
    str_p = str;
    while (*str_p == '\t' || *str_p == ' ')
        str_p++;
    assert(!strcmp(str_p, "}"));

    return 0;
}

int frpintftabs(const FILE* file, const int n_tabs)
{
    assert(file != nullptr);

    for (int i = 0; i < n_tabs; i++)
        fprintf((FILE*)file, "\t");

    return 0;
}

int print_tree(const FILE* file, const Node* node, const int n_tabs)
{
    assert(file != nullptr);

    if (node == nullptr)
    {
        frpintftabs((FILE*)file, n_tabs);
        fprintf((FILE*)file, "{\n");


        frpintftabs((FILE*)file, n_tabs + 1);
        fprintf((FILE*)file, "nil\n");


        frpintftabs((FILE*)file, n_tabs);
        fprintf((FILE*)file, "}\n");

        return 0;
    }


    frpintftabs((FILE*)file, n_tabs);
    fprintf((FILE*)file, "{\n");

    frpintftabs((FILE*)file, n_tabs + 1);
    fprintf((FILE*)file, "%s\n", node->info);


    print_tree((FILE*)file, node->left, n_tabs + 1);
    print_tree((FILE*)file, node->right, n_tabs + 1);


    frpintftabs((FILE*)file, n_tabs);
    fprintf((FILE*)file, "}\n");


    return 0;
}

int node_wasnt_found(Tree* tree, Node* node_par)
{
    TREE_ASSERT(tree)
    assert(node_par != nullptr);

    printf(":((( do you want to add new question?\n");
    printf("enter yes or no\n");

    char answer[MaxStr], question[MaxStr], name[MaxStr];
    scanf("%s", answer);

    if     (!strcmp(answer, "no"))
        return 0;
    else if (strcmp(answer, "yes"))
        assert(0);

    printf("enter new question\n");
    scanf("%s", question);
    printf("enter answer on it (yes or no)\n");
    scanf("%s", answer);
    printf("enter name of this thing\n");
    scanf("%s", name);

    Node* new_node     = (Node*)calloc(1, sizeof(Node));
    Node* new_question = (Node*)calloc(1, sizeof(Node));

    NodeCtor(new_question, question);
    NodeCtor(new_node, name);

    Node* node_p_p = node_par->parent;
    if      (node_p_p->left == node_par)
        PasteNode(tree, new_question, node_p_p, l_branch);
    else if (node_p_p->right == node_par)
        PasteNode(tree, new_question, node_p_p, r_branch);
    else
        assert(0);

    if      (!strcmp(answer, "yes"))
    {
        PasteNode(tree, new_node, new_question, l_branch);
        assert(node_par != nullptr);
        PasteNode(tree, node_par, new_question, r_branch);
    }
    else if (!strcmp(answer, "no"))
    {
        PasteNode(tree, new_node, new_question, r_branch);
        assert(node_par != nullptr);
        PasteNode(tree, node_par, new_question, l_branch);
    }
    else assert(0);

    return 0;
}

bool is_leaf(const Node* node)
{
    NODE_ASSERT(node)

    if (node->left == nullptr && node->right == nullptr)
        return true;
    else
        return false;
}

int find_node(Tree* tree, Node* node)
{
    TREE_ASSERT(tree)
    NODE_ASSERT(node)

    char answer[MaxStr];

    if (is_leaf(node))
    {
        printf("is it %s?\n", node->info);
        printf("enter yes or no\n");
        scanf("%s", answer);

        if      (!strcmp(answer, "yes"))
            printf("I won!!!\n");
        else if (!strcmp(answer, "no"))
            node_wasnt_found(tree, node);
        else assert(0);

    }
    else
    {
        assert(node->left != nullptr);
        assert(node->right != nullptr);


        printf("%s?\n", node->info);
        printf("enter yes or no\n");

        scanf("%s", answer);

        if      (!strcmp(answer, "yes"))
            find_node(tree, node->left);
        else if (!strcmp(answer, "no"))
            find_node(tree, node->right);
        else
            assert(0);
    }

    return 0;
}

int find_word(const Node* node, const char* word, Node** out_p)
{
    assert(word != nullptr);
    assert(out_p != nullptr);

    if (node == nullptr)
        return 0;

    if (!strcmp(node->info, word))
    {
        *out_p = (Node*)node;
    }
    else
    {
        find_word(node->left, word, out_p);
        find_word(node->right, word, out_p);
    }

    return 0;
}

Stack* find_way_p(Node* node, Stack* st)
{
    assert(StackOk(st));
    StackDtor(st);
    StackCtor(st, 8);

    NODE_ASSERT(node)

    StackPush(st, node);
    while (node->parent != nullptr)
    {
        StackPush(st, node->parent);
        node = node->parent;
    }

    return st;
}

Stack* find_way_s(const Tree* tree, const char* word, Stack* st)
{
    assert(StackOk(st));
    StackDtor(st);
    StackCtor(st, 8);

    TREE_ASSERT(tree)
    assert(word != nullptr);

    Node* out_p = nullptr;
    find_word(tree->root, word, &out_p);

    return find_way_p(out_p, st);
}

int print_way_st(Stack* st)
{
    assert(StackOk(st));

    Node* next_node = nullptr;
    Node* cur_node = nullptr;
    while (st->size > 1)
    {
        cur_node  = StackPop(st);
        next_node = StackPop(st);
        StackPush(st, next_node);
        if (cur_node->right == next_node)
            printf("not ");

        printf("%s\n", cur_node->info);
    }

    return 0;
}

int print_way_p(Node* node, Stack* st)
{
    assert(StackOk(st));
    NODE_ASSERT(node)

    find_way_p(node, st);
    print_way_st(st);

    return 0;
}

int print_way_s(const Tree* tree, const char* word, Stack* st)
{
    TREE_ASSERT(tree)
    assert(StackOk(st));
    assert(word != nullptr);

    find_way_s(tree, word, st);
    print_way_st(st);

    return 0;
}

int print_similar_way_st(Stack* st1, Stack* st2)
{
    assert(StackOk(st1));
    assert(StackOk(st2));

    Node* next_node1 = nullptr;
    Node* next_node2 = nullptr;
    Node* cur_node1 = nullptr;
    Node* cur_node2 = nullptr;

    int i = 0;
    while (st1->size > 0 && st2->size > 0)
    {

        cur_node1  = StackPop(st1);
        cur_node2  = StackPop(st2);

        if (st1->size == 0 || st2->size == 0)
            break;

        next_node1 = StackPop(st1);
        //printf("next_node1->info %s\n", next_node1->info);
        StackPush(st1, next_node1);

        next_node2 = StackPop(st2);
        //printf("next_node2->info %s\n", next_node2->info);
        StackPush(st2, next_node2);

        /*printf("%s\n", cur_node1->info);
        printf("%s\n", cur_node2->info);
        printf("%s\n", next_node1->info);
        printf("%s\n", next_node2->info);*/

        if (cur_node1 != cur_node2 || next_node1 != next_node2)
        //|| next_node1 == nullptr || next_node2 == nullptr)
            break;

        if (cur_node1->right == next_node1)
            printf("not ");

        printf("%s\n", cur_node1->info);
        i++;
    }

    if (i == 0)
        printf("nothing\n");

    return 0;
}

int print_different_way_st(Stack* st1, Stack* st2)
{
    assert(StackOk(st1));
    assert(StackOk(st2));

    Node* next_node1 = nullptr;
    Node* next_node2 = nullptr;
    Node* cur_node1 = nullptr;
    Node* cur_node2 = nullptr;

    while (st1->size > 0 && st2->size > 0)
    {

        cur_node1  = StackPop(st1);
        cur_node2  = StackPop(st2);

        if (st1->size == 0 || st2->size == 0)
            break;

        next_node1 = StackPop(st1);
        StackPush(st1, next_node1);

        next_node2 = StackPop(st2);
        StackPush(st2, next_node2);

        if (!(cur_node1 == cur_node2 && next_node1 == next_node2))
        {

            StackPush(st1, cur_node1);
            StackPush(st2, cur_node2);
            break;
        }
    }
    print_way_st(st1);
    printf("\n");
    print_way_st(st2);

    return 0;
}

int print_different_way_p(const Node* node1, const Node* node2, Stack* st1, Stack* st2)
{
    assert(StackOk(st1));
    NODE_ASSERT(node1)
    assert(StackOk(st2));
    NODE_ASSERT(node1)

    find_way_p((Node*)node1, st1);
    find_way_p((Node*)node2, st2);
    print_different_way_st(st1, st2);

    return 0;
}

int print_different_way_s(Tree* tree, char* word1, char* word2, Stack* st1, Stack* st2)
{
    assert(StackOk(st1));
    assert(StackOk(st2));
    TREE_ASSERT(tree)
    assert(word1 != nullptr);
    assert(word2 != nullptr);

    find_way_s(tree, word1, st1);
    find_way_s(tree, word2, st2);
    print_different_way_st(st1, st2);

    return 0;
}

int print_similar_way_p(Node* node1, Node* node2, Stack* st1, Stack* st2)
{
    assert(StackOk(st1));
    NODE_ASSERT(node1)
    assert(StackOk(st2));
    NODE_ASSERT(node1)

    find_way_p(node1, st1);
    find_way_p(node2, st2);
    print_similar_way_st(st1, st2);

    return 0;
}

int print_similar_way_s(Tree* tree, char* word1, char* word2, Stack* st1, Stack* st2)
{
    assert(StackOk(st1));
    assert(StackOk(st2));
    TREE_ASSERT(tree)
    assert(word1 != nullptr);
    assert(word2 != nullptr);

    find_way_s(tree, word1, st1);
    find_way_s(tree, word2, st2);
    print_similar_way_st(st1, st2);

    return 0;
}





int main()
{
    Tree tr = {};
    TreeCtor(&tr, nullptr);

    FILE* fd = fopen("tree.txt", "r");
    read_tree(fd, &tr, tr.root, l_branch);
    fclose(fd);

    Stack st1 = {};
    StackCtor(&st1, 8);
    Stack st2 = {};
    StackCtor(&st2, 8);

    char word1[MaxStr] = "", word2[MaxStr] = "";

    printf("what do you want to do?\n1 - find/add element\n2 - print similar of two elements\n3 - print different of two elements\n4 - print way to element\n");
    int action = 0;
    scanf("%d", &action);

    switch (action)
    {
        case 1:
            find_node(&tr, tr.root);

            fd = fopen("tree.txt", "w");
            print_tree(fd, tr.root, 0);
            //print_tree(stdout, tr.root, 0);
            fclose(fd);
            break;

        case 2:
            printf("enter two words from tree\n");
            scanf("%s", word1);
            scanf("%s", word2);

            printf("\n");
            print_similar_way_s(&tr, word1, word2, &st1, &st2);
            break;

        case 3:
            printf("enter two words from tree\n");
            scanf("%s", word1);
            scanf("%s", word2);

            printf("\n");
            print_different_way_s(&tr, word1, word2, &st1, &st2);
            break;

        case 4:
            printf("enter word from tree\n");
            scanf("%s", word1);

            printf("\n");
            print_way_s(&tr, word1, &st1);
            break;

    }


    TreeDtor(&tr);
    StackDtor(&st1);
    StackDtor(&st2);

    return 0;
}
