#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b)   (a < b) ? b : a
#define MIN(a, b)   (a < b) ? a : b

/*==================Constants=================*/

const int   MODULE              =  1000000000;

const int   KEY_IS_NOT_FOUND    = -1;    

/*===================Structs==================*/

/**
 * @struct Node
 * 
 * @brief структура узла AVL-дерева
*/
typedef struct Node
{
    int             key;        //!<  ключ узла
    size_t          height;     //!<  высота поддерева, которое имеет корень в этом узле  
    struct Node*    left;       //!<  Указатель на корень левого поддерева
    struct Node*    right;      //!<  Указатель на корень правого поддерева
} Node;


/**
 * @struct AVLTree
 * 
 * @brief структура AVL дерева
*/
typedef struct AVLTree
{   
    Node*   root;               //!< корень данного дерева
    size_t  size;               //!< количество узлов в дереве
} AVLTree;

/*==================Functions=================*/

/**
 * @brief Функция, которая создаёт узел дерева
 * 
 * @param [in] key - ключ, который будет в нем хранится
 * 
 * @return указатель на структуру узла
*/
Node*       NodeCtor(int key);

/**
 * @brief Функция, удаляющая узел из памяти
 * 
 * @param [in] node - указатель на структуру узла
*/
void        NodeDtor(Node* node);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/**
 * @brief Функция, которая создаёт пустое AVL-дерево
 * 
 * @return указатель на структуру AVL-дерева
*/
AVLTree*    AVLTreeCtor(void);

/**
 * @brief Функция, удаляющая AVL-дерево из памяти
 * 
 * @param [in] avl_tree - указатель на структуру AVL-дерева
*/
void        AVLTreeDtor(AVLTree* avl_tree);


/**
 * @brief Функция поиска ключа в AVL-дереве
 * 
 * @param [in] avl_tree - указатель на структуру AVL-дерева
 * 
 * @param [in] key - ключ, который хотим найти
 * 
 * @return true, если ключ найден, иначе false
*/
bool        AVLTreeSearch(AVLTree* avl_tree, int key);

/**
 * @brief Функция вставки ключа в AVL-дерево
 * 
 * @param [in] avl_tree - указатель на структуру AVL-дерева
 * 
 * @param [in] key - ключ, который хотим вставить
*/
void        AVLTreeInsert(AVLTree* avl_tree, int key);

/**
 * @brief Функция удаления ключа из AVL-дерева
 * 
 * @param [in] avl_tree - указатель на структуру AVL-дерева
 * 
 * @param [in] key - ключ, который хотим удалить
*/
void        AVLTreeDelete(AVLTree* avl_tree, int key);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/**
 * @brief Функция удаления поддерева с корня
 * 
 * @param [in] root - указатель на корень поддерева
*/
void        SubTreeDtor(Node* root);

/**
 * @brief Функция вставки ключа в поддерево
 * 
 * @param [in] root - указатель на корень поддерева
 * 
 * @param [in] key - ключ, который хотим вставить
 * 
 * @return Указатель на корень поддерева с новым узлом
*/
Node*       SubTreeInsert(Node* root, int key);

/**
 * @brief Функция удаления ключа из поддерева
 * 
 * @param [in] root - указатель на корень поддерева
 * 
 * @param [in] key -  ключ, который хотим удалить
 * 
 * @return Указатель на корень поддерева без узла с ключом key
*/
Node*       SubTreeDelete(Node* root, int key);



/**
 * @brief Функция, которая возвращает высоту поддерева с корнем в текущем узле
 * 
 * @param [in] node - указатель на корень поддерева
 * 
 * @return высота поддерева
*/
size_t      Height(Node* node);

/**
 * @brief Функция пересчета высоты в корне поддерева
 * 
 * @param [in] node - указатель на корень поддерева
*/
void        FixHeight(Node* node);

/**
 * @brief Функция, которая считает баланс в поддереве
 * 
 * @param [in] node - указатель на корень поддерева
 * 
 * @return разницу в высотах между правым узлом и левым
*/
int         CalculateBalance(Node* node);

/**
 * @brief Разворот дерева 
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
 * @param [in] parent - указатель на узел, который хотим развернуть
 * 
 * @return указатель на X
*/
Node*       RotateLeft(Node* parent);

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
 * @param [in] parent - 
 * 
 * @return указатель на X
*/
Node*       RotateRight(Node* parent);

/**
 * @brief Функция, которая подсчитывает баланс поддерева, и если он нарушен, то делает развороты
 * 
 * @param [in] subtree_root - указатель на корень поддерева
 * 
 * @return указатель на корень поддерева с исправленным балансом 
*/
Node*       FixBalanceInSubTree(Node* subtree_root);



/**
 * @brief Функция поиска максимального ключа в поддереве
 * 
 * @param [in] subtree_root - указатель на корень поддерева
 * 
 * @return указатель на узел с максимальным ключом
*/
Node*       FindMaxKeyInSubTree(Node* subtree_root);

/**
 * @brief Функция, которая открепляет узел с максимальным ключом из поддерева
 * 
 * @param [in] subtree_root - указатель на корень поддерева
 * 
 * @return корень поддерева без максимального ключа
*/
Node*       UnLinkMaxKeyInSubTree(Node* subtree_root);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/**
 * @brief Функция, которая считывает команды пользователя и исполняет их
 * 
 * @param [in] avl_tree - указатель на AVL-дерево
 * 
 * @param [in] number_of_commands - количество команд пользователя
*/
void        ExecuteCommands(AVLTree* avl_tree, size_t number_of_commands);

/**
 * @brief Функция добавления ключа в дереве
 * 
 * @param [in] avl_tree - указатель на AVL-дерево
 * 
 * @param [in] key - ключ, который хотим вставить
*/
void        Add(AVLTree* avl_tree, int key);

/**
 * @brief Функция поиска наименьшего ключа в поддереве, который больше key
 * 
 * @param [in] node - указатель на корень поддерева
 * 
 * @param [in] key - ключ, относительного которого происходит поиска
 * 
 * @return наименьший ключ, который больше key
*/
int         Next(Node* node, int key);

/*============================================*/

int main()
{
    size_t number_of_commands = 0;
    if (scanf("%lu", &number_of_commands) == 0)
    {
        assert(false && "Program can not read the number!\n");
    }

    AVLTree* avl_tree = AVLTreeCtor();

    ExecuteCommands(avl_tree, number_of_commands);

    AVLTreeDtor(avl_tree);

    return 0;
}

void ExecuteCommands(AVLTree* avl_tree, size_t number_of_commands)
{
    char    operation  = '\0';
    int     key         = 0;
    int     answer      = 0;

    for (size_t i = 0; i < number_of_commands; i++)
    {
        if (scanf(" %c %d", &operation, &key) == 0)
        {
            assert(false && "Program can not read the request!\n");
        }

        switch (operation)
        {
            case '+':
            {
                Add(avl_tree, (key + answer) % MODULE);
                answer = 0;
                break;
            }

            case '?':
            {
                answer = Next(avl_tree->root, key);
                printf("%d\n", answer);
                break;
            }
            
            default:
            {
                break;
            }
        }
    }
}

void Add(AVLTree* avl_tree, int key)
{
    AVLTreeInsert(avl_tree, key);
}

int Next(Node* node, int key)
{
    if (node == NULL)
    {
        return KEY_IS_NOT_FOUND;
    }

    if (node->key == key)
    {
        return node->key;
    }
    else if (node->key > key)
    {
        int key_in_left_subtree = Next(node->left, key);

        if (key_in_left_subtree == KEY_IS_NOT_FOUND)
        {
            return node->key;
        }

        return key_in_left_subtree;
    }
    else
    {
        return Next(node->right, key);
    }
}

/*v----Node functions----v*/

Node* NodeCtor(int key)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    assert((new_node != NULL) && "Program can not allocate memory!\n");

    new_node->height    = 1;
    new_node->key       = key;
    new_node->left      = NULL;
    new_node->right     = NULL;

    return new_node;
}

void NodeDtor(Node* node)
{
    if (node == NULL)   return;

    node->height = 0;
    node->key    = 0;
    node->left   = NULL;
    node->right  = NULL;

    free(node);
}

/*v----AVL Tree functions----v*/

AVLTree* AVLTreeCtor(void)
{
    AVLTree* avl_tree = (AVLTree*) calloc(1, sizeof(AVLTree));
    assert((avl_tree != NULL) && "Program can not allocate memory!\n");

    avl_tree->root  = NULL;
    avl_tree->size  = 0;

    return avl_tree;
}

void AVLTreeDtor(AVLTree* avl_tree)
{
    assert((avl_tree != NULL) && "Error! Pointer to \'avl_tree\' is NULL!!!\n");

    SubTreeDtor(avl_tree->root);

    avl_tree->root  = NULL;
    avl_tree->size  = 0;

    free(avl_tree);
}

bool AVLTreeSearch(AVLTree* avl_tree, int key)
{
    assert((avl_tree != NULL) && "Error! Pointer to \'avl_tree\' is NULL!!!\n");

    Node* cur_node = avl_tree->root;

    while(cur_node != NULL)
    {
        if (cur_node->key == key)
        {
            break;
        } 
        else if (cur_node->key < key)
        {
            cur_node = cur_node->right;
        }
        else
        {   
            cur_node = cur_node->left;
        }
    }

    return (cur_node == NULL) ? false : true;
}

void AVLTreeInsert(AVLTree* avl_tree, int key)
{
    assert((avl_tree != NULL) && "Error! Pointer to \'avl_tree\' is NULL!!!\n");

    avl_tree->root = SubTreeInsert(avl_tree->root, key);
    avl_tree->size = avl_tree->size + 1;
}

void AVLTreeDelete(AVLTree* avl_tree, int key)
{
    assert((avl_tree != NULL) && "Error! Pointer to \'avl_tree\' is NULL!!!\n");

    avl_tree->root = SubTreeDelete(avl_tree->root, key);
    avl_tree->size = avl_tree->size - 1;
}

/*v----Secondary Functions----v*/

void SubTreeDtor(Node* root)
{
    if (root == NULL) return;

    SubTreeDtor(root->left);
    SubTreeDtor(root->right);
    NodeDtor(root);
}

Node* SubTreeInsert(Node* root, int key)
{
    if (root == NULL)
    {
        root = NodeCtor(key);
        return FixBalanceInSubTree(root);
    }

    if (root->key == key)
    {
        return root;
    }
    else if (root->key > key)
    {
        root->left = SubTreeInsert(root->left, key);
    }
    else
    {
        root->right = SubTreeInsert(root->right, key);
    }

    return FixBalanceInSubTree(root);
}

Node* SubTreeDelete(Node* root, int key)
{
    if (root == NULL)
    {
        root = NodeCtor(key);
        return FixBalanceInSubTree(root);
    }

    if (root->key == key)
    {
        if (root->left == NULL || root->right == NULL)
        {
            Node* tmp_node = (root->left == NULL) ? root->right : root->left;
            NodeDtor(root);
            return FixBalanceInSubTree(tmp_node);
        }

        Node* max_left = FindMaxKeyInSubTree(root);
        root->left = UnLinkMaxKeyInSubTree(root->left);

        max_left->left  = root->left;
        max_left->right = root->right;
        NodeDtor(root);
        return FixBalanceInSubTree(max_left);
    }
    else if (root->key > key)
    {
        root->left = SubTreeDelete(root->left, key);
    }
    else
    {
        root->right = SubTreeDelete(root->right, key);
    }

    return FixBalanceInSubTree(root);
}

Node* FindMaxKeyInSubTree(Node* root)
{
    assert((root == NULL) && "ERROR Invalid value of root\n");

    while (root->right != NULL)
    {
        root = root->right;
    }

    return root;
}

Node* UnLinkMaxKeyInSubTree(Node* root)
{
    assert((root == NULL) && "ERROR! Invalid value of root\n");

    if (root->right == NULL)
    {
        return root->left;
    }

    root->right = UnLinkMaxKeyInSubTree(root->right);
    return FixBalanceInSubTree(root);
}

size_t Height(Node* node)
{
    return (node == NULL) ? 0 : node->height;
}

void FixHeight(Node* node)
{
    if (node == NULL)
    {
        return;
    }
    
    node->height = MAX(Height(node->left), Height(node->right));
    node->height = node->height + 1;
}

int CalculateBalance(Node* node)
{
    if (node == NULL)
    {
        return 0;
    }

    return Height(node->right) - Height(node->left);
}

Node* RotateLeft(Node* parent)
{
    Node* right_child   = parent->right;
    parent->right       = right_child->left;
    right_child->left   = parent;
    FixHeight(parent);
    FixHeight(right_child);
    return right_child;
}

Node* RotateRight(Node* parent)
{
    Node* left_child    = parent->left;
    parent->left        = left_child->right;
    left_child->right   = parent;
    FixHeight(parent);
    FixHeight(left_child);
    return left_child;
}

Node* FixBalanceInSubTree(Node* subtree_root)
{
    FixHeight(subtree_root);

    if (CalculateBalance(subtree_root) == 2)
    {
        if (CalculateBalance(subtree_root->right) < 0)
        {
            subtree_root->right = RotateRight(subtree_root->right);
        }
        return RotateLeft(subtree_root);
    }
    else if (CalculateBalance(subtree_root) == -2)
    {
        if (CalculateBalance(subtree_root->left) > 0) {
          subtree_root->left = RotateLeft(subtree_root->left);
        }
        return RotateRight(subtree_root);
    }

    return subtree_root;
}