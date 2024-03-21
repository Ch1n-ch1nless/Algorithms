#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*===Structs_&_Constants===*/

typedef struct MinMaxHeap 
{
    int* data;
    int  size;
} MinMaxHeap;

const int INCORRECT_VALUE = -1;

const int TRUE  = 1;
const int FALSE = 0;

const int ERROR = 0;
const int OK    = 1;

const int MIN_LEVEL = 1;
const int MAX_LEVEL = 0;

const int START_CAPACITY = 2 * 1e5;

/*===Function_Declaration===*/

void Swap(int* const ptr1, int* const ptr2);

int  IsLevel(int index);
int  GetHeapParent(int index);
int  GetLevelParent(int index);
int  GetMinHeapChild(int index, const MinMaxHeap* const heap);
int  GetMaxHeapChild(int index, const MinMaxHeap* const heap);
int  GetMinLevelChild(int index, const MinMaxHeap* const heap);
int  GetMaxLevelChild(int index, const MinMaxHeap* const heap);

int  MinMaxHeapCtor(MinMaxHeap* const heap, const int start_capacity);
int  MinMaxHeapDtor(MinMaxHeap* const heap);

int  SiftUp(MinMaxHeap* const heap, int index);
int  SiftDown(MinMaxHeap* const heap, int index);

int  MinMaxHeapInsert(MinMaxHeap* const heap, int new_elem);

int  ExtractMin(MinMaxHeap* const heap, int* const buffer);
int  ExtractMax(MinMaxHeap* const heap, int* const buffer);
int  GetMin(MinMaxHeap* const heap, int* const buffer);
int  GetMax(MinMaxHeap* const heap, int* const buffer);

/*===Function_Definition===*/

int main()
{
    return 0;
}

/*==========Secondary_functions==========*/

void Swap(int* const ptr1, int* const ptr2)
{
    int temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

int IsLevel(int index)
{
    int max_pow_of_two = 0;
    index += 1;

    while(index != 0)
    {
        index >>= 1;
        max_pow_of_two++;
    }   

    return max_pow_of_two % 2;
}

int GetHeapParent(int index)
{
    if (index < 1)
    {
        return 0;
    }

    return (index - 1) / 2;
}

int GetLevelParent(int index)
{
    if (index < 3)
    {
        return 0;
    }

    return GetHeapParent(GetHeapParent(index));
}

int GetMinHeapChild(int index, const MinMaxHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");

    int left_child  = index * 2 + 1;
    int right_child = index * 2 + 2;

    if (left_child >= heap->size)
    {
        return INCORRECT_VALUE;
    }

    if (right_child >= heap->size)
    {
        return left_child;
    }

    return (heap->data[left_child] < heap->data[right_child]) ? left_child : right_child;
}

int GetMaxHeapChild(int index, const MinMaxHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");

    int left_child  = index * 2 + 1;
    int right_child = index * 2 + 2;

    if (left_child >= heap->size)
    {
        return INCORRECT_VALUE;
    }

    if (right_child >= heap->size)
    {
        return left_child;
    }

    return (heap->data[left_child] > heap->data[right_child]) ? left_child : right_child;
}

int GetMinLevelChild(int index, const MinMaxHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");

    int left_child  = GetMinChild(index * 2 + 1);
    int right_child = GetMinChild(index * 2 + 2);

    if (left_child == INCORRECT_VALUE)
    {
        return INCORRECT_VALUE;
    }

    if (right_child == INCORRECT_VALUE)
    {
        return left_child;
    }

    return (heap->data[left_child] < heap->data[right_child]) ? left_child : right_child;
}

int GetMaxLevelChild(int index, const MinMaxHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");

    int left_child  = GetMinChild(index * 2 + 1);
    int right_child = GetMinChild(index * 2 + 2);

    if (left_child == INCORRECT_VALUE)
    {
        return INCORRECT_VALUE;
    }

    if (right_child == INCORRECT_VALUE)
    {
        return left_child;
    }

    return (heap->data[left_child] > heap->data[right_child]) ? left_child : right_child;
}

/*=======================================*/

int MinMaxHeapCtor(MinMaxHeap* const heap, const int start_capacity)
{
    assert((heap != NULL) && "Pointer to heap is NULL!!!\n");
    assert((start_capacity > 0) && "Incorrect value of \'start_capacity\'!!!\n");

    heap->size = 0;

    heap->data = (int*) calloc(start_capacity, sizeof(int));
    assert((heap->data != NULL) && "Program can not allocate memory!!!\n");

    return OK;
}

int MinMaxHeapDtor(MinMaxHeap* const heap)
{
    assert((heap != NULL) && "Pointer to heap is NULL!!!\n");

    free(heap->data);
    
    heap->data = NULL;
    heap->size = 0;

    return OK;
}

int SiftUp(MinMaxHeap* const heap, int index)
{
    assert((heap != NULL) && "Pointer to heap is NULL!!!\n");
    assert((-1 < index && index < heap->size) && "Incorrect value of index!\n");

    int cur_parent   = 0;
    int level_parent = 0;
    int is_index_changed = 0;

    while (index != 0)
    {
        cur_parent   = GetHeapParent(index);
        level_parent = GetLevelParent(index);

        if (IsLevel(index) == MIN_LEVEL)
        {
            if (heap->data[cur_parent] < heap->data[index])
            {
                Swap(heap->data + cur_parent, heap->data + index);
                index = parent;
                is_index_changed = 1;
            }

            if (heap->data[level_parent] > heap->data[index])
            {
                Swap(heap->data + level_parent, heap->data + index);
                index = level_parent;
                is_index_changed = 1;
            }
        }
        else
        {
            if (heap->data[cur_parent] > heap->data[index])
            {
                Swap(heap->data + cur_parent, heap->data + index);
                index = parent;
                is_index_changed = 1;
            }

            if (heap->data[level_parent] < heap->data[index])
            {
                Swap(heap->data + level_parent, heap->data + index);
                index = level_parent;
                is_index_changed = 1;
            }
        }

        if (!is_index_changed)
        {
            break;
        }
        is_index_changed = 0;
    }

    return OK;
}

int SiftDown(MinMaxHeap* const heap, int index)
{
    assert((heap != NULL) && "Pointer to heap is NULL!!!\n");
    assert((-1 < index && index < heap->size) && "Incorrect value of index!\n");

    int heap_child       = 0;
    int level_child      = 0;
    int is_index_changed = 0;

    while (index * 2 + 1 < heap->size)
    {
        if (IsLevel(index) == MIN_LEVEL)
        {
            heap_child  = GetMinHeapChild(index, heap);
            level_child = GetMinLevelChild(index, heap);

            if ((heap_child != -1) && (heap->data[index] > heap->data[heap_child]))
            {
                Swap(heap->data + index, heap->data + heap_child);
                index = heap_child;
                is_index_changed = 1;
            }

            if ((level_child != -1) && (heap->data[index] > heap->data[level_child]))
            {
                Swap(heap->data + index, heap->data + level_child);
                index = level_child;
                is_index_changed = 1;
            }
        }
        else
        {
            heap_child  = GetMaxHeapChild(index, heap);
            level_child = GetMaxLevelChild(index, heap);

            if ((heap_child != -1) && (heap->data[index] < heap->data[heap_child]))
            {
                Swap(heap->data + index, heap->data + heap_child);
                index = heap_child;
                is_index_changed = 1;
            }

            if ((level_child != -1) && (heap->data[index] < heap->data[level_child]))
            {
                Swap(heap->data + index, heap->data + level_child);
                index = level_child;
                is_index_changed = 1;
            }
        }

        if (!is_index_changed)
        {
            break;
        }
        is_index_changed = 0;
    }

    return OK;
}

int MinMaxHeapInsert(MinMaxHeap* const heap, int new_elem)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");

    heap->data[heap->size] = new_elem;
    heap->size++;

    SiftUp(heap, heap->size - 1);
}

int ExtractMin(MinMaxHeap* const heap, int* const buffer)
{
    
}

int ExtractMax(MinMaxHeap* const heap, int* const buffer)
{

}

int GetMin(MinMaxHeap* const heap, int* const buffer)
{

}

int GetMax(MinMaxHeap* const heap, int* const buffer)
{

}
