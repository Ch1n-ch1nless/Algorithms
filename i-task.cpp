#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*==================Constants=================*/

#define MAX_SIZE 1000

/*===================Structs==================*/

typedef struct Node
{
    char*           key1;
    char*           key2;
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

/**
 * @brief Функция, которая создаёт узел дерева с 2-мя ключами key1 и key2
 * 
 * @param [in] key1 - ключ, который попадёт в key1
 * 
 * @param [in] key2 - ключ, который попадёт в key2
 * 
 * @return узел, с 2-мя ключами
*/
Node*   NodeCtor(char* key1, char* key2);

/**
 * @brief Функция, которая удаляет узел дерева из памяти 
 * 
 * @param [in] node - узел, который надо удалить
*/
void    NodeDtor(Node* node);

/**
 * @brief Функция, которая возвращает указатель на узел родителя
 * 
 * @param [in] node - указатель на узел 
 * 
 * @return указатель на родителя узла
*/
Node*   GetParent(     Node* node);

/**
 * @brief Функция, которая возвращает указатель на дедушку (родителя родителя) узла 
 * 
 * @param [in] node - указатель на узел 
 * 
 * @return указатель на дедушку (родителя родителя) узла
*/
Node*   GetGrandParent(Node* node);

/**
 * @brief Функция, которая проверяет, является ли текущий узел левым ребенком родителя узла
 * 
 * @param [in] node - указатель на узел
 * 
 * @return true, если узел - это левый ребенок родителя, иначе false
*/
bool    IsLeftChild( Node* node);

/**
 * @brief Функция, которая проверяет, является ли текущий узел правым ребенком родителя узла
 * 
 * @param [in] node - указатель на узел
 * 
 * @return true, если узел - это правый ребенок родителя, иначе false
*/
bool    IsRightChild(Node* node);

/**
 * @brief Разворот дерева:
 * 
 *  P - parent
 *  X - левый ребенок           
 *                     
 *            P           X             
 * rotate -> / \         / \   
 *          X   C   =>  A   P           
 *         / \             / \          
 *        A   B           B   C
 * 
 * @param [in] parent - узел родителя
*/
void    RotateRight(Node* parent);

/**
 * @brief Разворот дерева:
 * 
 * P - parent
 * X - правый ребенок
 * 
 *   P                     X
 *  / \  <- rotate        / \
 * A   X            =>   P   C
 *    / \               / \  
 *   B   C             A   B
 * 
 * @param [in] parent - узел родителя
*/
void    RotateLeft( Node* parent);

/**
 * @brief Функция делает следующее действие:
 *                  
 *            Y            X             
 * rotate -> / \          / \ <- rotate  
 *          X   C   <=>  A   Y           
 *         / \              / \          
 *        A   B            B   C  
 * 
 * @param [in] node - указатель на узел X       
*/
void    Zig(   Node* node);

/**
 * @brief Функция делают следующие действия:
 * 
 *                   1)                    2)                
 *             Z                  Y                 X        
 * rotate ->  / \    rotate ->  /   \       =>     / \       
 *           Y   D     =>      X     Z            A   Y      
 *          / \               / \   /  \             / \     
 *         X   C             A   B C    D           B   Z    
 *        / \                                          / \   
 *       A   B         <=                   <=        C   D
 * 
 * @param [in] node - указатель на узел X  
*/
void    ZigZig(Node* node);

/**
 * @brief Функция делает следующие действия:
 * 
 *               1)                    2)                    
 *       Z                    Z                   X          
 *      / \       rotate ->  / \                /   \        
 *     Y   D                X   D      =>      Y     Z       
 *    / \  <- rotate       / \                / \   / \      
 *   A   X                Y   C              A   B C   D     
 *      / \      =>      / \                                 
 *     B   C            A   B    
 * 
 * @param [in] node - указатель на узел X                            
*/
void    ZigZag(Node* node);

/**
 * @brief Функция, которая текущий узел возвращает в корень дерева
 * 
 * @param [in] root - указатель на новый корень дерева
*/
Node*   SubTreeSplay( Node* root);

/**
 * @brief Функция, которая в поддереве ищет ключ key
 * 
 * @param [in] root - указатель на корень поддерева
 * 
 * @param [in] key - ключ, по которому происходит поиск
 * 
 * @return указатель на узел, который содержит key
*/
Node*   SubTreeSearch(Node* root, char* key);

/**
 * @brief Функция, которая вставляет узел в поддерево
 * 
 * @param [in] root - указатель на корень поддерева
 * 
 * @param [in] key1 - ключ под номером 1
 * 
 * @param [in] key2 - ключ под номером 2
 * 
 * @return указатель на корень поддерева с добавленным узлом
*/
Node*   SubTreeInsert(Node* root, char* key1, char* key2);

/**
 * @brief Функция, которая удаляет поддерево
 * 
 * @param [in] root - указатель на корень поддерева 
*/
void    SubTreeDtor(  Node* root);

//^^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^^

//vv~~~~~~~~~~~~SplayTree functions~~~~~~~~~~~vv

/**
 * @brief Функция, которая создаёт splay-дерево
 * 
 * @return указатель на структуру splay-дерева
*/
SplayTree*  SplayTreeCtor(void);

/**
 * @brief Функция, которая удаляет splay-дерево
 * 
 * @param [in] splay_tree - указатель на splay-дерево
*/
void        SplayTreeDtor(SplayTree* splay_tree);

/**
 * @brief Функция, которая ищет в splay-дереве узел с ключом key и возвращает его второй ключ
 * 
 * @param [in] splay_tree - указатель на splay-дерево
 * 
 * @param [in] key - ключ, по которому происходит поиск
 * 
 * @return второй ключ в искомом узел
*/
char*       SplayTreeSearch(SplayTree* splay_tree, char* key);

/**
 * @brief Функция, которая добавляет в splay-дерево узел с парой ключей (key1, key2)
 * 
 * @param [in] splay_tree - указатель на splay-дерево
 * 
 * @param [in] key1 - ключ под номером 1
 * 
 * @param [in] key2 - ключ под номером 2
*/
void        SplayTreeInsert(SplayTree* splay_tree, char* key1, char* key2);

//^^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^^

/**
 * @brief Функция, которая читает массив ключей и вставляет его в splay-дерево 
 * 
 * @param [in] splay_tree - указатель на splay-дерево
 * 
 * @param [in] key_buffer - массив ключей
 * 
 * @param [in] number - количество ключей в массиве
*/
void        AddStormtroopersToSplayTree( SplayTree* splay_tree, char** key_buffer, size_t number);

/**
 * @brief Функция, которая читает ключи, введенные пользователем и ищет их в splay-дереве
 * 
 * @param [in] splay_tree - указатель на splay-дерево
 * 
 * @param [in] number - число запросов
*/
void        FindStormtroopersInSplayTree(SplayTree* splay_tree, size_t number);

/**
 * @brief Функция, которая создаёт массив ключей размером number_of_keys
 * 
 * @param [in] number_of_keys - размер массива ключей
 * 
 * @return указатель на массив ключей
*/
char**      KeyBufferBuild(size_t number_of_keys);

/**
 * @brief Функция, которая удаляет массив ключей из памяти
 * 
 * @param [in] key_buffer - указатель на массив ключей
 * 
 * @param [in] number_of_keys - размер массива ключей
*/
void        KeyBufferDelete(char** key_buffer, size_t number_of_keys);

/*============================================*/

int main()
{
    size_t N = 0;
    size_t Q = 0;

    if (scanf("%lu", &N) == 0)
    {
        assert(false && "ERROR!!! Program can not read the number!\n");
    }

    SplayTree* splay_tree = SplayTreeCtor();

    char** key_buffer     = KeyBufferBuild(N * 2);

    AddStormtroopersToSplayTree(splay_tree, key_buffer, N);

    if (scanf("%lu", &Q) == 0)
    {
        assert(false && "ERROR!!! Program can not read the number!\n");
    }

    FindStormtroopersInSplayTree(splay_tree, Q);

    KeyBufferDelete(key_buffer, 2 * N);

    SplayTreeDtor(splay_tree);

    return 0;
}

void AddStormtroopersToSplayTree( SplayTree* splay_tree, char** key_buffer, size_t number)
{
    assert((splay_tree != NULL) && "ERROR!!! Pointer to \'splay_tree\' is NULL!\n");

    char key1[MAX_SIZE] = {};
    char key2[MAX_SIZE] = {};

    for (size_t i = 0; i < number; i++)
    {
        if (scanf("%999s", key_buffer[2*i]) == 0)
        {
            assert(false && "ERROR!!! Program can not read the string!\n");
        }

        if (scanf("%999s", key_buffer[2*i+1]) == 0)
        {
            assert(false && "ERROR!!! Program can not read the string!\n");
        }

        SplayTreeInsert(splay_tree, key_buffer[2*i], key_buffer[2*i+1]);
    }
}

void FindStormtroopersInSplayTree(SplayTree* splay_tree, size_t number)
{
    assert((splay_tree != NULL) && "ERROR!!! Pointer to \'splay_tree\' is NULL!\n");

    char    key[MAX_SIZE]   = {};
    char*   answer          = NULL;

    for (size_t i = 0; i < number; i++)
    {
        if (scanf("%999s", key) == 0)
        {
            assert(false && "ERROR!!! Program can not read the string!\n");
        }

        answer = SplayTreeSearch(splay_tree, key);

        if (answer == NULL)
        {
            assert(false && "ERROR!!! Program can not find the key in splay_tree");
        }
        else
        {
            printf("%s\n", answer);
        }
    }
}

char** KeyBufferBuild(size_t number_of_keys)
{
    char** new_key_buffer   = (char**) calloc(number_of_keys, sizeof(char*));\
    assert((new_key_buffer != NULL) && "ERROR!!! Program can not allocate memory!\n");

    for (size_t i = 0; i < number_of_keys; i++)
    {
        new_key_buffer[i] = (char*) calloc(MAX_SIZE, sizeof(char));
        assert((new_key_buffer[i] != NULL) && "ERROR!!! Program can not allocate memory!\n");
    }

    return new_key_buffer;
}

void KeyBufferDelete(char** key_buffer, size_t number_of_keys)
{
    assert((key_buffer != NULL) && "ERROR!!! Pointer to \'key_buffer\' is NULL!\n");

    for (size_t i = 0; i < number_of_keys; i++)
    {
        free(key_buffer[i]);
        key_buffer[i] = NULL;
    }

    free(key_buffer);
}


Node* NodeCtor(char* key1, char* key2)
{
    assert((key1 != NULL) && "ERROR!!! Pointer to \'key1\' is NULL!\n");
    assert((key2 != NULL) && "ERROR!!! Pointer to \'key2\' is NULL!\n");

    Node* new_node = (Node*) calloc(1, sizeof(Node));
    assert((new_node != NULL) && "ERROR!!! Pointer to \'new_node\' is NULL!\n");

    new_node->key1          = key1;
    new_node->key2          = key2;

    new_node->left_child    = NULL;
    new_node->right_child   = NULL;
    new_node->parent        = NULL;

    return new_node;
}

void NodeDtor(Node* node)
{
    assert((node != NULL) && "ERROR!!! Pointer to \'node\' is NULL!\n");

    //Null the lines
    node->key1          = NULL;
    node->key2          = NULL;

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

bool IsLeftChild( Node* node)
{
    assert((node         != NULL) && "ERROR!!! Pointer to \'node\'         is NULL!\n");
    assert((node->parent != NULL) && "ERROR!!! Pointer to \'node->parent\' is NULL!\n");

    if (node == node->parent->left_child)
    {
        return true;
    }

    return false;
}

bool IsRightChild(Node* node)
{
    assert((node         != NULL) && "ERROR!!! Pointer to \'node\'         is NULL!\n");
    assert((node->parent != NULL) && "ERROR!!! Pointer to \'node->parent\' is NULL!\n");

    if (node == node->parent->right_child)
    {
        return true;
    }

    return false;
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
      RotateRight(GetParent(node));
    } 
    else 
    {
      RotateRight(GetParent(node));
      RotateLeft(GetParent(node));
    }
}

Node* SubTreeSplay(Node* root)
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

Node* SubTreeSearch(Node* root, char* key)
{
    Node* next = NULL;

    while (root != NULL)
    {
        next = NULL;

        if (strncmp(key, root->key1, MAX_SIZE) == 0)
        {
            return root;
        }
        else if (strncmp(key, root->key1, MAX_SIZE) < 0)
        {
            next = root->left_child;
        }
        else
        {
            next = root->right_child;
        }

        root = next;
    }

    return NULL;
}

Node* SubTreeInsert(Node* root, char* key1, char* key2)
{  
    Node* new_node  = NodeCtor(key1, key2);

    if (root == NULL)
    {
        return new_node;
    }

    Node* prev      = NULL;
    Node* next      = NULL;

    int   is_left   = false;

    while (root != NULL)
    {
        if (strncmp(key1, root->key1, MAX_SIZE) <= 0)
        {
            next    = root->left_child;
            is_left = true;
        }
        else
        {
            next    = root->right_child;
            is_left = false;
        }

        prev = root;
        root = next;
    }

    if (is_left)
    {
        prev->left_child    = new_node;
        new_node->parent    = prev;
    }
    else
    {
        prev->right_child   = new_node;
        new_node->parent    = prev;
    }

    return SubTreeSplay(new_node);
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
    SplayTree* new_splay_tree = (SplayTree*) calloc(1, sizeof(SplayTree));
    assert((new_splay_tree != NULL) && "ERROR!!! Program can not allocate memory!\n");

    new_splay_tree->number_of_nodes = 0;
    new_splay_tree->root            = NULL;

    return new_splay_tree;
}

void SplayTreeDtor(SplayTree* splay_tree)
{
    assert((splay_tree != NULL) && "ERROR!!! Pointer to \'splay_tree\' is NULL!\n");

    SubTreeDtor(splay_tree->root);

    splay_tree->number_of_nodes = 0;
    splay_tree->root            = NULL;

    free(splay_tree);
}

char* SplayTreeSearch(SplayTree* splay_tree, char* key)
{
    assert((splay_tree != NULL) && "ERROR!!! Pointer to \'splay_tree\' is NULL!\n");

    Node* key_node = SubTreeSearch(splay_tree->root, key);

    if (key_node == NULL)
    {
        return NULL;
    }

    splay_tree->root    = SubTreeSplay(key_node);
    return key_node->key2;
}

void SplayTreeInsert(SplayTree* splay_tree, char* key1, char* key2)
{
    assert((splay_tree != NULL) && "ERROR!!! Pointer to \'splay_tree\' is NULL!\n");

    splay_tree->root    = SubTreeInsert(splay_tree->root, key1, key2);
    splay_tree->root    = SubTreeInsert(splay_tree->root, key2, key1);
}