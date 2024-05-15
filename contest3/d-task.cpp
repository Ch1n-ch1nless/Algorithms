#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*==================Constants=================*/

const int           TRUE                =  1;
const int           FALSE               =  0;

const int           POISON_VALUE        = -1000000001;

const int           MODULE              =  1000000000;

/*===================Structs==================*/

typedef struct node_elem_t
{
    long long   key;
    long long   priority;
    long long   sum;
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

Node*       NodeCtor(long long key);
void        NodeDtor(Node* node);

long long   GetSum(Node* node);
void        UpdateSum(Node* node);

void        SubTreeSplit(Node* old_subtree, int key, Node** left_subtree, Node** right_subtree);
Node*       SubTreeMerge(Node* left_subtree, Node* right_subtree);
void        SubTreeDtor(Node* subtree_root);

Treap*      TreapCtor(void);
void        TreapDtor(Treap* treap);

int         TreapFind(Treap* treap, long long key);
void        TreapInsert(Treap* treap, long long key);
long long   TreapGetSum(Treap* treap, long long left, long long right);
void        TreapDelete(Treap* treap, long long key);

void    ExecuteCommands(Treap* treap);

/*============================================*/

int main()
{
    srand(time(NULL));

    Treap* treap = TreapCtor();

    ExecuteCommands(treap);

    TreapDtor(treap);

    return 0;
}

void ExecuteCommands(Treap* treap)
{
    assert((treap != NULL) && "ERROR!!! Incorrect value of \'treap\'!");

    size_t number_of_commands = 0;

    if (scanf("%lu", &number_of_commands) == 0)
    {
        assert(FALSE && "ERROR!!! Program can not read the number!\n");
    }

    char        command     = '\0';
    long long   left        = 0;
    long long   right       = 0;
    long long   new_key     = 0;

    long long   answer      = 0;

    for (size_t i = 0; i < number_of_commands; i++)
    {
        if (scanf(" %c", &command) == 0)
        {
            assert(FALSE && "ERROR!!! Program can not read the number!\n");
        }

        switch (command)
        {
            case '+':
                if (scanf("%lld", &new_key) == 0)
                {
                    assert(FALSE && "ERROR!!! Program can not read the number!\n");
                }

                TreapInsert(treap, new_key);
                break;

            case '?':
                if (scanf("%lld", &right) == 0)
                {
                    assert(FALSE && "ERROR!!! Program can not read the number!\n");
                }

                answer = TreapGetSum(treap, 0, right);

                printf("%lld\n", answer);

                break;
            
            default:
                assert(FALSE && "ERROR!!! Incorrect value of command!\n");
                break;
        }
    }
}

Node* NodeCtor(long long key)
{
    Node*   new_node = (Node*) calloc(1, sizeof(Node));
    assert((new_node != NULL) && "ERROR!!! Program can not allocate memory!\n");

    new_node->data.key      = key;
    new_node->data.priority = rand();
    new_node->data.sum      = key;
    
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
    
    node->left      = NULL;
    node->right     = NULL;

    free(node);
}

long long GetSum(Node* node)
{
    return (node == NULL) ? 0 : node->data.sum;
}

void UpdateSum(Node* node)
{
    if (node != NULL)
    {
        node->data.sum  = node->data.key + GetSum(node->left) + GetSum(node->right);
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

    UpdateSum(*left_subtree);
    UpdateSum(*right_subtree);
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
        UpdateSum(left_subtree);
        UpdateSum(right_subtree);

        return left_subtree;
    }

    right_subtree->left = SubTreeMerge(left_subtree, right_subtree->left);
    UpdateSum(right_subtree);
    UpdateSum(left_subtree);

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

int TreapFind(Treap* treap, long long key)
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

void TreapInsert(Treap* treap, long long key)
{
    assert((treap != NULL) && "ERROR!!! Pointer to \'treap\' is NULL!\n");

    treap->number_of_nodes += 1;

    Node* new_node = NodeCtor(key);

    if (treap->root == NULL)
    {
        treap->root = new_node;
        return;
    }

    Node* left_tree  = NULL;
    Node* right_tree = NULL;

    SubTreeSplit(treap->root, key, &left_tree, &right_tree);

    left_tree   = SubTreeMerge(left_tree, new_node);
    treap->root = SubTreeMerge(left_tree, right_tree);
}


long long TreapGetSum(Treap* treap, long long left, long long right)
{
    Node*   left_subtree    = NULL; // subtree, in which all keys of nodes < left
    Node*   sum_subtree     = NULL; // subtree, in which all keys of nodes \in [left, right]
    Node*   right_subtree   = NULL; // subtree, in which all keys of nodes > right

    SubTreeSplit(treap->root, right + 1, &sum_subtree,  &right_subtree);
    SubTreeSplit(sum_subtree, left,      &left_subtree, &sum_subtree);

    long long sum = GetSum(sum_subtree);

    treap->root = SubTreeMerge(SubTreeMerge(left_subtree, sum_subtree), right_subtree);

    return sum;
}

void TreapDelete(Treap* treap, long long key)
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

        Node* new_key_node = SubTreeMerge(key_node->left, key_node->right);
        NodeDtor(key_node);

        treap->root = SubTreeMerge(left_tree, SubTreeMerge(new_key_node, right_tree));
    }
    else
    {
        treap->number_of_nodes += 1;
    }
}