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

const int ERROR = 0;
const int OK    = 1;

const int MIN_LEVEL = 1;
const int MAX_LEVEL = 0;

const int START_CAPACITY = 2 * 1e5;

/*===Function_Declaration===*/

int Swap(int* const ptr1, int* const ptr2);

int IsLevel(int index);
int GetHeapParent(int index);
int GetLevelParent(int index);
int GetLeftChild(int index);
int GetRightChild(int index);

int MinMaxHeapCtor(MinMaxHeap* const heap, const int start_capacity);
int MinMaxHeapDtor(MinMaxHeap* const heap);

int SiftUp(MinMaxHeap* const heap, int index);
int SiftDown(MinMaxHeap* const heap, int index);

int MinMaxHeapInsert(MinMaxHeap* const heap, int new_elem);

int ExtractMin(MinMaxHeap* const heap, int* const buffer);
int ExtractMax(MinMaxHeap* const heap, int* const buffer);
int GetMin(MinMaxHeap* const heap, int* const buffer);
int GetMax(MinMaxHeap* const heap, int* const buffer);

/*===Function_Definition===*/

int main()
{
    return 0;
}

/*=====Secondary_functions=====*/

int Swap(int* const ptr1, int* const ptr2)
{

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

int GetLeftChild(int index)
{
    return index * 2 + 1;
}

int GetRightChild(int index)
{
    return index * 2 + 2;
}

/*=============================*/

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
    
}

int MinMaxHeapInsert(MinMaxHeap* const heap, int new_elem)
{

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
