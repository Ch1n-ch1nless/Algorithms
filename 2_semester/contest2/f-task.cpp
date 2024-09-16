#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*==================Constants=================*/

const int       TRUE            = 1;
const int       FALSE           = 0;

/*===================Structs==================*/

typedef struct node_elem_t
{
    int     key;
    int     priority;
    size_t  index;
} node_elem_t;

typedef struct Node 
{
    node_elem_t     data;
    struct Node*    left;
    struct Node*    right;
    struct Node*    parent;
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

Node*       NodeCtor(node_elem_t data);
void        NodeDtor(Node* node);

void        SubTreeDtor(Node* subtree_root);

Treap*      TreapCtor(void);
void        TreapDtor(Treap* treap);

Node*       TreapInsert(Node* last_node, Node* new_node);
void        BuildTreapByLinearTime(Treap* treap, node_elem_t* array, size_t size);

void        PrintNodeData(Node* node);
void        PrintTreapInOrder(Node* node);

/*============================================*/

int main()
{
    Treap* treap = TreapCtor();

    size_t size = 0;

    if (scanf("%lu", &size) == 0)
    {
        assert(FALSE && "ERROR!!! Program can not read the number!\n");
    }

    node_elem_t* array = (node_elem_t*) calloc(size, sizeof(node_elem_t));
    assert((array != NULL) && "ERROR!!! Program can not allocate memory!\n");

    int key         = 0;
    int priority    = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (scanf("%d %d", &key, &priority) == 0)
        {
            assert(FALSE && "ERROR!!! Program can not read the numbers!\n");
        }

        array[i].key        = key;
        array[i].priority   = priority;
        array[i].index      = i + 1;
    }

    BuildTreapByLinearTime(treap, array, size);

    printf("YES\n");
    PrintTreapInOrder(treap->root);

    TreapDtor(treap);

    return 0;
}

Node* NodeCtor(node_elem_t data)
{
    Node*   new_node = (Node*) calloc(1, sizeof(Node));
    assert((new_node != NULL) && "ERROR!!! Program can not allocate memory!\n");

    new_node->data      = data;
    new_node->left      = NULL;
    new_node->right     = NULL;
    new_node->parent    = NULL;

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
    node->data.index    = 0;
    node->left      = NULL;
    node->right     = NULL;
    node->parent    = NULL;

    free(node);
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

Node* TreapInsert(Node* last_node, Node* new_node)
{
    assert((new_node != NULL) && "Incorrect value of node");

    if (last_node == NULL)
    {
        return new_node;
    }

    Node* parent_of_new_node = last_node;

    while (parent_of_new_node->data.priority >= new_node->data.priority)
    {
        if (parent_of_new_node->parent == NULL)
        {
            new_node->left              = parent_of_new_node;
            parent_of_new_node->parent  = new_node;

            return new_node;
        }

        parent_of_new_node = parent_of_new_node->parent;
    }

    if (parent_of_new_node->right == NULL)
    {
        parent_of_new_node->right   = new_node;
        new_node->parent            = parent_of_new_node;

        return new_node;
    }

    new_node->parent                    = parent_of_new_node;
    new_node->left                      = parent_of_new_node->right;
    parent_of_new_node->right->parent   = new_node;

    parent_of_new_node->right           = new_node;

    return new_node;
    
}

void BuildTreapByLinearTime(Treap* treap, node_elem_t* array, size_t size)
{
    assert((treap   != NULL) && "ERROR!!! Pointer to \'treap\' is NULL!");
    assert((array   != NULL) && "ERROR!!! Pointer to \'array\' is NULL");
    
    treap->number_of_nodes = size;

    Node* last_node = treap->root;
    Node* new_node  = NULL;

    for (size_t i = 0; i < size; i++)
    {
        new_node  = NodeCtor(array[i]);
        last_node = TreapInsert(last_node, new_node);
    }

    while (last_node->parent != NULL)
    {
        last_node = last_node->parent;
    }
    
    treap->root = last_node;
}

void PrintNodeData(Node* node)
{
    assert((node != NULL) && "ERROR!!! Pointer to \'node\' is NULL!");

    if (node->parent == NULL)
    {
        printf("%d ", 0);
    } 
    else
    {
        printf("%lu ", node->parent->data.index);
    }

    if (node->left == NULL)
    {
        printf("%d ", 0);
    } 
    else
    {
        printf("%lu ", node->left->data.index);
    } 

    if (node->right == NULL)
    {
        printf("%d\n", 0);
    } 
    else
    {
        printf("%lu\n", node->right->data.index);
    }
}

void PrintTreapInOrder(Node* node)
{
    if (node->left != NULL)
    {
        PrintTreapInOrder(node->left);
    }

    PrintNodeData(node);

    if (node->right != NULL)
    {
        PrintTreapInOrder(node->right);
    }
}
