#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===Structs_&_Constants===*/

typedef struct Node
{
    int         index;
    int         request;
    long long   value;
} Node;

typedef struct MinHeap
{
    size_t      size;
    size_t      capacity;
    Node*       data;
    Node**      requests_array;
} MinHeap;

const int POISON_VALUE = INT_MAX;

const size_t START_CAPACITY = 1e7;

const int FALSE = 0;
const int TRUE  = 1;

const char* const INSERT_COMMAND   = "insert";
const size_t INSERT_LEN   = 6;
const char* const GETMIN_COMMAND   = "getMin";
const size_t GETMIN_LEN   = 6;
const char* const EXTRACT_COMMAND  = "extractMin";
const size_t EXTRACT_LEN  = 10;
const char* const DECREASE_COMMAND = "decreaseKey";
const size_t DECREASE_LEN = 11;

/*===Function_Declaration===*/

void        HeapCtor(MinHeap* const heap, const size_t start_capacity);
void        HeapDtor(MinHeap* const heap);

void        ReallocUp(MinHeap* const heap);
void        ReallocDown(MinHeap* const heap);

void        SwapNodes(MinHeap* const heap, size_t index1, size_t index2);

void        SiftDown(MinHeap* const heap, size_t index); 
void        SiftUp(MinHeap* const heap, size_t index);

void        HeapInsert(MinHeap* const heap, long long value, int request); 
long long   GetMin(MinHeap* const heap);
long long   ExtractMin(MinHeap* const heap);
void        DecreaseKey(MinHeap* const heap, int delta, int request);

void        ExecuteCommands(MinHeap* const heap);

//void        PrintHeap(MinHeap* const heap);

/*===Function_Definition===*/

int main()
{
    MinHeap my_min_heap = {};

    HeapCtor(&my_min_heap, START_CAPACITY);

    ExecuteCommands(&my_min_heap);

    HeapDtor(&my_min_heap);

    return 0;
}

void ExecuteCommands(MinHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");
    assert((heap->requests_array != NULL) && "Pointer to \'heap->requests_array\' is NULL!!!\n");

    char command[12];

    size_t      number_of_requests = 0;
    int         index  = 0;
    int         delta  = 0;
    long long   number = 0;
    int         insert_request = 0; 

    if (scanf("%ld", &number_of_requests) != 1)
    {
        assert(FALSE && "Program can not read the number!\n");
    }

    for (size_t i = 0; i < number_of_requests; i++)
    {
        if (scanf("%11s", command) == 0)
        {
            assert(FALSE && "Program can not read the string!\n");
        }

        if (strncmp(command, INSERT_COMMAND, INSERT_LEN) == 0)
        {
            if (scanf("%lld", &number) != 1)
            {
                assert(FALSE && "Program can not read the number!!!\n");
            }

            HeapInsert(heap, number, insert_request);
            insert_request++;
        }
        else if (strncmp(command, GETMIN_COMMAND, GETMIN_LEN) == 0)
        {
            number = GetMin(heap);
            printf("%lld\n", number);
        }
        else if (strncmp(command, EXTRACT_COMMAND, EXTRACT_LEN) == 0)
        {
            ExtractMin(heap);
        }
        else if (strncmp(command, DECREASE_COMMAND, DECREASE_LEN) == 0)
        {
            if (scanf("%d %d", &index, &delta) != 2)
            {
                assert(FALSE && "Program can not read the numbers!!!\n");
            }

            DecreaseKey(heap, delta, index-1);
        }
        else
        {
            assert(FALSE && "Illegal command!!!\n");
        }

        //PrintHeap(heap);
    }
}

void HeapCtor(MinHeap* const heap, const size_t start_capacity)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");

    heap->size     = 0;
    heap->capacity = start_capacity;

    heap->data = (Node*) calloc(start_capacity, sizeof(Node));
    assert((heap->data != NULL) && "Program can not allocate memory!\n");

    heap->requests_array = (Node**) calloc(start_capacity, sizeof(Node*));
    assert((heap->requests_array != NULL) && "Program can not allocate memory!\n");
}

void HeapDtor(MinHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");
    assert((heap->requests_array != NULL) && "Pointer to \'heap->requests_array\' is NULL!!!\n");

    free(heap->data);
    heap->data = NULL;

    free(heap->requests_array);
    heap->requests_array = NULL;

    heap->size     = 0;
    heap->capacity = 0;
}

void ReallocUp(MinHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");
    assert((heap->requests_array != NULL) && "Pointer to \'heap->requests_array\' is NULL!!!\n");

    heap->data = (Node*) realloc(heap->data, 2 * heap->capacity);
    assert((heap->data != NULL) && "Program can not allocate memory!\n");

    heap->capacity *= 2;

    heap->requests_array = (Node**) realloc(heap->requests_array, heap->capacity);
    assert((heap->requests_array != NULL) && "Program can not allocate memory!\n");
}

void SwapNodes(MinHeap* const heap, size_t index1, size_t index2)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");
    assert((heap->requests_array != NULL) && "Pointer to \'heap->requests_array\' is NULL!!!\n");

    heap->data[index1].index = index2;
    heap->data[index2].index = index1;

    Node temp_node = heap->data[index1];
    heap->data[index1] = heap->data[index2];
    heap->data[index2] = temp_node;

    Node* temp_ptr = heap->requests_array[heap->data[index1].request];
    heap->requests_array[heap->data[index1].request] = heap->requests_array[heap->data[index2].request];
    heap->requests_array[heap->data[index2].request] = temp_ptr;
}

void SiftDown(MinHeap* const heap, size_t index)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");
    assert((index <= heap->size) && "Incorrect value of index!\n");

    size_t left_child  = 2 * index + 1;
    size_t right_child = 2 * index + 2;
    size_t min_index   = index;

    if (left_child < heap->size && heap->data[left_child].value < heap->data[min_index].value)
    {
        min_index = left_child;
    }

    if (right_child < heap->size && heap->data[right_child].value < heap->data[min_index].value)
    {
        min_index = right_child;
    }

    if (min_index != index)
    {
        SwapNodes(heap, min_index, index);
        SiftDown(heap, min_index);
    }
}

void SiftUp(MinHeap* const heap, size_t index)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");
    assert((index <= heap->size) && "Incorrect value of index!\n");

    size_t parent = 0;

    while (index != 0)
    {
        parent = (index - 1) / 2;

        if (heap->data[parent].value > heap->data[index].value)
        {
            SwapNodes(heap, parent, index);
            index = parent;
        }
        else
        {
            break;
        }
    }
}

long long ExtractMin(MinHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");

    long long min_elem = heap->data[0].value;  

    SwapNodes(heap, 0, heap->size -1);
    heap->data[--heap->size].value = POISON_VALUE;
    heap->requests_array[heap->data[heap->size].request] = NULL;
    heap->data[heap->size].request = -1;
    heap->data[heap->size].index   = -1;

    SiftDown(heap, 0);

    return min_elem;
}

void HeapInsert(MinHeap* const heap, long long value, int request)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");
    assert((heap->requests_array != NULL) && "Pointer to \'heap->requests_array\' is NULL!!!\n");

    if (heap->size >= heap->capacity)
    {
        ReallocUp(heap);
    }

    heap->data[heap->size].value   = value;
    heap->data[heap->size].request = request;
    heap->data[heap->size].index   = heap->size;

    heap->requests_array[request] = &heap->data[heap->size];

    heap->size++;

    SiftUp(heap, heap->size - 1);
}

long long GetMin(MinHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");
    assert((heap->requests_array != NULL) && "Pointer to \'heap->requests_array\' is NULL!!!\n");

    return heap->data[0].value;
}


void DecreaseKey(MinHeap* const heap, int delta, int request)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");
    assert((heap->requests_array != NULL) && "Pointer to \'heap->requests_array\' is NULL!!!\n");

    int index = (heap->requests_array[request])->index;

    heap->data[index].value -= delta;

    SiftUp(heap, index);
}

/*void PrintHeap(MinHeap* const heap)
{
    assert((heap != NULL) && "Pointer to \'heap\' is NULL!!!\n");
    assert((heap->data != NULL) && "Pointer to \'heap->data\' is NULL!!!\n");
    assert((heap->requests_array != NULL) && "Pointer to \'heap->requests_array\' is NULL!!!\n");

    for (int i = 0; i < heap->size; i++)
    {
        printf("|%6d|", i);
    }
    printf("\n");

    for (int i = 0; i < heap->size; i++)
    {
        printf("|%6d|", heap->data[i].value);
    }
    printf("\n");
    for (int i = 0; i < heap->size; i++)
    {
        printf("|%6d|", heap->data[i].request);
    }
    printf("\n");

    for (int i = 0; i < 10; i++)
    {
        printf("|%6d|", heap->requests_array[i]);
    }
    printf("\n==============================================\n");
}*/