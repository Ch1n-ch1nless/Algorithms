#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*===Structs_&_Constants===*/

typedef int elem_t;

#define elem_format "%d"

const int TRUE  = 1;
const int FALSE = 0;

/*===Function_Declaration===*/

long long   CountInversions(elem_t* const array, const int begin, const int end);
long long   Merge(elem_t* const array, const int begin, const int end);

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

    //Read elements in array
    for (int i = 0; i < array_size; i++)
    {
        if (scanf(elem_format, array + i) == 0)
        {
            assert(FALSE && "Program can not read the number:(\n");
        }   
    }

    //Sort all elements && count the number of the inversions
    long long inversions_number = CountInversions(array, 0, array_size - 1);

    //Print answer
    printf("%lld", inversions_number);

    return 0;
}

long long CountInversions(elem_t* const array, const int begin, const int end)
{
    assert((array != NULL)  && "Pointer to \"array\" is NULL!!!\n");
    assert((begin >= 0)     && "Function has incorrect value of \"begin\"!!!\n");
    assert((end >= begin)   && "Function has incorrect value of \"end\"!!\n");

    long long inversions_number = 0;

    if (begin < end)
    {
        inversions_number += CountInversions(array, begin, begin + (end - begin) / 2);
        inversions_number += CountInversions(array, begin + 1 + (end - begin) / 2, end);
        inversions_number += Merge(array, begin, end);
    }

    return inversions_number;
}

long long Merge(elem_t* const array, const int begin, const int end)
{
    assert((array != NULL)  && "Pointer to \"array\" is NULL!!!\n");
    assert((begin >= 0)     && "Function has incorrect value of \"begin\"!!!\n");
    assert((end >= begin)   && "Function has incorrect value of \"end\"!!\n");

    long long inversions_number = 0;

    int middle = begin + (end - begin) / 2;
    int left_index  = begin;
    int right_index = middle + 1;

    elem_t* sorted_array = (elem_t*) calloc(end - begin + 1, sizeof(elem_t));
    assert((sorted_array != NULL) && "Pointer to \'sorted_array\' is NULL!!!\n");

    int index = 0;

    while (left_index <= middle && right_index <= end)
    {
        if (array[left_index] < array[right_index])
        {
            sorted_array[index++] = array[left_index++];
        }
        else 
        {
            inversions_number += middle - left_index + 1;
            sorted_array[index++] = array[right_index++];
        }
    }

    while (left_index <= middle)
    {
        sorted_array[index++] = array[left_index++];
    }

    while (right_index <= end)
    {   
        sorted_array[index++] = array[right_index++];
    }

    for (int i = 0; i <= end - begin; i++)
    {
        array[i + begin] = sorted_array[i];
    }

    free(sorted_array);

    return inversions_number;
}