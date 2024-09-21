#include <cassert>
#include <cstdio>

unsigned long long**    CreateArray             (unsigned int n);
void                    DestroyArray            (unsigned long long** array, unsigned int n);
void                    ClearArray              (unsigned long long** array, unsigned int n);
void                    FindPeacefulSets        (unsigned long long** array, unsigned int n);
unsigned long long      GetNumberOfPeacefulSets (unsigned long long** array, unsigned int power_of_set);      

int main()
{
    unsigned int n = 0;

    if (!scanf("%u", &n))
    {
        fprintf(stderr, "ERROR!!! Program can not read the number!\n");
        return -1;
    }


    unsigned long long** dp = CreateArray(n);

    ClearArray(dp, n);

    FindPeacefulSets(dp, n);

    unsigned long long answer = GetNumberOfPeacefulSets(dp, n);

    printf("%llu\n", answer);

    DestroyArray(dp, n);

    return 0;
}

unsigned long long** CreateArray(unsigned int n)
{
    unsigned long long** array_ptr = new unsigned long long*[n+1];

    for (unsigned int i = 0; i <= n; ++i)
    {
        array_ptr[i] = new unsigned long long[n+1];
    }

    return array_ptr;
}

void DestroyArray(unsigned long long** array, unsigned int n)
{
    for (unsigned int i = 0; i <= n; ++i)
    {
        delete[] array[i];
    }

    delete[] array;
}

void ClearArray(unsigned long long** array, unsigned int n)
{
    for (unsigned int i = 0; i <= n; ++i)
    {
        for (unsigned int j = 0; j <= n; ++j)
        {
            array[i][j] = 0;
        }
    }
}

void FindPeacefulSets(unsigned long long** array, unsigned int n)
{
    array[0][0] = 1;

    for (unsigned int i = 1; i <= n; ++i) 
    {
        for (unsigned int j = 1; j <= i; ++j) 
        {
            unsigned long long res = 0;

            for (unsigned int k = 0; k <= j / 2; ++k) 
            {
                res += array[i - j][k];
            }

            array[i][j] = res;
        }
    }
}

unsigned long long GetNumberOfPeacefulSets(unsigned long long** array, unsigned int power_of_set)
{
    unsigned long long answer = 0;

    for (unsigned int i = 0; i <= power_of_set; ++i) 
    {
        answer += array[power_of_set][i];
    }

    return answer;
}