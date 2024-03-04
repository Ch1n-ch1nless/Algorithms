#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/*===Structs_&_Constants===*/

/*===Function_Declaration===*/

/*===Function_Definition===*/

int main()
{
    int number_of_candidates = 0;   // := N в условии задачи
    int quantity_of_requests = 0;   // := Q в условии задачи

    scanf("%d", &number_of_candidates);

    int speed_array[number_of_candidates]   = {};
    int pref_array[number_of_candidates]    = {};
    int suff_array[number_of_candidates]    = {};

    for (int i = 0; i < number_of_candidates; i++)
    {
        scanf("%d", &(speed_array[i]));
    }

    int min_speed = 2 * 1e9;

    //Fill prefix array
    for (int i = 0; i < number_of_candidates; i++)
    {
        if (speed_array[i] < min_speed)
        {
            pref_array[i]   = speed_array[i];
            min_speed       = speed_array[i];
        }
        else
        {
            pref_array[i]   = min_speed;
        }
    }

    min_speed = 2 * 1e9;

    //Fill suffix array
    for (int i = number_of_candidates - 1; i > -1; i--)
    {
        if (speed_array[i] < min_speed)
        {
            suff_array[i]   = speed_array[i];
            min_speed       = speed_array[i];
        }
        else
        {
            suff_array[i]   = min_speed;
        }
    }

    scanf("%d", &quantity_of_requests);

    for (int i = 0; i < quantity_of_requests; i++)
    {
        int left   = 0;
        int right  = 0;
        int answer = 0;

        scanf("%d %d", &left, &right);

        left--;
        right--;

        answer = (pref_array[left] < suff_array[right]) ? pref_array[left] : suff_array[right];
        printf("%d\n", answer);
    }

    return 0;
}