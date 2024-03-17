#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===Structs_&_Constants===*/

const size_t MAX_PREF_SIZE = 1 << 8;

/*===Function_Declaration===*/

void LeastSignificantDigitSort(unsigned long long* const src_array, unsigned long long* const res_array, const size_t array_size);

/*===Function_Definition===*/

int main()
{
    size_t array_size = 0;

    if (scanf("%ld", &array_size) != 1)
    {
        assert(0 && "Program can not read the number!\n");
    }

    unsigned long long* src_array = (unsigned long long*) calloc(array_size, sizeof(unsigned long long*));
    assert((src_array != NULL) && "Program can not allocate memory for array!\n");

    unsigned long long* res_array = (unsigned long long*) calloc(array_size, sizeof(unsigned long long*));
    assert((res_array != NULL) && "Program can not allocate memory for array!\n");

    for (size_t i = 0; i < array_size; i++)
    {
        if (scanf("%lld", src_array + i) != 1)
        {
            assert(0 && "Program can not read the number!\n");
        }

        res_array[i] = src_array[i];
    }

    LeastSignificantDigitSort(src_array, res_array, array_size);

    for (size_t i = 0; i < array_size; i++)
    {
        printf("%lld\n", src_array[i]);
    }

    return 0;
}

void LeastSignificantDigitSort(unsigned long long* const src_array, unsigned long long* const res_array, const size_t array_size)
{
    assert((src_array != NULL) && "Pointer to \'src_array\' is NULL!!!\n");
    assert((res_array != NULL) && "Pointer to \'res_array\' is NULL!!!\n");

    unsigned long long mask  = 0xff;    // bit mask to take 1 byte of number
    unsigned int shift = 0;             // shift of bytes

    while (mask > 0)
    {
        unsigned int pref_byte_cnt[MAX_PREF_SIZE] = {};
        for (size_t i = 0; i < MAX_PREF_SIZE; i++)
        {
            pref_byte_cnt[i] = 0;
        }
        
        for (size_t i = 0; i < array_size; i++)
        {
            pref_byte_cnt[(src_array[i] & mask) >> shift]++;
        }

        for (size_t i = 1; i < MAX_PREF_SIZE; i++)
        {
            pref_byte_cnt[i] += pref_byte_cnt[i-1];
        }


        for (size_t i = 1; i <= array_size; i++)
        {
            res_array[--pref_byte_cnt[(src_array[array_size - i] & mask) >> shift]] = src_array[array_size - i];
        }

        for (size_t i = 0; i < array_size; i++)
        {
            src_array[i] = res_array[i];
        }

        mask <<= 8;
        shift += 8;
    }
}