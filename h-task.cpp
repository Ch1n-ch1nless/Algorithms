#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*===Structs_&_Constants===*/

typedef int elem_t;

#define elem_format "%d"

const int TRUE  = 1;
const int FALSE = 0;

/*===Function_Declaration===*/

void    QuickSort(elem_t* const array, int begin, int end);
void    HoarePartition(elem_t* const array, int begin, int end, elem_t pivot, int* const new_begin, int* const new_end);
elem_t  GetPivot(const elem_t* const array, int begin, int end);
void    SwapValues(elem_t* const ptr1, elem_t* const ptr2);
void    ReadArrayElements(elem_t* const array);

/*===Function_Definition===*/

int main()
{
    int array_size = 0;

    //Read size of the array
    if (scanf("%d", &array_size) == 0)
    {
        assert(FALSE && "Program can not read the number:(\n");
    }

    //Create array
    elem_t* array = (int*) calloc(array_size, sizeof(elem_t));
    if (array == NULL)
    {
        assert(FALSE && "Program can not allocate memory for array:(\n");
    }

    ReadArrayElements(array);

    //Sort all elements
    QuickSort(array, 0, array_size - 1);

    //Print elements in array
    for (int i = 0; i < array_size; i++)
    {
        printf(elem_format " ", array[i]);
    }

    return 0;
}

void ReadArrayElements(elem_t* const array)
{    
    assert((array != NULL) && "Pointer to \'array\' is NULL!!!\n");
    
    for (int i = 0; i < array_size; i++)
    {
        if (scanf(elem_format, array + i) == 0)
        {
            assert(FALSE && "Program can not read the number:(\n");
        }   
    }
}

void QuickSort(elem_t* const array, const int begin, const int end)
{
    assert((array != NULL)  && "Pointer to \"array\" is NULL!!!\n");
    assert((begin >= 0)     && "Function has incorrect value of \"begin\"!!!\n");
    assert((end >= begin)   && "Function has incorrect value of \"end\"!!\n");

    elem_t pivot = GetPivot(array, begin, end);

    int new_begin = 0;
    int new_end   = 0;

    HoarePartition(array, begin, end, pivot, &new_begin, &new_end);

    if (new_end > begin)
    {
        QuickSort(array, begin, new_end);
    }

    if (new_begin < end)
    {
        QuickSort(array, new_begin, end);
    }
}

void HoarePartition(elem_t* const array, const int begin, const int end, const elem_t pivot, int* const new_begin, int* const new_end)
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

    int mid_index = begin + rand() % (end - begin);

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
