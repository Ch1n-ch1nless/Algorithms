#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*===Structs_&_Constants===*/

typedef int elem_t;

#define elem_format "%d"

const int TRUE  = 1;
const int FALSE = 0;

/*===Function_Declaration===*/

elem_t  FingKthOrdinalStatics(elem_t* const array, const int begin, const int end, const int k);
void    MyPartition(elem_t* const array, const int begin, const int end, const elem_t pivot, int* const new_begin, int* const new_end);
elem_t  GetPivot(const elem_t* const array, const int begin, const int end);
void    SwapValues(elem_t* const ptr1, elem_t* const ptr2);

/*===Function_Definition===*/

int main()
{
    int array_size = 0;
    int k = 0;
    int a0 = 0;
    int a1 = 0;

    //Read the numbers
    if (scanf("%d %d %d %d", &array_size, &k, &a0, &a1) == 0)
    {
        assert(FALSE && "Program can not read the number:(\n");
    }
    k--;

    //Create array
    elem_t* array = (int*) calloc(array_size, sizeof(elem_t));
    if (array == NULL)
    {
        assert(FALSE && "Program can not allocate memory for array:(\n");
    }

    array[0] = a0;
    array[1] = a1;

    //Fill array
    for (int i = 2; i < array_size; i++)
    {
        array[i] = (array[i-1] * 123 + array[i-2] * 45) % (10004321);
    }

    //Sort all elements
    int answer = FingKthOrdinalStatics(array, 0, array_size - 1, k);

    //Print elements in array
    printf(elem_format, answer);

    return 0;
}

elem_t FingKthOrdinalStatics(elem_t* const array, const int begin, const int end, const int k)
{
    assert((array != NULL)  && "Pointer to \"array\" is NULL!!!\n");
    assert((begin >= 0)     && "Function has incorrect value of \"begin\"!!!\n");
    assert((end >= begin)   && "Function has incorrect value of \"end\"!!\n");

    elem_t pivot = GetPivot(array, begin, end);

    int new_begin = 0;
    int new_end   = 0;

    MyPartition(array, begin, end, pivot, &new_begin, &new_end);

    if (new_end < k && k < new_begin)
    {
        return array[k];
    }

    if (begin <= k && k <= new_end)
    {
        return FingKthOrdinalStatics(array, begin, new_end, k);
    }

    return FingKthOrdinalStatics(array, new_begin, end, k);
}

void MyPartition(elem_t* const array, const int begin, const int end, const elem_t pivot, int* const new_begin, int* const new_end)
{
    assert((array != NULL)  && "Pointer to \"array\" is NULL!!!\n");
    assert((begin >= 0)     && "Function has incorrect value of \"begin\"!!!\n");
    assert((end >= begin)   && "Function has incorrect value of \"end\"!!\n");
    assert((new_begin != NULL)  && "Pointer to \"new_begin\" is NULL!!!\n");
    assert((new_end   != NULL)  && "Pointer to \"new_end\"   is NULL!!!\n");

    *new_begin  = begin;
    *new_end    = end;

    while ((*new_begin) <= (*new_end))
    {
        while (array[*new_begin] < pivot) { (*new_begin)++; }
        while (array[*new_end] > pivot)   { (*new_end)--; }

        if (*new_begin <= *new_end)
        {
            SwapValues(array + *new_begin, array + *new_end);
            (*new_begin)++;
            (*new_end)--;
        }
    }
}

elem_t  GetPivot(const elem_t* const array, const int begin, const int end)
{
    assert((array != NULL)  && "Pointer to \"array\" is NULL!!!\n");
    assert((begin >= 0)     && "Function has incorrect value of \"begin\"!!!\n");
    assert((end >= begin)   && "Function has incorrect value of \"end\"!!\n");

    int mid_index = begin + (end - begin) / 2;

    return array[mid_index];
}

void SwapValues(elem_t* const ptr1, elem_t* const ptr2)
{
    assert((ptr1 != NULL) && "Pointer to \"ptr1\" is NULL!!!\n");
    assert((ptr2 != NULL) && "Pointer to \"ptr2\" is NULL!!!\n");

    elem_t temp_value = *ptr2;
    *ptr2 = *ptr1;
    *ptr1 = temp_value;
}