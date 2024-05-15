#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*==================Constants=================*/

const int   CHANGE_VALUE    = 0;
const int   GET_SUM         = 1;

/*===================Structs==================*/

typedef struct FenwickTree
{
    int*    array;
    size_t  size;
} FenwickTree;

/*==================Functions=================*/

int             GetSign(int index);

FenwickTree*    FenwickTreeCtor(int* array, size_t array_size);
void            FenwickTreeDtor(FenwickTree* fenwick_tree);

int             FenwickTreeGetSum(FenwickTree* fenwick_tree, int start);

void            FenwickTreeUpdate(FenwickTree* fenwick_tree, int index, int delta);
int             FenwickTreeFindSum(FenwickTree* fenwick_tree, int left, int right);

void            ExecuteCommands(FenwickTree* fenwick_tree, int number_of_requests, int* array);
/*============================================*/

int main()
{
    size_t array_size   = 0;

    if (scanf("%lu", &array_size) == 0)
    {
        assert(false && "ERROR!!! Program can not read the number!\n");
    }

    int* array = (int*) calloc(array_size, sizeof(int));
    assert((array != NULL) && "ERROR!!! Program can not allocate memory!\n");

    for (size_t i = 0; i < array_size; i++)
    {
        if (scanf("%d", array + i) == 0)
        {
            assert(false && "ERROR!!! Program can not read the number!\n");
        }
    }

    FenwickTree* fenwick_tree = FenwickTreeCtor(array, array_size);

    int number_of_requests  = 0;

    if (scanf("%d", &number_of_requests) == 0)
    {
        assert(false && "ERROR!!! Program can not read the number!\n");
    }

    ExecuteCommands(fenwick_tree, number_of_requests, array);

    FenwickTreeDtor(fenwick_tree);
    free(array);

    return 0;
}

void ExecuteCommands(FenwickTree* fenwick_tree, int number_of_requests, int* array)
{
    assert((fenwick_tree != NULL) && "ERROR!!! Pointer to \'fenwick_tree\' is NULL!\n");
    assert((array != NULL) && "ERROR!!! Pointer to \'array\' is NULL!\n");

    int command     = -1;
    
    int index       = 0;
    int new_value   = 0;
    int delta       = 0;

    int left        = 0;
    int right       = 0;
    int answer      = 0;

    for (int i = 0; i < number_of_requests; i++)
    {
        if (scanf("%d", &command) == 0)
        {
            assert(false && "ERROR!!! Program can not read the number!\n");
        }

        switch (command)
        {
            case CHANGE_VALUE:
            {
                if (scanf("%d", &index) == 0)
                {
                    assert(false && "ERROR!!! Program can not read the number!\n");
                }

                if (scanf("%d", &new_value) == 0)
                {
                    assert(false && "ERROR!!! Program can not read the number!\n");
                }

                index--;

                delta   = new_value - array[index];

                array[index] = new_value;

                FenwickTreeUpdate(fenwick_tree, index, delta);

                break;
            }

            case GET_SUM:
            {
                if (scanf("%d", &left) == 0)
                {
                    assert(false && "ERROR!!! Program can not read the number!\n");
                }

                if (scanf("%d", &right) == 0)
                {
                    assert(false && "ERROR!!! Program can not read the number!\n");
                }

                left--;
                right--;

                answer = FenwickTreeFindSum(fenwick_tree, left, right);

                printf("%d\n", answer);

                break;
            }
            
            default:
            {
                fprintf(stderr, "ERROR!!! Invalid command!\n");
                return;
            }
        }
    }
}

int GetSign(int index)
{
    return (index % 2 == 0) ? 1 : -1;
}

FenwickTree* FenwickTreeCtor(int* array, size_t array_size)
{
    assert((array != NULL)      && "ERROR!!! Pointer to \'array\' is NULL!\n");
    assert((array_size != 0)    && "ERROR!!! Invalid value of \'array_size\'!\n");

    FenwickTree* new_fenwick_tree = (FenwickTree*) calloc(1, sizeof(FenwickTree));
    assert((new_fenwick_tree != NULL) && "ERROR!!! Program can not allocate memory!\n");

    new_fenwick_tree->array = (int*) calloc(array_size, sizeof(int));
    assert((new_fenwick_tree->array != NULL) && "ERROR!!! Program can not allocate memory!\n");

    new_fenwick_tree->size  = array_size;

    for (size_t i = 0; i < new_fenwick_tree->size; i++)
    {
        FenwickTreeUpdate(new_fenwick_tree, i, array[i]);
    }

    return new_fenwick_tree;
}

void FenwickTreeDtor(FenwickTree* fenwick_tree)
{
    assert((fenwick_tree != NULL) && "ERROR!!! Pointer to \'fenwick_tree\' is NULL");

    for (size_t i = 0; i  < fenwick_tree->size; i++)
    {
        fenwick_tree->array[i] = 0;
    }

    free(fenwick_tree->array);

    fenwick_tree->array = NULL;
    fenwick_tree->size  = 0;

    free(fenwick_tree);
}

int FenwickTreeGetSum(FenwickTree* fenwick_tree, int start)
{
    assert((fenwick_tree != NULL) && "ERROR!!! Pointer to \'fenwick_tree\' is NULL");

    int result = 0;

    for (int i = start; i >= 0; i = (i & (i + 1)) - 1)
    {
        result += fenwick_tree->array[i];
    }

    return result;
}

void FenwickTreeUpdate(FenwickTree* fenwick_tree, int index, int delta)
{
    assert((fenwick_tree != NULL) && "ERROR!!! Pointer to \'fenwick_tree\' is NULL");

    for (int i = index; i < fenwick_tree->size; i = (i | (i + 1)))
    {
        fenwick_tree->array[i] += delta * GetSign(index);
    }
}

int FenwickTreeFindSum(FenwickTree* fenwick_tree, int left, int right)
{
    assert((fenwick_tree != NULL) && "ERROR!!! Pointer to \'fenwick_tree\' is NULL");

    int right_sum   = FenwickTreeGetSum(fenwick_tree, right);
    int left_sum    = FenwickTreeGetSum(fenwick_tree, left-1);

    return (right_sum - left_sum) * GetSign(left);
}
