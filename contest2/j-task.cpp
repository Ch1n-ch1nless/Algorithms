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

void        SubTreeSplit(Node* old_subtree, int key, Node** left_subtree, Node** right_subtree);
Node*       SubTreeMerge(Node* left_subtree, Node* right_subtree);
void        SubTreeDtor(Node* subtree_root);

long long   SubTreeGetSum(Node* subtree_root, long long l, long long r);

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
    char        prev_cmd    = '\0';
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

                if (prev_cmd == '?')
                {
                    new_key = (new_key + answer) % MODULE;
                }

                TreapInsert(treap, new_key);
                break;

            case '?':
                if (scanf("%lld", &left) == 0)
                {
                    assert(FALSE && "ERROR!!! Program can not read the number!\n");
                }

                if (scanf("%lld", &right) == 0)
                {
                    assert(FALSE && "ERROR!!! Program can not read the number!\n");
                }

                answer = TreapGetSum(treap, left, right);

                printf("%lld\n", answer);

                break;
            
            default:
                assert(FALSE && "ERROR!!! Incorrect value of command!\n");
                break;
        }

        prev_cmd = command;
    }
}

Node* NodeCtor(long long key)
{
    Node*   new_node = (Node*) calloc(1, sizeof(Node));
    assert((new_node != NULL) && "ERROR!!! Program can not allocate memory!\n");

    new_node->data.key      = key;
    new_node->data.priority = rand();
    
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
        
        return left_subtree;
    }

    right_subtree->left = SubTreeMerge(left_subtree, right_subtree->left);

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

long long SubTreeGetSum(Node* subtree_root, long long l, long long r)
{
    if (subtree_root == NULL)
    {
        return 0;
    }

    if (r < subtree_root->data.key)
    {
        return SubTreeGetSum(subtree_root->left, l, r);
    }
    else if (l > subtree_root->data.key)
    {
        return SubTreeGetSum(subtree_root->right, l, r);
    }
    else
    {
        return subtree_root->data.key + SubTreeGetSum(subtree_root->left, l, r) + SubTreeGetSum(subtree_root->right, l, r);
    }
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


long long TreapGetSum(Treap* treap, long long left, long long right)
{
    return SubTreeGetSum(treap->root, left, right);
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

        NodeDtor(key_node);

        treap->root = SubTreeMerge(left_tree, right_tree);
    }
    else
    {
        treap->number_of_nodes += 1;
    }
}