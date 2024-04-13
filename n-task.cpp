#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*===Structs_&_Constants===*/

const int FALSE = 0;
const int TRUE  = 1;

/*===Function_Declaration===*/

int MaxElem(int first_elem, int second_elem);

void*   ArrayCtor(const int array_size, const int size_of_elem);
void    ArrayDtor(void* array);
void    ArrayOfArraysDtor(void** array, const int number_of_elements);

int*    ReadArray(const int array_size);
int**   ReadArrays(const int number_of_arrays, const int array_size); 
int     BinSearchMinMax(int* const first_array, int* const second_array, const int len);

/*===Function_Definition===*/

int main()
{
    int n = 0;
    int m = 0;
    int l = 0;

    // Read 3 numbers
    if (scanf("%d %d %d", &n, &m, &l) != 3)
    {
        assert(FALSE && "Program can not read the 3 numbers!\n");
    }

    int** stillage_A = ReadArrays(n, l);
    int** stillage_B = ReadArrays(m, l);

    int requests_number = 0;
    int A_index = 0;
    int B_index = 0;
    int answer  = 0;

    //Read number of requests
    if (scanf("%d", &requests_number) != 1)
    {
        assert(FALSE && "Program can not read the 1 number!\n");
    }

    for (int i = 0; i < requests_number; i++)
    {
        if (scanf("%d %d", &A_index, &B_index) != 2)
        {
            assert(FALSE && "Program can not read the 2 numbers!\n");
        }

        answer = BinSearchMinMax(stillage_A[A_index-1], stillage_B[B_index-1], l);
        printf("%d\n", answer);
    }

    ArrayOfArraysDtor((void**)stillage_A, n);
    ArrayOfArraysDtor((void**)stillage_B, m);

    return 0;
}

int MaxElem(int first_elem, int second_elem)
{
    return (first_elem < second_elem) ? second_elem : first_elem;
}

void* ArrayCtor(const int array_size, const int size_of_elem)
{
    assert(array_size   > 0);
    assert(size_of_elem > 0);

    void* ptr = calloc(array_size, size_of_elem);
    assert((ptr != NULL) && "Program can not allocate memory!");

    return ptr;
}

void ArrayDtor(void* array)
{
    assert((array != NULL) && "Pointer to \'array\' is NULL!!!\n");
    free(array);
}

void ArrayOfArraysDtor(void** array, const int number_of_elements)
{
    assert(array != NULL);
    assert(number_of_elements > 0);

    for (int i = 0; i < number_of_elements; i++)
    {
        ArrayDtor(array[i]);
    }

    free(array);
}

int* ReadArray(const int array_size)
{
    assert(array_size > 0);

    int* array = (int*) calloc(array_size, sizeof(int));

    for (int i = 0; i < array_size; i++)
    {
        if (scanf("%d", array + i) != 1)
        {
            assert(FALSE && "Program can not read the number!\n");
        }
    }

    return array;
}

int** ReadArrays(const int number_of_arrays, const int array_size)
{
    assert(number_of_arrays > 0);
    assert(array_size > 0);

    int** array_of_arrays = (int**) ArrayCtor(number_of_arrays, sizeof(int*));

    for (int i = 0; i < number_of_arrays; i++)
    {
        array_of_arrays[i] = ReadArray(array_size);
    }

    return array_of_arrays;
}

int BinSearchMinMax(int* const first_array, int* const second_array, const int len)
{
    assert(first_array  != NULL);
    assert(second_array != NULL);
    assert(len > 0);

    int left_index  = -1;
    int right_index = len;
    int mid_index   = 0;

    while (right_index - left_index > 1)
    {
        mid_index = (left_index + right_index) / 2;
        if (first_array[mid_index] > second_array[mid_index])
        {
            right_index = mid_index;
        }
        else
        {
            left_index = mid_index;
        }
    }

    if (right_index < len && left_index >= 0)
    {
        if (MaxElem(first_array[left_index], second_array[left_index]) > MaxElem(first_array[right_index], second_array[right_index]))
        {
            return right_index + 1;
        }
        return left_index + 1;
    }
    else if (right_index >= len)
    {
        return left_index + 1;
    }
    return right_index + 1;
}