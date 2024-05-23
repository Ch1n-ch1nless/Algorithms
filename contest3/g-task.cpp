#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*==================Constants=================*/

const int   CHANGE_VALUE    = 1;
const int   GET_SUM         = 2;
const int   STOP_EXECUTE    = 3;

/*===================Structs==================*/

typedef struct FenwickTree
{
    int***  array;
    size_t  size;
} FenwickTree;

/*==================Functions=================*/

FenwickTree*    FenwickTreeCtor(size_t array_size);
void            FenwickTreeDtor(FenwickTree* fenwick_tree);

int             FenwickTreeGetSum(FenwickTree* fenwick_tree, int start_x, int start_y, int start_z);

void            FenwickTreeUpdate(FenwickTree* fenwick_tree, int x, int y, int z, int delta);
int             FenwickTreeFindSum(FenwickTree* fenwick_tree, int left_x, int left_y, int left_z, int right_x, int right_y, int right_z);

void            ExecuteCommands(FenwickTree* fenwick_tree);
/*============================================*/

int main()
{
    size_t array_size   = 0;

    if (scanf("%lu", &array_size) == 0)
    {
        assert(false && "ERROR!!! Program can not read the number!\n");
    }

    FenwickTree* fenwick_tree = FenwickTreeCtor(array_size);

    ExecuteCommands(fenwick_tree);

    FenwickTreeDtor(fenwick_tree);

    return 0;
}

void ExecuteCommands(FenwickTree* fenwick_tree)
{
    assert((fenwick_tree != NULL) && "ERROR!!! Pointer to \'fenwick_tree\' is NULL!\n");

    int command     = -1;
    
    int delta       = 0;

    int left_coords[3]  = {};
    int right_coords[3] = {};

    while (command != STOP_EXECUTE)
    {
        if (scanf("%d", &command) == 0)
        {
            assert(false && "ERROR!!! Program can not read the command!\n");
        }

        switch (command)
        {
            case CHANGE_VALUE:
            {
                for (int i = 0; i < 3; i++)
                {
                    if (scanf("%d", &(left_coords[i])) == 0)
                    {
                        assert(false && "ERROR!!! Program can not read the coordinate!\n");
                    }
                }

                if (scanf("%d", &delta) == 0)
                {
                    assert(false && "ERROR!!! Program can not read the delta!\n");
                }

                FenwickTreeUpdate(fenwick_tree, left_coords[0], left_coords[1], left_coords[2], delta);

                break;
            }

            case GET_SUM:
            {
                for (int i = 0; i < 3; i++)
                {
                    if (scanf("%d", &(left_coords[i])) == 0)
                    {
                        assert(false && "ERROR!!! Program can not read the coordinate!\n");
                    }
                }

                for (int i = 0; i < 3; i++)
                {
                    if (scanf("%d", &(right_coords[i])) == 0)
                    {
                        assert(false && "ERROR!!! Program can not read the coordinate!\n");
                    }
                }

                int result = FenwickTreeFindSum(fenwick_tree, left_coords[0], left_coords[1], left_coords[2], right_coords[0], right_coords[1], right_coords[2]);

                printf("%d\n", result);

                break;
            }
            
            case STOP_EXECUTE:
            {
                return;
            }

            default:
                break;
        }
    };
}

FenwickTree* FenwickTreeCtor(size_t array_size)
{
    assert((array_size != 0)    && "ERROR!!! Invalid value of \'array_size\'!\n");

    FenwickTree* new_fenwick_tree = (FenwickTree*) calloc(1, sizeof(FenwickTree));
    assert((new_fenwick_tree != NULL) && "ERROR!!! Program can not allocate memory!\n");

    new_fenwick_tree->array = (int***) calloc(array_size, sizeof(int**));
    assert((new_fenwick_tree->array != NULL) && "ERROR!!! Program can not allocate memory!\n");

    new_fenwick_tree->size  = array_size;

    for (size_t i = 0; i < new_fenwick_tree->size; i++)
    {
        new_fenwick_tree->array[i] = (int**) calloc(array_size, sizeof(int*));
        assert((new_fenwick_tree->array[i] != NULL) && "ERROR!!! Program can not allocate memory!\n");

        for (size_t j = 0; j < new_fenwick_tree->size; j++)
        {
            new_fenwick_tree->array[i][j] = (int*) calloc(new_fenwick_tree->size, sizeof(int));
            assert((new_fenwick_tree->array[i][j] != NULL) && "ERROR!!! Program can not allocate memory!\n");
        }
    }

    return new_fenwick_tree;
}

void FenwickTreeDtor(FenwickTree* fenwick_tree)
{
    assert((fenwick_tree != NULL) && "ERROR!!! Pointer to \'fenwick_tree\' is NULL");

    for (size_t i = 0; i < fenwick_tree->size; i++)
    {
        for (size_t j = 0; j < fenwick_tree->size; j++)
        {
            free(fenwick_tree->array[i][j]);
        }

        free(fenwick_tree->array[i]);
    }

    free(fenwick_tree->array);

    fenwick_tree->array = NULL;
    fenwick_tree->size  = 0;

    free(fenwick_tree);
}

int FenwickTreeGetSum(FenwickTree* fenwick_tree, int start_x, int start_y, int start_z)
{
    assert((fenwick_tree != NULL) && "ERROR!!! Pointer to \'fenwick_tree\' is NULL");

    int result = 0;

    for (int i = start_x; i >= 0; i = (i & (i + 1)) - 1)
    {
        for (int j = start_y; j >= 0; j = (j & (j + 1)) - 1)
        {
            for (int k = start_z; k >= 0; k = (k & (k + 1)) - 1)
            {
                result += fenwick_tree->array[i][j][k];
            }
        }
    }

    return result;
}

void FenwickTreeUpdate(FenwickTree* fenwick_tree, int x, int y, int z, int delta)
{
    assert((fenwick_tree != NULL) && "ERROR!!! Pointer to \'fenwick_tree\' is NULL");

    for (int i = x; i < fenwick_tree->size; i = (i | (i + 1)))
    {
        for (int j = y; j < fenwick_tree->size; j = (j | (j + 1)))
        {
            for (int k = z; k < fenwick_tree->size; k = (k | (k + 1)))
            {
                fenwick_tree->array[i][j][k] += delta;
            }
        }
    }
}

int FenwickTreeFindSum(FenwickTree* fenwick_tree, int left_x, int left_y, int left_z, int right_x, int right_y, int right_z)
{
    assert((fenwick_tree != NULL) && "ERROR!!! Pointer to \'fenwick_tree\' is NULL");

    int max_sum = FenwickTreeGetSum(fenwick_tree, right_x, right_y, right_z);
    int min_sum = FenwickTreeGetSum(fenwick_tree, left_x-1, left_y-1, left_z-1);

    int max_sum1    = FenwickTreeGetSum(fenwick_tree, left_x-1, right_y,  right_z);
    int max_sum2    = FenwickTreeGetSum(fenwick_tree, right_x,  left_y-1, right_z);
    int max_sum3    = FenwickTreeGetSum(fenwick_tree, right_x,  right_y,  left_z-1);

    int min_sum1    = FenwickTreeGetSum(fenwick_tree, right_x,  left_y-1, left_z-1);
    int min_sum2    = FenwickTreeGetSum(fenwick_tree, left_x-1, right_y,  left_z-1);
    int min_sum3    = FenwickTreeGetSum(fenwick_tree, left_x-1, left_y-1, right_z );    

    return max_sum - max_sum1 - max_sum2 - max_sum3 + min_sum1 + min_sum2 + min_sum3 - min_sum;
}
