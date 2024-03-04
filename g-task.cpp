#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*===Structs_&_Constants===*/

typedef struct Fragment
{
    int begin;
    int end;
} Fragment;

/**===Function_Declaration===*/

int Compare(const void* const elem1, const void* const elem2);
int CalculateQuantityIndependentFragments(Fragment* const src_array, Fragment* const dst_array, const int number);

/*===Function_Definition===*/

int main()
{
    int number_of_fragments = 0;

    //Read the N
    scanf("%d", &number_of_fragments);

    Fragment* frag_array     = (Fragment*) calloc(number_of_fragments+1, sizeof(Fragment));
    Fragment* new_frag_array = (Fragment*) calloc(number_of_fragments, sizeof(Fragment));

    frag_array[number_of_fragments].begin = 2 * 1e9;
    frag_array[number_of_fragments].end   = 2 * 1e9;

    //Read fragments
    for (int i = 0; i < number_of_fragments; i++)
    {
        scanf("%d %d",  &(frag_array[i].begin), &(frag_array[i].end));
    }

    //Sort fragment array
    qsort(frag_array, number_of_fragments, sizeof(Fragment), Compare);

    //Calculate 'K'
    int number_of_independent_fragments = CalculateQuantityIndependentFragments(frag_array, new_frag_array, number_of_fragments);

    printf("%d\n", number_of_independent_fragments);

    for (int i = 0; i < number_of_independent_fragments; i++)
    {
        printf("%d %d\n", new_frag_array[i].begin, new_frag_array[i].end);
    }

    return 0;
}

int Compare(const void* const elem1, const void* const elem2)
{
    return ((Fragment*)elem1)->begin - ((Fragment*) elem2)->begin;
}

int CalculateQuantityIndependentFragments(Fragment* const src_array, Fragment* const dst_array, const int number)
{
    int count = 0;

    Fragment temp_frag = {};

    temp_frag.begin = src_array[0].begin;
    temp_frag.end   = src_array[0].end;

    for (int n_index = 1; n_index <= number; n_index++)
    {
        if (src_array[n_index].begin <= temp_frag.end)
        {
            temp_frag.end = (temp_frag.end > src_array[n_index].end) ? temp_frag.end : src_array[n_index].end;
        }
        else
        {
            dst_array[count].begin = temp_frag.begin;
            dst_array[count].end   = temp_frag.end;

            temp_frag.begin = src_array[n_index].begin;
            temp_frag.end   = src_array[n_index].end;

            count++;
        }
    }

    return count;
}