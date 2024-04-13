#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*===Structs_&_Constants===*/

const int MAX_DISTANCE = 2 * 1e9 + 2;

const int YES = 1;
const int NO  = 0;

/*===Function_Declaration===*/

int Compare(const void* x1, const void* x2);
int FindMinLenForLumosSpell(const int number_of_receivers, const int max_number_of_spells, int* const coordinate_array);
int CoverAllReceivers(const int number_of_receivers, const int max_number_of_spells, const int mid, int* const coordinate_array);

/*===Function_Definition===*/

int main()
{
    int number_of_receivers  = 0;
    int max_number_of_spells = 0;

    scanf("%d %d", &number_of_receivers, &max_number_of_spells);

    int* coordinate_array = (int*) calloc(number_of_receivers, sizeof(int));

    for (int i = 0; i < number_of_receivers; i++)
    {
        scanf("%d", coordinate_array + i);
    }

    qsort(coordinate_array, number_of_receivers, sizeof(int), Compare);

    int min_len = FindMinLenForLumosSpell(number_of_receivers, max_number_of_spells, coordinate_array);

    printf("%d", min_len);

    return 0;
}

int Compare(const void* x1, const void* x2)
{
    return (*(int*) x1 - *(int*) x2);
}

int FindMinLenForLumosSpell(const int number_of_receivers, const int max_number_of_spells, int* const coordinate_array)
{
    assert((number_of_receivers  > 0) && "Error! number_of_receivers  is not greater than 0!\n");
    assert((max_number_of_spells > 0) && "Error! max_number_of_spells is not greater than 0!\n");
    assert((coordinate_array != NULL) && "Error! coordinate_array     is NULL!\n");

    int left  = 0;
    int right = MAX_DISTANCE;

    while (right - left > 1)
    {
        int mid = left + (right - left) / 2;

        if (CoverAllReceivers(number_of_receivers, max_number_of_spells, mid, coordinate_array) == YES)
        {
            right = mid;
        }
        else
        {
            left = mid;
        }
    }

    if (left > right)
    {
        int temp = left;
        left = right;
        right = temp;
    }

    if (CoverAllReceivers(number_of_receivers, max_number_of_spells, left, coordinate_array) == YES)
    {
        return left;
    }

    return right;
}

int CoverAllReceivers(const int number_of_receivers, const int max_number_of_spells, const int len, int* const coordinate_array)
{
    int count = 1;
    int last = coordinate_array[0];

    for (int i = 1; i < number_of_receivers; i++)
    {
        if (coordinate_array[i] - last > len)
        {
            count++;
            last = coordinate_array[i];
        }
    }

    return (count > max_number_of_spells) ? NO : YES;

}