#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

/*===Structs_&_Constants===*/

typedef struct MaxHeap 
{
    size_t size;
    int*   elem;
} MaxHeap;

const int POISON_VALUE = -1024;

const size_t MAX_HEAP_SIZE = 1e7;

const int MOD = 1 << 30;

/*===Function_Declaration===*/

int GetNextElem(const int old_elem, const int x, const int y);

void HeapCtor(MaxHeap* const heap, const size_t number_of_elements);
void HeapDtor(MaxHeap* const heap);

void SwapNodes(int* const node1, int* const node2);

void SiftDown(MaxHeap* const heap, size_t index);
void SiftUp(MaxHeap* const heap, size_t index);

int  ExtractMax(MaxHeap* const heap);
void InsertElem(MaxHeap* const heap, int element);

/*===Function_Definition===*/

int main()
{
    int n = 0;      
    int k = 0;

    if (scanf("%d %d", &n, &k) != 2)
    {
        assert(0 && "Program can not read 2 numbers!\n");
    }

    int a0 = 0;
    int x = 0;
    int y = 0;

    if (scanf("%d %d %d", &a0, &x, &y) != 3)
    {
        assert(0 && "Program can not read 3 numbers!\n");
    }

    int cur_elem = a0;

    MaxHeap my_heap = {};
    HeapCtor(&my_heap, MAX_HEAP_SIZE);

    for (int i = 0; i < k; i++)
    {
        cur_elem = GetNextElem(cur_elem, x, y);
        InsertElem(&my_heap, cur_elem);
    }

    for (int i = k; i < n; i++)
    {
        cur_elem = GetNextElem(cur_elem, x, y);

        if (cur_elem < my_heap.elem[0])
        {
            InsertElem(&my_heap, cur_elem);
            ExtractMax(&my_heap);
        }
    }

    int* k_min_elems = (int*) calloc(k, sizeof(int));
    assert((k_min_elems != NULL) && "Pointer to \'k\' is NULL!!!\n");

    for (int i = 0; i < k; i++)
    {
        k_min_elems[i] = ExtractMax(&my_heap);
    }

    for (int i = k - 1; i > -1; i--)
    {
        printf("%d ", k_min_elems[i]);
    }

    free(k_min_elems);
    HeapDtor(&my_heap);

    return 0;
}

int GetNextElem(const int old_elem, const int x, const int y)
{
    long long temp_res = (long long)old_elem * (long long)x + (long long)y;
    int result = (int)(temp_res % MOD);

    return result;
}

void HeapCtor(MaxHeap* const heap, const size_t number_of_elements)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");

    heap->elem = (int*) calloc(number_of_elements, sizeof(int));
    heap->size = 0;
    assert((heap->elem != NULL) && "Program can not allocate memory for heap!\n");
}

void HeapDtor(MaxHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->elem != NULL) && "Pointer to \'heap->elem\' is NULL!!!\n");

    free(heap->elem);
    heap->elem = NULL;
    heap->size = 0;
}

void SwapNodes(int* const node1, int* const node2)
{
    assert((node1 != NULL) && "Pointer to \'node1\' is NULL!!!\n");
    assert((node2 != NULL) && "Pointer to \'node2\' is NULL!!!\n");

    int temp_var = *node1;
    *node1 = *node2;
    *node2 = temp_var;
}

void SiftDown(MaxHeap* const heap, size_t index)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->elem != NULL) && "Pointer to \'heap->elem\' is NULL!!!\n");
    assert((index <= heap->size) && "Incorrect value of index!\n");

    size_t left_child  = 2 * index + 1;
    size_t right_child = 2 * index + 2;
    size_t max_index   = index;

    if (left_child < heap->size && heap->elem[left_child] > heap->elem[max_index])
    {
        max_index = left_child;
    }

    if (right_child < heap->size && heap->elem[right_child] > heap->elem[max_index])
    {
        max_index = right_child;
    }

    if (max_index != index)
    {
        SwapNodes(heap->elem + max_index, heap->elem + index);
        SiftDown(heap, max_index);
    }
}

void SiftUp(MaxHeap* const heap, size_t index)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->elem != NULL) && "Pointer to \'heap->elem\' is NULL!!!\n");
    assert((index <= heap->size) && "Incorrect value of index!\n");

    size_t parent = 0;

    while (index != 0)
    {
        parent = (index - 1) / 2;

        if (heap->elem[parent] < heap->elem[index])
        {
            SwapNodes(heap->elem + parent, heap->elem + index);
            index = parent;
        }
        else
        {
            break;
        }
    }
}

int  ExtractMax(MaxHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->elem != NULL) && "Pointer to \'heap->elem\' is NULL!!!\n");

    int max_elem = heap->elem[0];

    SwapNodes(heap->elem, heap->elem + heap->size -1);
    heap->elem[--heap->size] = POISON_VALUE;

    SiftDown(heap, 0);

    return max_elem;
}

void InsertElem(MaxHeap* const heap, int element)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->elem != NULL) && "Pointer to \'heap->elem\' is NULL!!!\n"); 

    heap->elem[heap->size++] = element;
    SiftUp(heap, heap->size - 1);
}