#include <assert.h>
#include <cstdio>
#include <cmath>

const long long MODULE = 1000003;

struct Matrix
{
    long long x11, x12;
    long long x21, x22;

    Matrix(long long a11, long long a12, long long a21, long long a22) :
        x11(a11 % MODULE), x12(a12 % MODULE),
        x21(a21 % MODULE), x22(a22 % MODULE)    {}

    Matrix() : 
        x11(1), x12(1),
        x21(1), x22(0)  {}
};

Matrix operator + (Matrix a, Matrix b)
{
    return Matrix(  (a.x11 + b.x11) % MODULE, (a.x12 + b.x12) % MODULE,
                    (a.x21 + b.x21) % MODULE, (a.x22 + b.x22) % MODULE );
}

Matrix operator * (Matrix a, Matrix b)
{
    return Matrix(  ((a.x11 * b.x11) % MODULE + (a.x12 * b.x21) % MODULE) % MODULE, ((a.x11 * b.x12) % MODULE + (a.x12 * b.x22) % MODULE) % MODULE,
                    ((a.x21 * b.x11) % MODULE + (a.x22 * b.x21) % MODULE) % MODULE, ((a.x21 * b.x12) % MODULE + (a.x22 * b.x22) % MODULE) % MODULE );
}

Matrix BinPow(Matrix old_matrix, long long pow)
{
    if (pow == 0) return Matrix(1, 0, 0, 1);
    if (pow == 1) return old_matrix;

    Matrix new_matrix = BinPow(old_matrix, pow / 2);
    new_matrix = new_matrix * new_matrix;

    if (pow % 2 == 1) {
        new_matrix = old_matrix * new_matrix;
    }

    return new_matrix;
}

int main()
{
    Matrix fib_matrix;

    long long n = 0;

    if (!scanf("%lld", &n)) {
        assert(0 && "ERROR!!! Program can not read the number!\n");
    }

    fib_matrix = BinPow(fib_matrix, n-1);

    printf("%lld\n", fib_matrix.x21);

    return 0;
}