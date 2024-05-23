#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*==================Constants=================*/

const int   MAX_VALUE   = INT_MAX;

typedef struct Pair
{
    const int* first;
    const int* second;

} Pair;

void CreateNewPair(Pair* pair1, Pair* pair2, Pair* dest)
{
    const int* f1 = pair1->first;
    const int* s1 = pair1->second;
    const int* f2 = pair2->first;
    const int* s2 = pair2->second;

    const int* new_f = f1;
    const int* new_s = s1;
    const int* max_f = &(MAX_VALUE);

    if (*f1 > *f2)
    {
        new_f = f2;
    }
    else if (*f1 == *f2)
    {
        new_f = (f1 < f2) ? f1 : f2;
    }

    max_f = f1 + (f2 - new_f);

    if (*s1 > *s2)
    {
        new_s = s2;
    }
    else if (*s1 == *s2)
    {
        new_s = (s1 < s2) ? s1 : s2;
    }

    if (*new_s > *max_f)
    {
        if (max_f != new_f)
        {
            new_s = max_f;
        }
    }
    else if ((*new_s == *max_f) && (max_f < new_s))
    {
        if (max_f != new_f)
        {
            new_s = max_f;
        }
    }

    dest->first  = new_f;
    dest->second = new_s;
};

typedef struct SparseTable
{
    int*    log2;
    Pair**  data;
    int     size;

} SparseTable;

void SparseTableCtor(SparseTable* sparse_table, const int* array, int arr_size)
{
    sparse_table->size = arr_size;

    sparse_table->log2 = (int*) calloc(sparse_table->size + 1, sizeof(int));
    
    sparse_table->log2[0] = -1;

    for (int i = 1; i <= sparse_table->size; i++)
    {
        sparse_table->log2[i] = sparse_table->log2[i/2] + 1;
    }

    sparse_table->data = (Pair**) calloc(sparse_table->log2[sparse_table->size] + 1, sizeof(Pair*));

    for (int i = 0; i <= sparse_table->log2[sparse_table->size]; i++)
    {
        sparse_table->data[i] = (Pair*) calloc(sparse_table->size, sizeof(Pair));
    }

    for (int i = 0; i < sparse_table->size; i++)
    {
        sparse_table->data[0][i].first    = array + i;
        sparse_table->data[0][i].second   = &(MAX_VALUE);
    }

    for (int k = 1; k <= sparse_table->log2[sparse_table->size]; k++)
    {
        for (int i = 0; i + (1 << k) - 1 < sparse_table->size; i++) 
        {
            CreateNewPair(sparse_table->data[k-1] + i, sparse_table->data[k-1]+ i + (1 << (k-1)), sparse_table->data[k] + i);
        }
    }

};

int SparseTableGetMin(SparseTable* sparse_table, int left, int right)
{
    int k = sparse_table->log2[right - left + 1];
    Pair temp_pair = {};
    CreateNewPair(sparse_table->data[k] + left, sparse_table->data[k] + right - (1 << k) + 1, &temp_pair);
    return *(temp_pair.second);
};

void SparseTableDtor(SparseTable* sparse_table)
{
    for (int i = 0; i <= sparse_table->log2[sparse_table->size]; i++)
    {
        free(sparse_table->data[i]);
    }

    free(sparse_table->data);
    free(sparse_table->log2);

    sparse_table->data = NULL;
    sparse_table->log2 = NULL;
    sparse_table->size = 0;
};

int main()
{
    int n = 0;

    if (scanf("%d", &n) == 0)
    {
        assert(false && "ERROR!!! Program can not read the number!\n");
    }

    int q = 0;

    if (scanf("%d", &q) == 0)
    {
        assert(false && "ERROR!!! Program can not read the number!\n");
    }

    int* array = (int*) calloc(n, sizeof(int));
    assert((array != NULL) && "ERROR!!! Program can not allocate memory!\n");

    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", array + i) == 0)
        {
            assert(false && "ERROR!!! Program can not read the number!\n");
        }
    }

    SparseTable sparse_table = {};

    SparseTableCtor(&sparse_table, array, n);

    int left    = 0;
    int right   = 0;
    int answer  = 0;

    for (int i = 0; i < q; i++)
    {
        if (scanf("%d", &left) == 0)
        {
            assert(false && "ERROR!!! PRogram can not read the number!\n");
        }

        if (scanf("%d", &right) == 0)
        {
            assert(false && "ERROR!!! PRogram can not read the number!\n");
        }

        left--;
        right--;

        answer = SparseTableGetMin(&sparse_table, left, right);

        printf("%d\n", answer);
    }

    SparseTableDtor(&sparse_table);

    free(array);

    return 0;
}