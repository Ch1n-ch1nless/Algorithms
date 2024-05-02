#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*==================Constants=================*/

const int   TRUE    = 1;
const int   FALSE   = 0;

/*===================Structs==================*/

typedef struct 
{
    char*   str1;
    char*   str2;
} key_t;

const key_t POISON_VALUE = {NULL, NULL};

typedef struct Node
{
    key_t           key;
    struct Node*    left_child;
    struct Node*    right_child;
    struct Node*    parent;
} Node;

typedef struct SplayTree
{
    Node*   root;
    size_t  number_of_nodes;
} SplayTree;

/*==================Functions=================*/

//vv~~~~~~~~~~~~Secondary functions~~~~~~~~~~~vv

Node*   NodeCtor(key_t key);
void    NodeDtor(Node* node);

Node*   GetParent(     Node* node);
Node*   GetGrandParent(Node* node);

int     IsLeftChild( Node* node);
int     IsRightChild(Node* node);

void    RotateRight(Node* parent);
void    RotateLeft( Node* parent);

void    Zig(   Node* node);
void    ZigZig(Node* node);
void    ZigZag(Node* node);

Node*   SubTreeSplay( Node* root, key_t key);
Node*   SubTreeSearch(Node* root, key_t key);
Node*   SubTreeInsert(Node* root, key_t key);
void    SubTreeDtor(  Node* root);

//^^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^^

//vv~~~~~~~~~~~~SplayTree functions~~~~~~~~~~~vv

SplayTree*  SplayTreeCtor(void);
void        SplayTreeDtor(SplayTree* splay_tree);

void        SplayTreeSearch(SplayTree* splay_tree, key_t key);
void        SplayTreeInsert(SplayTree* splay_tree, key_t key);

//^^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^^

void        AddStormtroopersToSplayTree( SplayTree* splay_tree, size_t number);
void        FindStormtroopersInSplayTree(SplayTree* splay_tree, size_t number);

/*============================================*/

int main()
{
    size_t N = 0;
    size_t Q = 0;

    if (scanf("%d", &N) == 0)
    {
        assert(FALSE && "ERROR!!! Program can not read the number!\n");
    }

    SplayTree* splay_tree = SplayTreeCtor();

    AddStormtroopersToSplayTree(splay_tree, N);

    if (scanf("%d", &Q) == 0)
    {
        assert(FALSE && "ERROR!!! Program can not read the number!\n");
    }

    FindStormtroopersInSplayTree(splay_tree, Q);

    SplayTreeDtor(splay_tree);

    return 0;
}

void AddStormtroopersToSplayTree(SplayTree* splay_tree, size_t number)
{
    assert((splay_tree != NULL) && "ERROR!!! Pointer to \'splay_tree\' is NULL!\n");

    for (size_t i = 0; i < number; i++)
    {
        key_t* key = (key_t*) calloc(1, sizeof(key_t));
    }
}

Node* NodeCtor(key_t key)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    assert((new_node != NULL) && "ERROR!!! Program can not allocate memory!\n");

    new_node->key           = key;
    new_node->left_child    = NULL;
    new_node->right_child   = NULL;
    new_node->parent        = NULL;

    return new_node;
}

void NodeDtor(Node* node)
{
    assert((node != NULL) && "ERROR!!! Pointer to \'node\' is NULL!\n");

    node->key           = POISON_VALUE;
    node->left_child    = NULL;
    node->right_child   = NULL;
    node->parent        = NULL; 

    free(node);     
}

Node* GetParent(Node* node)
{
    if (node != NULL)
    {
        return node->parent;
    }

    return NULL;
}

Node* GetGrandParent(Node* node)
{
    if (node != NULL)
    {
        if (node->parent != NULL)
        {
            return node->parent->parent;
        }
    }

    return NULL;
}

int IsLeftChild( Node* node)
{
    assert((node         != NULL) && "ERROR!!! Pointer to \'node\'         is NULL!\n");
    assert((node->parent != NULL) && "ERROR!!! Pointer to \'node->parent\' is NULL!\n");

    if (node == node->parent->left_child)
    {
        return TRUE;
    }

    return FALSE;
}

int IsRightChild(Node* node)
{
    assert((node         != NULL) && "ERROR!!! Pointer to \'node\'         is NULL!\n");
    assert((node->parent != NULL) && "ERROR!!! Pointer to \'node->parent\' is NULL!\n");

    if (node == node->parent->right_child)
    {
        return TRUE;
    }

    return FALSE;
}

void RotateRight(Node* parent)
{
    assert((parent != NULL) && "ERROR!!! Pointer to \'parent\' is NULL!\n");

    Node* left          = parent->left_child;
    Node* grand_parent  = parent->parent;

    int is_root = (grand_parent != NULL);
    int is_left = (is_root) && (grand_parent->left_child == parent);  

    parent->left_child = left->right_child;

    if (left->right_child != NULL)
    {
        left->right_child->parent = parent;
    }

    left->right_child = parent;
    parent->parent    = left;
    left->parent      = grand_parent;

    if (is_root && is_left)
    {
        grand_parent->left_child = left;
    }
    else if (is_root)
    {
        grand_parent->right_child = left;
    }
}

void RotateLeft(Node* parent)
{
    assert((parent != NULL) && "ERROR!!! Pointer to \'parent\' is NULL!\n");

    Node* right          = parent->right_child;
    Node* grand_parent  = parent->parent;

    int is_root = (grand_parent != NULL);
    int is_right = (is_root) && (grand_parent->right_child == parent);  

    parent->right_child = right->left_child;

    if (right->left_child != NULL)
    {
        right->left_child->parent = parent;
    }

    right->left_child = parent;
    parent->parent    = right;
    right->parent      = grand_parent;

    if (is_root && is_right)
    {
        grand_parent->right_child = right;
    }
    else if (is_root)
    {
        grand_parent->left_child = right;
    }
}

void Zig(Node* node)
{
    if (IsLeftChild(node))
    {   
        RotateRight(GetParent(node));
    }
    else
    {
        RotateLeft(GetParent(node));
    }
}

void ZigZig(Node* node)
{
    if (IsLeftChild(node)) 
    {
      RotateRight(GetGrandParent(node));
      RotateRight(GetParent(node));
    } 
    else 
    {
      RotateLeft(GetGrandParent(node));
      RotateLeft(GetParent(node));
    }
}

void ZigZag(Node* node)
{
    if (IsRightChild(node)) 
    {
      RotateLeft(GetParent(node));
      RotateRight(GetGrandParent(node));
    } 
    else 
    {
      RotateRight(GetParent(node));
      RotateLeft(GetGrandParent(node));
    }
}

Node* SubTreeSplay(Node* root, key_t key)
{
    while (GetParent(root) != NULL)
    {
        if (GetGrandParent(root) == NULL)
        {
            Zig(root);
        }
        else if ((IsLeftChild(root) && IsLeftChild(GetParent(root)))  ||
                 (IsRightChild(root) && IsRightChild(GetParent(root)))   )
        {
            ZigZig(root);
        }
        else if ((IsRightChild(root) && IsLeftChild(GetParent(root))) ||
                 (IsLeftChild(root) && IsRightChild(GetParent(root)))   ) 
        {
            ZigZag(root);
        }
    }

    return root;
}

Node* SubTreeSearch(Node* root, key_t key)
{
    return SubTreeSplay(root, key);
}

Node* SubTreeInsert(Node* root, key_t key)
{
    Node* new_node = NodeCtor(key);

    if (root == NULL)
    {
        return new_node;
    }

    Node* prev = NULL;
    Node* next = NULL;

    int is_left_child = FALSE;

    while (root != NULL)
    {
        if (key <= root->key)
        {
            next = root->left_child;
            is_left_child = TRUE;
        }
        else
        {
            next = root->right_child;
            is_left_child = FALSE;
        }

        prev = root;
        root = next;
    }

    if (is_left_child)
    {
        prev->left_child    = new_node;
        new_node->parent    = prev;
    }
    else
    {
        prev->right_child    = new_node;
        new_node->parent    = prev;
    }

    SubTreeSplay(new_node, key);

    return new_node;    
}

void SubTreeDtor(Node* root)
{
    if (root == NULL)
    {
        return;
    }

    SubTreeDtor(root->left_child);
    SubTreeDtor(root->right_child);
    NodeDtor(root);
}



SplayTree*  SplayTreeCtor(void)
{
    SplayTree* splay_tree = (SplayTree*) calloc(1, sizeof(SplayTree));
    assert((splay_tree != NULL) && "ERROR!!! Program can not allocate memory!\n");

    splay_tree->number_of_nodes = 0;
    splay_tree->root            = NULL;

    return splay_tree;
}

void SplayTreeDtor(SplayTree* splay_tree)
{
    assert((splay_tree != NULL) && "ERROR!!! Pointer to \'splay_tree\' is NULL!\n");

    SubTreeDtor(splay_tree->root);

    splay_tree->number_of_nodes = 0;
    splay_tree->root            = NULL;

    free(splay_tree);
}

void SplayTreeSearch(SplayTree* splay_tree, key_t key)
{
    assert((splay_tree != NULL) && "ERROR!!! Pointer to \'splay_tree\' is NULL!\n");

    Node* key_node = SubTreeSearch(splay_tree->root, key);

    if (key_node != NULL && key_node->key == key)
    {
        return;
    }
}

void SplayTreeInsert(SplayTree* splay_tree, key_t key)
{
    assert((splay_tree != NULL) && "ERROR!!! Pointer to \'splay_tree\' is NULL!\n");

    splay_tree->root             = SubTreeInsert(splay_tree->root, key);
    splay_tree->number_of_nodes += 1;
}
