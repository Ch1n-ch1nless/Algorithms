#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*==================Constants=================*/

const int           TRUE                =  1;
const int           FALSE               =  0;

const size_t        MAX_SIZE_OF_COMMAND =  7;

const int           POISON_VALUE        = -1000000001;

const int           MODULE              =  1000000007;

const char* const   INSERT_COMMAND      = "insert";
const char* const   DELETE_COMMAND      = "delete";
const char* const   EXISTS_COMMAND      = "exists";
const char* const   NEXT_COMMAND        = "next";
const char* const   PREV_COMMAND        = "prev";
const char* const   KTH_COMMAND         = "kth";

/*===================Structs==================*/

typedef struct node_elem_t
{
    int     key;
    int     priority;
    size_t  size;
} node_elem_t;

typedef struct Node 
{
    node_elem_t     data;
    struct Node*    left;
    struct Node*    right;
} Node;

typedef struct Treap 
{
    Node*   root;
    size_t  number_of_nodes;
} Treap;

typedef struct NodePair
{
    Node*  left_node;
    Node*  right_node;
} NodePair;  

/*==================Functions=================*/

Node*   NodeCtor(int key);
void    NodeDtor(Node* node);

int     GetSize(Node* node);
void    UpdateSize(Node* node);

void    SubTreeSplit(Node* old_subtree, int key, Node** left_subtree, Node** right_subtree);
Node*   SubTreeMerge(Node* left_subtree, Node* right_subtree);
void    SubTreeDtor(Node* subtree_root);

Node*   SubTreeFindNext(Node* node, int key);
Node*   SubTreeFindPrev(Node* node, int key);
Node*   SubTreeFindKthStatistics(Node* node, int k);

Treap*  TreapCtor(void);
void    TreapDtor(Treap* treap);

int     TreapFind(Treap* treap, int key);
void    TreapInsert(Treap* treap, int key);
void    TreapDelete(Treap* treap, int key);

int     TreapFindNext(Treap* treap, int key);
int     TreapFindPrev(Treap* treap, int key);
int     TreapFindKthStatistics(Treap* treap, int k);

void    ExecuteCommands(Treap* treap);

/*============================================*/

int main()
{
    Treap* treap = TreapCtor();

    ExecuteCommands(treap);

    TreapDtor(treap);

    return 0;
}

void ExecuteCommands(Treap* treap)
{
    assert((treap != NULL) && "ERROR!!! Incorrect value of \'treap\'!");

    char    command[MAX_SIZE_OF_COMMAND]    = {};
    int     argument                        = 0;
    int     answer                          = 0;

    while (scanf("%6s %d", command, &argument) != EOF)
    { 
        if (strcmp(command, INSERT_COMMAND) == 0)
        {
            TreapInsert(treap, argument);
        }
        else if (strcmp(command, DELETE_COMMAND) == 0)
        {
            TreapDelete(treap, argument);
        }
        else if (strcmp(command, EXISTS_COMMAND) == 0)
        {
            if (TreapFind(treap, argument) == TRUE)
            {
                printf("true\n");
            }
            else
            {
                printf("false\n");
            }
        }
        else
        {
            if (strcmp(command, NEXT_COMMAND) == 0)
            {
                answer = TreapFindNext(treap, argument);
            }
            else if (strcmp(command, PREV_COMMAND) == 0)
            {
                answer = TreapFindPrev(treap, argument);
            }
            else if (strcmp(command, KTH_COMMAND) == 0)
            {
                answer = TreapFindKthStatistics(treap, argument);
            }
            else
            {
                assert(FALSE && "User entered an invalid command!\n");
            }

            if (answer == POISON_VALUE)
            {
                printf("none\n");
            }
            else
            {
                printf("%d\n", answer);
            }
        }
    }
}

Node* NodeCtor(int key)
{
    Node*   new_node = (Node*) calloc(1, sizeof(Node));
    assert((new_node != NULL) && "ERROR!!! Program can not allocate memory!\n");

    new_node->data.key      = key;
    new_node->data.priority = rand() % MODULE;
    new_node->data.size     = 1;
    
    new_node->left  = NULL;
    new_node->right = NULL;

    return new_node;
}

void NodeDtor(Node* node)
{
    if (node == NULL)
    {
        return;
    }

    node->data.key      = 0;
    node->data.priority = 0;
    node->data.size     = 0;
    
    node->left      = NULL;
    node->right     = NULL;

    free(node);
}

int GetSize(Node* node)
{
    return (node == NULL) ? 0 : node->data.size;
}

void UpdateSize(Node* node)
{
    if (node != NULL)
    {
        node->data.size = 1 + GetSize(node->left) + GetSize(node->right);
    }
}

void SubTreeSplit(Node* old_subtree, int key, Node** left_subtree, Node** right_subtree)
{
    assert((left_subtree  != NULL) && "ERROR!!! Pointer to \'left_subtree\'  is NULL!\n");
    assert((right_subtree != NULL) && "ERROR!!! Pointer to \'right_subtree\' is NULL!\n");

    if (old_subtree == NULL)
    {
        *left_subtree  = NULL;
        *right_subtree = NULL;
    }
    else if (old_subtree->data.key < key)
    {
        SubTreeSplit(old_subtree->right, key, &(old_subtree->right), right_subtree);
        *left_subtree = old_subtree;
    }
    else
    {
        SubTreeSplit(old_subtree->left, key, left_subtree, &(old_subtree->left));
        *right_subtree = old_subtree;
    }

    UpdateSize(old_subtree);
}

Node* SubTreeMerge(Node* left_subtree, Node* right_subtree)
{
    if (left_subtree == NULL || right_subtree == NULL)
    {
        return (left_subtree != NULL) ? left_subtree : right_subtree;
    }

    if (left_subtree->data.priority > right_subtree->data.priority)
    {
        left_subtree->right = SubTreeMerge(left_subtree->right, right_subtree);
        UpdateSize(left_subtree);
        
        return left_subtree;
    }

    right_subtree->left = SubTreeMerge(left_subtree, right_subtree->left);
    UpdateSize(right_subtree);

    return right_subtree;
}

void SubTreeDtor(Node* subtree_root)
{
    if (subtree_root == NULL)
    {
        return;
    }

    SubTreeDtor(subtree_root->left);
    SubTreeDtor(subtree_root->right);
    NodeDtor(subtree_root);
}

Node* SubTreeFindNext(Node* node, int key)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (node->data.key <= key)
    {
        return SubTreeFindNext(node->right, key);
    }
    else
    {
        Node* next_key_node = SubTreeFindNext(node->left, key);

        if (next_key_node != NULL)
        {
            return next_key_node;
        }

        return node;
    }
}

Node* SubTreeFindPrev(Node* node, int key)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (node->data.key >= key)
    {
        return SubTreeFindPrev(node->left, key);
    }
    else
    {
        Node* next_key_node = SubTreeFindPrev(node->right, key);

        if (next_key_node != NULL)
        {
            return next_key_node;
        }

        return node;
    }
}

Node* SubTreeFindKthStatistics(Node* node, int k)
{
    if (node == NULL)
    {
        return NULL;
    }

    size_t left_size = GetSize(node->left);

    if (k == left_size)
    {
        return node;
    }
    else if (k < left_size)
    {
        return SubTreeFindKthStatistics(node->left, k);
    }

    return SubTreeFindKthStatistics(node->right, k - left_size - 1);
}

Treap* TreapCtor(void)
{
    Treap* new_treap = (Treap*) calloc(1, sizeof(Treap));

    new_treap->root             = NULL;
    new_treap->number_of_nodes  = 0;

    return new_treap;
}

void TreapDtor(Treap* treap)
{
    assert((treap != NULL) && "ERROR!!! Pointer to \'treap\' is NULL!");

    SubTreeDtor(treap->root);
    
    treap->root             = NULL;
    treap->number_of_nodes  = 0;

    free(treap);
}

int TreapFind(Treap* treap, int key)
{
    assert((treap != NULL) && "ERROR!!! Pointer to \'treap\' is NULL!\n");

    Node* key_node = treap->root;

    while (key_node != NULL)
    {
        if (key_node->data.key == key)
        {
            break;
        }
        else if (key_node->data.key < key)
        {
            key_node = key_node->right;
        }
        else
        {
            key_node = key_node->left;
        }
    }

    return (key_node != NULL) ? TRUE : FALSE;
}

void TreapInsert(Treap* treap, int key)
{
    assert((treap != NULL) && "ERROR!!! Pointer to \'treap\' is NULL!\n");

    treap->number_of_nodes += 1;

    Node* new_node = NodeCtor(key);

    if (treap->root == NULL)
    {
        treap->root = new_node;
        return;
    }

    if (TreapFind(treap, key) == FALSE)
    {
        Node* left_tree  = NULL;
        Node* right_tree = NULL;

        SubTreeSplit(treap->root, key, &left_tree, &right_tree);

        left_tree   = SubTreeMerge(left_tree, new_node);
        treap->root = SubTreeMerge(left_tree, right_tree);
    }
    else
    {
        treap->number_of_nodes -= 1;
        NodeDtor(new_node);
    }
}

void TreapDelete(Treap* treap, int key)
{
    assert((treap != NULL) && "ERROR!!! Pointer to \'treap\' is NULL!\n");

    treap->number_of_nodes -= 1;

    if (TreapFind(treap, key) == TRUE)
    {
        Node* left_tree  = NULL;
        Node* right_tree = NULL;
        Node* key_node   = NULL;

        SubTreeSplit(treap->root, key, &left_tree, &key_node);
        SubTreeSplit(key_node, key + 1, &key_node, &right_tree);

        NodeDtor(key_node);

        treap->root = SubTreeMerge(left_tree, right_tree);
    }
    else
    {
        treap->number_of_nodes += 1;
    }
}

int TreapFindNext(Treap* treap, int key)
{
    assert((treap != NULL) && "ERROR!!! Pointer to \'treap\' is NULL!\n");

    Node* next_key_node = SubTreeFindNext(treap->root, key);

    return (next_key_node != NULL) ? next_key_node->data.key : POISON_VALUE;
}

int TreapFindPrev(Treap* treap, int key)
{
    assert((treap != NULL) && "ERROR!!! Pointer to \'treap\' is NULL!\n");

    Node* prev_key_node = SubTreeFindPrev(treap->root, key);

    return (prev_key_node != NULL) ? prev_key_node->data.key : POISON_VALUE;
}

int TreapFindKthStatistics(Treap* treap, int k)
{
    assert((treap != NULL) && "ERROR!!! Pointer to \'treap\' is NULL!\n");

    Node* key_node = SubTreeFindKthStatistics(treap->root, k);

    return (key_node != NULL) ? key_node->data.key : POISON_VALUE;
}