#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*==================Constants=================*/

const int       OK              = 1;
const int       ERROR           = 0;

const size_t    HASH_TABLE_SIZE = 1e6 + 3;

/*===================Structs==================*/

typedef struct Node
{
    int             val;
    struct Node*    next;
    struct Node*    prev;
} Node;

typedef struct List
{
    Node*   head;
    Node*   tail;
    size_t  size;
} List;

typedef struct HashTable
{
    List*   array;
    size_t  size;
} HashTable;

/*==================Functions=================*/

Node*       NodeCtor(int val);
void        NodeDtor(Node* node);

List*       ListCtor(void);
void        ListDtor(List* list);

Node*       ListSearch(List* list, int val);
int         ListInsert(List* list, int val);
int         ListErase(List* list, int val);

HashTable*  HashTableCtor(size_t hash_table_size);
void        HashTableDtor(HashTable* hash_table);

Node*       HashTableSearch(HashTable* hash_table, int elem);
int         HashTableInsert(HashTable* hash_table, int elem);
int         HashTableErase(HashTable* hash_table, int elem);

int         HashFunc(int elem);

void        ExecuteCommands(HashTable* hash_table);
/*============================================*/

int main()
{
    HashTable* hash_table = HashTableCtor(HASH_TABLE_SIZE);

    ExecuteCommands(hash_table);

    HashTableDtor(hash_table);

    return 0;
}

void ExecuteCommands(HashTable* hash_table)
{
    assert(hash_table != NULL);

    size_t number_of_commands = 0;

    if (scanf("%ld", &number_of_commands) == 0)
    {
        assert(0 && "Reading error!\n");
    }

    char    operation   = '\0';
    int     number      = 0;

    for (size_t i = 0; i < number_of_commands; i++)
    {
        //Read the command
        if (scanf(" %c %d", &operation, &number) == 0)
        {
            assert(0 && "Reading error!\n");
        }

        switch (operation)
        {
            case '+':
            {
                HashTableInsert(hash_table, number);
                break;
            }

            case '-':
            {
                HashTableErase(hash_table, number);
                break;
            }

            case '?':
            {
                if (HashTableSearch(hash_table, number) == NULL)
                {
                    printf("NO\n");
                }
                else
                {
                    printf("YES\n");
                }
                break;
            }
        }
    }
}

//----------------------------------------------

Node* NodeCtor(int val)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    assert((new_node != NULL) && "Program can not allocate memory!\n");

    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->val  = val;

    return new_node;
}

void NodeDtor(Node* node)
{
    assert(node != NULL);

    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }

    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }

    node->next = NULL;
    node->prev = NULL;
    node->val  = 0;
    free(node);
}

//----------------------------------------------

List* ListCtor(void)
{
    List* new_list = (List*) calloc(1, sizeof(List));
    assert((new_list != NULL) && "Program can not allocate memory!\n");

    return new_list;
}

void ListDtor(List* list)
{
    assert(list != NULL);

    Node* cur_node  = list->head;
    Node* next_node = NULL;

    for (size_t i = 0; i < list->size; i++)
    {   
        next_node = cur_node->next;
        NodeDtor(cur_node);
        cur_node = next_node;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

Node* ListSearch(List* list, int val)
{
    assert(list != NULL);

    Node* node = list->head;

    for (size_t i = 0; i < list->size; i++)
    {
        if (node->val == val)
        {
            break;
        }
        node = node->next;
    }

    return node;
}

int ListInsert(List* list, int val)
{
    assert(list != NULL);

    Node* node = ListSearch(list, val);

    if (node == NULL)
    {
        Node* new_node = NodeCtor(val);
        if (new_node == NULL) 
        {
            return ERROR;
        }

        if (list->head == NULL)
        {
            list->head = new_node;
            list->tail = new_node;
            list->size = 1;
            return OK;
        }
        
        list->tail->next = new_node;
        new_node->prev = list->tail;
        list->tail = new_node;
        list->size++;
    }

    return OK;
}

int ListErase(List* list, int val)
{
    assert(list != NULL);

    Node* node = ListSearch(list, val);

    if (node == NULL)
    {
        return OK;
    } 

    if (list->head == node)
    {
        list->head = node->next;
    } 

    if (list->tail == node)
    {
        list->tail = node->prev;
    }

    NodeDtor(node);
    list->size--;

    return OK;
}

//----------------------------------------------

HashTable* HashTableCtor(size_t hash_table_size)
{
    assert(hash_table_size != 0);

    HashTable* new_hash_table = (HashTable*) calloc(1, sizeof(HashTable));
    assert((new_hash_table != NULL) && "Program can not allocate memory!\n");

    new_hash_table->size = hash_table_size;

    new_hash_table->array = (List*) calloc(hash_table_size, sizeof(List));
    assert((new_hash_table->array != NULL) && "Program can not allocate memory!\n");

    return new_hash_table;
}

void HashTableDtor(HashTable* hash_table)
{
    assert(hash_table != NULL);

    for (size_t i = 0; i < hash_table->size; i++)
    {
        ListDtor(&(hash_table->array[i]));
    }

    free(hash_table->array);
    hash_table->array   = NULL;
    hash_table->size    = 0;

    free(hash_table);
}

Node* HashTableSearch(HashTable* hash_table, int elem)
{
    assert(hash_table != NULL);

    int index = HashFunc(elem);

    Node* cur_node = ListSearch(&(hash_table->array[index]), elem);

    return cur_node;
}

int HashTableInsert(HashTable* hash_table, int elem)
{
    assert(hash_table != NULL);

    int index = HashFunc(elem);

    Node* node = HashTableSearch(hash_table, elem);

    if (node != NULL)
    {
        return OK;
    }

    return ListInsert(&(hash_table->array[index]), elem);
}

int HashTableErase(HashTable* hash_table, int elem)
{
    assert(hash_table != NULL);

    int index = HashFunc(elem);

    Node* node = HashTableSearch(hash_table, elem);

    if (node == NULL)
    {
        return OK;
    }

    return ListErase(&(hash_table->array[index]), elem);
}

//----------------------------------------------

int HashFunc(int elem)
{
    return elem % HASH_TABLE_SIZE;
}
