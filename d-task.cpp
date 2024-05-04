#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*==================Constants=================*/

const int       OK                          = 1;
const int       ERROR                       = 0;

const int       TRUE                        = 1;
const int       FALSE                       = 0;

const size_t    BIG_PRIME_NUMBER            = 1e9 + 7;

const int       POISON_VALUE                = -525252;

/*===================Structs==================*/

typedef struct
{
    int filled;
    int value;
} elem_t;

typedef struct HashData
{
    size_t  a;
    size_t  b;
    size_t  p;
} HashData;

typedef struct InternalHashTable
{
    HashData    coefficients;
    size_t      size;
    elem_t*     data;
} InternalHashTable;

typedef struct HashTable
{
    HashData            coefficients;
    int                 size;
    InternalHashTable*  data;
} HashTable;

typedef struct DynamicArray
{
    int*    data;
    size_t  size;
    size_t  capacity;
} DynamicArray;


typedef struct SecondaryHashTable
{
    DynamicArray*   data;
    size_t          size;
} SecondaryHashTable;

/*==================Functions=================*/

int                 FindMinPowerOfTwo(int number);

void                DynamicArrayCtor(  DynamicArray* array, size_t capacity );
void                DynamicArrayDtor(  DynamicArray* array                  );
void                DynamicArrayInsert(DynamicArray* array, int    key      );

SecondaryHashTable* SecondaryHashTableCtor(int* collisions_array, int* array, size_t array_size, size_t ht_size, HashData* hash_cf);
void                SecondaryHashTableDtor(SecondaryHashTable* hash_table);

int                 Hash(HashData* hash_coefficients, int key);

int*                FindPerfectFirstHashFunction( HashData* hash_coefficients, int* array, size_t array_size, size_t ht_size);
void                FindPerfectSecondHashFunction(HashData* hash_coefficients, int* array, size_t array_size);

void                InternalHashTableCtor(InternalHashTable* hash_table, size_t size    );
void                InternalHashTableDtor(InternalHashTable* hash_table                 );

int                 InternalHashTableInsert(InternalHashTable* hash_table, int key);
int                 InternalHashTableSearch(InternalHashTable* hash_table, int key);

HashTable*          HashTableCtor(int* array, size_t array_size);
void                HashTableDtor(HashTable* hash_table);

int                 HashTableInsert(HashTable* hash_table, int key);
int                 HashTableSearch(HashTable* hash_table, int key);

void                FindKeysInHashTable(HashTable* hash_table);

#define red(str)    "\033[31m"#str"\033[0m"
#define green(str)  "\033[32m"#str"\033[0m"
#define yellow(str) "\033[33m"#str"\033[0m"

void Dump(struct HashTable* hash_table) {
    if(!hash_table) {
        printf("\nHash table is NULL\n");
        return;
    }
    printf("\nHash table Dump:\n");
    printf("Size: %d\n", hash_table->size);
    printf("Hash Parameters: a=%d, b=%d, p=%d\n", hash_table->coefficients.a, hash_table->coefficients.b, hash_table->coefficients.p);
    for(size_t i = 0; i < hash_table -> size; i++) {
        printf(red(Bucket)" %lu:\n", i);
        InternalHashTable* second_hash_table = hash_table->data + i;
        if(!second_hash_table) {
            printf("\tHash_Table is NULL\n");
            continue;
        }
        printf("\tSize: %lu\n", second_hash_table -> size);
        printf("\tHash Parameters: a=%d, b=%d, p=%d\n", second_hash_table->coefficients.a, second_hash_table->coefficients.b, second_hash_table->coefficients.p);
        for(size_t j = 0; j < second_hash_table -> size; j++) {
            printf(green(\tBucket)" %lu:\n", j);
            printf("\t\tValue: %d\n", second_hash_table->data[j].value);
        }
    }
}

/*==============================================*/

int main()
{
    srand(time(NULL));

    size_t array_size = 0;

    if (scanf("%lu", &array_size) == 0)
    {
        assert(FALSE && "ERROR!!! Program can not read the number!\n");
    }

    int* array_of_keys = (int*) calloc(array_size, sizeof(int));
    assert((array_of_keys != NULL) && "ERROR!!! Program can not allocate memory!\n");

    //Read the array!
    for (size_t i = 0; i < array_size; i++)
    {
        if (scanf("%d", array_of_keys + i) == 0)
        {
            assert(FALSE && "ERROR!!! Program can not read the number!\n");
        }
    }

    HashTable* hash_table = HashTableCtor(array_of_keys, array_size);

    //Dump(hash_table);

    FindKeysInHashTable(hash_table);

    HashTableDtor(hash_table);

    free(array_of_keys);

    return 0;
}

void FindKeysInHashTable(HashTable* hash_table)
{
    assert((hash_table != NULL) && "ERROR!!! Pointer to \'hash_table\' is NULL!\n");

    int key = POISON_VALUE;

    while (scanf("%d", &key) != 0)
    {
        if (HashTableSearch(hash_table, key) == TRUE)
        {
            printf("YES\n");
        }
        else
        {
            printf("NO\n");
        }
    }
}

/*v~~~~~~~~~~~~Secondary functions~~~~~~~~~~~v*/

int FindMinPowerOfTwo(int number)
{
    int min_pow = 1;

    while (number > min_pow)
    {
        min_pow <<= 1;
    }

    return min_pow;
}

/*v~~~~~~~~~~DynamicArray functions~~~~~~~~~~v*/

void DynamicArrayCtor(DynamicArray* array, size_t capacity)
{
    assert((array != NULL) && "ERROR!!! Pointer to \'array\' is NULL!\n");

    array->capacity = capacity;
    array->size     = 0;

    array->data     = (int*) calloc(array->capacity, sizeof(int));
    assert((array->data != NULL) && "ERROR!!! Program can not allocate memory!\n");
}

void DynamicArrayInsert(DynamicArray* array, int key)
{
    assert((array != NULL) && "ERROR!!! Pointer to \'array\' is NULL!\n");

    array->data[array->size] = key;
    array->size++;
}

void DynamicArrayDtor(DynamicArray* array)
{
    assert((array != NULL) && "ERROR!!! Pointer to \'array\' is NULL!\n");

    free(array->data);

    array->data     = NULL;
    array->size     = 0;
    array->capacity = 0;
}

/*^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^*/

/*v~~~~~~~SecondaryHashTable functions~~~~~~~v*/

SecondaryHashTable* SecondaryHashTableCtor(int* collisions_array, int* array, size_t array_size, size_t ht_size, HashData* hash_cf)
{
    assert((collisions_array    != NULL) && "ERROR!!! Pointer to \'collisions_array\'   is NULL!\n");
    assert((array               != NULL) && "ERROR!!! Pointer to \'array\'              is NULL!\n");
    assert((hash_cf             != NULL) && "ERROR!!! Pointer to \'hash_cf\'            is NULL!\n");

    SecondaryHashTable* hash_table = (SecondaryHashTable*) calloc(1, sizeof(SecondaryHashTable));
    assert((hash_table != NULL) && "ERROR!!! Program can not allocate memory!\n");

    hash_table->size    = ht_size;
    hash_table->data    = (DynamicArray*) calloc(ht_size, sizeof(DynamicArray));
    assert((hash_table->data != NULL) && "ERROR!!! Program can not allocate memory!\n");

    for (size_t i = 0; i < ht_size; i++)
    {
        DynamicArrayCtor(hash_table->data + i, collisions_array[i]);
    }

    for (size_t i = 0; i < array_size; i++)
    {
        int index = Hash(hash_cf, array[i]) % ht_size;

        DynamicArrayInsert(hash_table->data + index, array[i]);
    }

    return hash_table;
}

void SecondaryHashTableDtor(SecondaryHashTable* hash_table)
{
    assert((hash_table != NULL) && "ERROR!!! Pointer to \'hash_table\' is NULL!\n");

    for (size_t i = 0; i < hash_table->size; i++)
    {
        DynamicArrayDtor(hash_table->data + i);
    }

    free(hash_table->data);

    hash_table->data    = NULL;
    hash_table->size    = 0;

    free(hash_table);
}

/*^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^*/

/*v~~~~~~~~~~~~~Calculate Hash~~~~~~~~~~~~~~~v*/

int Hash(HashData* hash_coefficients, int key)
{
    int temp_result = (hash_coefficients->a * key + hash_coefficients->b) % hash_coefficients->p;
    temp_result     = (temp_result + hash_coefficients->p) % hash_coefficients->p;
    return temp_result;
}

int* FindPerfectFirstHashFunction(HashData* hash_coefficients, int* array, size_t array_size, size_t ht_size)
{
    assert((hash_coefficients   != NULL) && "ERROR!!! Pointer to \'hash_coefficients\'  is NULL!\n");
    assert((array               != NULL) && "ERROR!!! Pointer to \'array\'              is NULL!\n");

    int* collisions_array = (int*) calloc(ht_size, sizeof(int));
    assert((collisions_array != NULL) && "ERROR!!! Program can not allocate memory!\n");

    HashData new_coefficients = {0, 0, 0};

    while (TRUE)
    {   
        while (new_coefficients.a == 0)
        {
            new_coefficients.a = rand() % BIG_PRIME_NUMBER;
        }
        new_coefficients.b = rand() % BIG_PRIME_NUMBER;
        new_coefficients.p = BIG_PRIME_NUMBER;

        for (size_t i = 0; i < array_size; i++)
        {
            int index = Hash(&new_coefficients, array[i]) % ht_size;
            assert(index >= 0);
            collisions_array[index]++;
        }

        int sum_of_collisions = 0;  

        for (size_t i = 0; i < array_size; i++)
        {
            collisions_array[i] *= collisions_array[i];
            sum_of_collisions   += collisions_array[i];
        }

        if (sum_of_collisions < 4 * array_size)
        {
            hash_coefficients->a = new_coefficients.a;
            hash_coefficients->b = new_coefficients.b;
            hash_coefficients->p = new_coefficients.p;
            return collisions_array;
        }

        for (size_t i = 0; i < array_size; i++)
        {
            collisions_array[i] = 0;
        }
    }

    free(collisions_array);
    return NULL;
}

void FindPerfectSecondHashFunction(HashData* hash_coefficients, int* array, size_t array_size)
{
    assert((hash_coefficients   != NULL) && "ERROR!!! Pointer to \'hash_coefficients\'  is NULL!\n");
    assert((array               != NULL) && "ERROR!!! Pointer to \'array\'              is NULL!\n");

    int* collisions_array = (int*) calloc(array_size * array_size, sizeof(int));
    assert((collisions_array != NULL) && "ERROR!!! Program can not allocate memory!\n");

    HashData new_coefficients = {0, 0, 0};

    while (TRUE)
    {   
        while (new_coefficients.a == 0)
        {
            new_coefficients.a = rand() % BIG_PRIME_NUMBER;
        }
        new_coefficients.b = rand() % BIG_PRIME_NUMBER;
        new_coefficients.p = BIG_PRIME_NUMBER;

        for (size_t i = 0; i < array_size; i++)
        {
            int index = Hash(&new_coefficients, array[i]) % (array_size * array_size);
            assert(index >= 0);
            collisions_array[index]++;
        }

        int sum_of_collisions = 0;  

        for (size_t i = 0; i < array_size; i++)
        {
            collisions_array[i] *= collisions_array[i];
            sum_of_collisions   += collisions_array[i];
        }

        if (sum_of_collisions <= array_size)
        {
            hash_coefficients->a = new_coefficients.a;
            hash_coefficients->b = new_coefficients.b;
            hash_coefficients->p = new_coefficients.p;
            break;
        }

        for (size_t i = 0; i < array_size * array_size; i++)
        {
            collisions_array[i] = 0;
        }
    }

    free(collisions_array);
}

/*^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^*/

/*v~~~~~~~~InternalHashTable functions~~~~~~~v*/

void InternalHashTableCtor(InternalHashTable* hash_table, size_t size)
{
    assert((hash_table != NULL) && "ERROR!!! Pointer to \'hash_table\' is NULL!\n");

    hash_table->size            = size;
    hash_table->coefficients.a  = 0;
    hash_table->coefficients.b  = 0;
    hash_table->coefficients.p  = 0;

    if (hash_table->size == 0)
    {
        hash_table->data    = NULL;
        return;
    }

    hash_table->data = (elem_t*) calloc(size, sizeof(elem_t));
    assert((hash_table->data != NULL) && "ERROR!!! Program can not allocate memory!\n");

    for (size_t i = 0; i < size; i++)
    {
        hash_table->data[i].filled  = FALSE;
        hash_table->data[i].value   = POISON_VALUE;
    }
}

void InternalHashTableDtor(InternalHashTable* hash_table)
{
    assert((hash_table != NULL) && "ERROR!!! Pointer to \'hash_table\' is NULL!\n");

    if (hash_table->size == 0)
    {
        return;
    }

    for (size_t i = 0; i < hash_table->size; i++)
    {
        hash_table->data[i].filled  = 0;
        hash_table->data[i].value   = 0;
    }

    free(hash_table->data);

    hash_table->data            = NULL;
    hash_table->size            = 0;
    hash_table->coefficients.a  = 0;
    hash_table->coefficients.b  = 0;
    hash_table->coefficients.p  = 0;
}

int InternalHashTableInsert(InternalHashTable* hash_table, int key)
{
    assert((hash_table != NULL) && "ERROR!!! Pointer to \'hash_table\' is NULL!\n");

    size_t index = Hash(&hash_table->coefficients, key) % hash_table->size;

    if (hash_table->data[index].filled == FALSE)
    {
        hash_table->data[index].value   = key;
        hash_table->data[index].filled  = TRUE;
        return OK;
    }

    return FALSE;
}

int InternalHashTableSearch(InternalHashTable* hash_table, int key)
{
    assert((hash_table != NULL) && "ERROR!!! Pointer to \'hash_table\' is NULL!\n");

    size_t index = Hash(&hash_table->coefficients, key) % hash_table->size;

    if (hash_table->data[index].filled == FALSE)
    {
        return FALSE;
    }
    else
    {
        if (hash_table->data[index].value == key)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

/*^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^*/

/*v~~~~~~~~~~~~HashTable functions~~~~~~~~~~~v*/

HashTable*  HashTableCtor(int* array, size_t array_size)
{
    assert((array != NULL) && "ERROR!!! Pointer to \'array\' is NULL!\n");

    HashTable* hash_table = (HashTable*) calloc(1, sizeof(HashTable));
    assert((hash_table != NULL) && "ERROR!!! Program can not allocate memory!\n");

    hash_table->size            = FindMinPowerOfTwo(array_size);

    hash_table->data            = (InternalHashTable*) calloc(hash_table->size, sizeof(InternalHashTable));
    assert((hash_table->data != NULL) && "ERROR!!! Program can not allocate memory!\n");

    int* collisions_array = FindPerfectFirstHashFunction(&hash_table->coefficients, array, array_size, hash_table->size);

    SecondaryHashTable* temp_hash_table = SecondaryHashTableCtor(collisions_array, array, array_size, hash_table->size, &hash_table->coefficients);

    for (size_t i = 0; i < hash_table->size; i++)
    {
        InternalHashTableCtor(hash_table->data + i, collisions_array[i]);

        int*    cur_array   = temp_hash_table->data[i].data;
        size_t  cur_size    = temp_hash_table->data[i].size;

        int number_of_collisions = 0;

        do 
        {
            number_of_collisions = 0;

            FindPerfectSecondHashFunction(&hash_table->data[i].coefficients, cur_array, cur_size);

            for (size_t j = 0; j < cur_size; j++)
            {
                if (InternalHashTableInsert(hash_table->data + i, cur_array[j]) == ERROR)
                {
                    number_of_collisions++;
                }
            }

            if (number_of_collisions > 0)
            {
                for (size_t j = 0; j < hash_table->data[i].size; j++)
                {
                    hash_table->data[i].data[j].filled  = FALSE;
                    hash_table->data[i].data[j].value   = POISON_VALUE;
                }
            }
        } 
        while (number_of_collisions != 0);

    }   

    SecondaryHashTableDtor(temp_hash_table);
    free(collisions_array);

    return hash_table;
}

void HashTableDtor(HashTable* hash_table)
{
    assert((hash_table != NULL) && "ERROR!!! Program can not allocate memory!\n");

    for (size_t i = 0; i < hash_table->size; i++)
    {
        InternalHashTableDtor(hash_table->data + i);
    }

    free(hash_table->data);

    hash_table->data            = NULL;
    hash_table->size            = 0;
    hash_table->coefficients.a  = 0;
    hash_table->coefficients.b  = 0;
    hash_table->coefficients.p  = 0;

    free(hash_table);
}

int HashTableInsert(HashTable* hash_table, int key)
{
    assert((hash_table != NULL) && "ERROR!!! Program can not allocate memory!\n");

    int index = Hash(&hash_table->coefficients, key) % hash_table->size;

    if (hash_table->data[index].size == 0)
    {
        return ERROR;
    }
    else
    {
        return InternalHashTableInsert(hash_table->data + index, key);
    }
}

int HashTableSearch(HashTable* hash_table, int key)
{
    assert((hash_table != NULL) && "ERROR!!! Program can not allocate memory!\n");

    int index = Hash(&hash_table->coefficients, key) % hash_table->size;

    if (hash_table->data[index].size == 0)
    {
        return FALSE;
    }
    else
    {
        return InternalHashTableSearch(hash_table->data + index, key);
    }
}

/*^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^*/