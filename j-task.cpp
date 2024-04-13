#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===Structs_&_Constants===*/

typedef struct MinMaxHeap 
{
    int* data;
    int  size;
} MinMaxHeap;

const int MAX_CMD_LEN = 16;

const int INCORRECT_VALUE = -1;

const int TRUE  = 1;
const int FALSE = 0;

const int ERROR = 0;
const int OK    = 1;

const int MIN_LEVEL = 1;
const int MAX_LEVEL = 0;

const int START_CAPACITY = 2 * 1e6;

const char* const INSERT_CMD            = "insert";
const int         INSERT_CMD_LEN        = 6;
const char* const EXTRACT_MIN_CMD       = "extract_min";
const int         EXTRACT_MIN_CMD_LEN   = 11;
const char* const EXTRACT_MAX_CMD       = "extract_max";
const int         EXTRACT_MAX_CMD_LEN   = 11;
const char* const GET_MIN_CMD           = "get_min";
const int         GET_MIN_CMD_LEN       = 7;
const char* const GET_MAX_CMD           = "get_max";
const int         GET_MAX_CMD_LEN       = 7;
const char* const SIZE_CMD              = "size";
const int         SIZE_CMD_LEN          = 4;
const char* const CLEAR_CMD             = "clear";
const int         CLEAR_CMD_LEN         = 5;

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

int  MinMaxHeapClear(MinMaxHeap* const heap);

void ExecuteCommand(MinMaxHeap* const heap);


/*===Function_Definition===*/

int main()
{
    MinMaxHeap my_heap = {};

    MinMaxHeapCtor(&my_heap, START_CAPACITY);

    int number_of_commands = 0;

    if (scanf("%d", &number_of_commands) != 1)
    {
        assert(FALSE && "Program can not read the number!\n");
    }

    for (int i = 0; i < number_of_commands; i++)
    {
        ExecuteCommand(&my_heap);
    }

    MinMaxHeapDtor(&my_heap);

    return 0;
}

void ExecuteCommand(MinMaxHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");

    char command[MAX_CMD_LEN];
    int  buffer = 0;

    if (scanf("%11s", command) == 0)
    {
        assert(FALSE && "Program can not read the string!\n");
    }

    if (strncmp(command, INSERT_CMD, INSERT_CMD_LEN) == 0)
    {
        if (scanf("%d", &buffer) != 1)
        {
            assert(FALSE && "Program can not read the number!\n");
        }

        if (MinMaxHeapInsert(heap, buffer) == OK)
        {
            printf("ok\n");
        }
        else
        {
            printf("error\n");
        }
    }
    else if (strncmp(command, EXTRACT_MIN_CMD, EXTRACT_MIN_CMD_LEN) == 0)
    {
        if (ExtractMin(heap, &buffer) == OK)
        {
            printf("%d\n", buffer);
        }
        else
        {
            printf("error\n");
        }
    }
    else if (strncmp(command, EXTRACT_MAX_CMD, EXTRACT_MAX_CMD_LEN) == 0)
    {
        if (ExtractMax(heap, &buffer) == OK)
        {
            printf("%d\n", buffer);
        }
        else
        {
            printf("error\n");
        }
    }
    else if (strncmp(command, GET_MIN_CMD, GET_MIN_CMD_LEN) == 0)
    {
        if (GetMin(heap, &buffer) == OK)
        {
            printf("%d\n", buffer);
        }
        else
        {
            printf("error\n");
        }
    }
    else if (strncmp(command, GET_MAX_CMD, GET_MAX_CMD_LEN) == 0)
    {
        if (GetMax(heap, &buffer) == OK)
        {
            printf("%d\n", buffer);
        }
        else
        {
            printf("error\n");
        }
    }
    else if (strncmp(command, SIZE_CMD, SIZE_CMD_LEN) == 0)
    {
        printf("%d\n", heap->size);
    }
    else if (strncmp(command, CLEAR_CMD, CLEAR_CMD_LEN) == 0)
    {
        MinMaxHeapClear(heap);
        printf("ok\n");
    }
    else
    {
        assert(FALSE && "Illegal command!!!");
    }
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

    int left_child  = GetMinHeapChild(index * 2 + 1, heap);
    int right_child = GetMinHeapChild(index * 2 + 2, heap);

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

    int left_child  = GetMaxHeapChild(index * 2 + 1, heap);
    int right_child = GetMaxHeapChild(index * 2 + 2, heap);

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
    assert((-1 < index && index <= heap->size) && "Incorrect value of index!\n");

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
                index = cur_parent;
                is_index_changed = 1;
            }
            else if ((level_parent != cur_parent) && heap->data[level_parent] > heap->data[index])
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
                index = cur_parent;
                is_index_changed = 1;
            }
            else if ((level_parent != cur_parent) && heap->data[level_parent] < heap->data[index])
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
    assert((-1 < index && index <= heap->size) && "Incorrect value of index!\n");

    int heap_child       = 0;
    int level_child      = 0;
    int is_index_changed = 0;

    while (index * 2 + 1 < heap->size)
    {
        if (IsLevel(index) == MIN_LEVEL)
        {
            heap_child  = GetMinHeapChild(index, heap);
            level_child = GetMinLevelChild(index, heap);

            if (level_child != -1)
            {
                if ((heap->data[heap_child] < heap->data[level_child]) && (heap->data[heap_child] < heap->data[index]))
                {
                    Swap(heap->data + index, heap->data + heap_child);
                    index = heap_child;
                    is_index_changed = 1;

                    if (heap->data[level_child] < heap->data[index])
                    {   
                        Swap(heap->data + index, heap->data + level_child);
                    }
                }
                else if (heap->data[level_child] < heap->data[index])
                {
                    Swap(heap->data + index, heap->data + level_child);
                    index = level_child;
                    is_index_changed = 1;

                    if (heap->data[heap_child] < heap->data[index])
                    {   
                        Swap(heap->data + index, heap->data + heap_child);
                    }
                }
            }
            else if ((heap_child != -1) && (heap->data[heap_child] < heap->data[index]))
            {
                Swap(heap->data + index, heap->data + heap_child);
                index = heap_child;
                is_index_changed = 1;
            }
        }
        else
        {
            heap_child  = GetMaxHeapChild(index, heap);
            level_child = GetMaxLevelChild(index, heap);

            if (level_child != -1)
            {
                if ((heap->data[heap_child] > heap->data[level_child]) && (heap->data[heap_child] > heap->data[index]))
                {
                    Swap(heap->data + index, heap->data + heap_child);
                    index = heap_child;
                    is_index_changed = 1;

                    if (heap->data[level_child] > heap->data[index])
                    {   
                        Swap(heap->data + index, heap->data + level_child);
                    }
                }
                else if (heap->data[level_child] > heap->data[index])
                {
                    Swap(heap->data + index, heap->data + level_child);
                    index = level_child;
                    is_index_changed = 1;

                    if (heap->data[heap_child] > heap->data[index])
                    {   
                        Swap(heap->data + index, heap->data + heap_child);
                    }
                }
            }
            else if ((heap_child != -1) && (heap->data[heap_child] > heap->data[index]))
            {
                Swap(heap->data + index, heap->data + heap_child);
                index = heap_child;
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

    return OK;
}

int ExtractMin(MinMaxHeap* const heap, int* const buffer)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");

    if (heap->size == 0)
    {   
        return ERROR;
    }

    heap->size--;
    *buffer = heap->data[0];
    heap->data[0] = heap->data[heap->size];
    SiftDown(heap, 0);

    return OK;
}

int ExtractMax(MinMaxHeap* const heap, int* const buffer)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");

    if (heap->size == 0)
    {   
        return ERROR;
    }
    else if (heap->size == 1)
    {
        heap->size--;
        *buffer = heap->data[0];
        return OK;
    }
    else if (heap->size == 2)
    {
        heap->size--;
        *buffer = heap->data[1];
        return OK;
    }

    heap->size--;

    if (heap->data[1] > heap->data[2])
    {
        *buffer = heap->data[1];
        heap->data[1] = heap->data[heap->size];
        SiftDown(heap, 1);
    }
    else
    {
        *buffer = heap->data[2];
        heap->data[2] = heap->data[heap->size];
        SiftDown(heap, 2);
    }

    return OK;
}

int GetMin(MinMaxHeap* const heap, int* const buffer)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");

    if (heap->size == 0)
    {   
        return ERROR;
    }

    *buffer = heap->data[0];

    return OK;
}

int GetMax(MinMaxHeap* const heap, int* const buffer)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");

    if (heap->size == 0)
    {   
        return ERROR;
    }
    else if (heap->size == 1)
    {
        *buffer = heap->data[0];
        return OK;
    }
    else if (heap->size == 2)
    {
        *buffer = heap->data[1];
        return OK;
    }

    *buffer = (heap->data[1] < heap->data[2]) ? heap->data[2] : heap->data[1];
    return OK;
}

int MinMaxHeapClear(MinMaxHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!");

    for (int i = 0; i < heap->size; i++)
    {
        heap->data[i] = 0;
    }

    heap->size = 0;

    return OK;
}
