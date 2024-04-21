#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*==================Constants=================*/

const int       TRUE            = 1;
const int       FALSE           = 0;

const int       PRIME_NUMBER    = 64;
const int       MODULE          = 997;

/*===================Structs==================*/



/*==================Functions=================*/

inline int  MultiplyModulo(int number1, int number2);
inline int  SumModulo(int number1, int number2);
inline int  SubModulo(int number1, int number2);

inline int  BinPowModulo(int number, int degree);

int*        HashString(char string[50001], size_t len);

/*============================================*/

int main()
{
    char P[50001] = {};
    char S[50001] = {};

    //Read string P
    if (scanf("%s", P) == 0)
    {
        assert(FALSE && "Program can not read the string");
    }

    //Read the string S
    if (scanf("%s", S) == 0)
    {
        assert(FALSE && "Program can not read the string");
    }

    //printf("MODULE is %d\n", MODULE);

    size_t len_of_P = strlen(P);
    size_t len_of_S = strlen(S);

    printf("Hash of P:\n");
    int* hash_array_P   = HashString(P, len_of_P);
    printf("Hash of S:\n");
    int* hash_array_S   = HashString(S, len_of_S);

    int hashS   = hash_array_S[len_of_S];

    int index_begin = 0;
    int index_end   = len_of_S;

    int cur_substring_hash  = 0;
    int exp_prime_number    = BinPowModulo(PRIME_NUMBER, len_of_S);

    printf("P^3 := %d\n", exp_prime_number);

    printf("HASH:\n");
    while (index_end <= len_of_P)
    {
        cur_substring_hash = hash_array_P[index_end];
        cur_substring_hash = SubModulo(cur_substring_hash, MultiplyModulo(hash_array_P[index_begin], exp_prime_number));

        printf("Hash of [%d:%d] := %d\n", index_begin, index_end, cur_substring_hash);

        if (cur_substring_hash == hashS)
        {
            if (strncmp(&(P[index_begin]), S, len_of_S) == 0)
                printf("%d\n", index_begin);
        }

        index_end++;
        index_begin++;
    }
    

    return 0;
}

int* HashString(char string[50001], size_t len)
{
    int* array_with_hashes = (int*) calloc(len+1, sizeof(int));
    assert((array_with_hashes != nullptr) && "ERROR! Program can not allocate the memory!\n");

    array_with_hashes[0] = 0;

    for (int i = 1; i <= len; i++)
    {
        array_with_hashes[i] = SumModulo(MultiplyModulo(array_with_hashes[i-1], PRIME_NUMBER), string[i-1]);
        printf("[%d]: %d\n", i-1, array_with_hashes[i]);
    }

    return array_with_hashes;
}

inline int  MultiplyModulo(int number1, int number2)
{
    long long result = number1 * number2;
    result %= MODULE;
    result = (result + MODULE) % MODULE;

    printf("\tMUL:");
    printf("\tnumber1 := %d\n", number1);
    printf("\tnumber2 := %d\n", number2);
    printf("\tresult  := %lld\n", result);

    return (int)result;
}

inline int SumModulo(int number1, int number2)
{
    long long result = number1 + number2;
    result = result % MODULE;

    printf("\tSUM:");
    printf("\tnumber1 := %d\n", number1);
    printf("\tnumber2 := %d\n", number2);
    printf("\tresult  := %lld\n", result);

    return (int)result;
}

inline int SubModulo(int number1, int number2)
{
    long long result = number1 - number2;
    result = result % MODULE;
    result = (result + MODULE) % MODULE;

    printf("\tSUB:");
    printf("\tnumber1 := %d\n", number1);
    printf("\tnumber2 := %d\n", number2);
    printf("\tresult  := %lld\n", result);

    return (int)result;
}

int BinPowModulo(int number, int degree)
{
    if (degree == 0)
    {
        return 1;
    }
    else if (degree % 2 == 0)
    {
        return MultiplyModulo(BinPowModulo(number, degree / 2), BinPowModulo(number, degree / 2));
    }

    return MultiplyModulo(number, BinPowModulo(number, degree-1));
}