#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

const long long MODULE = 1000000007;

struct Segment
{
    long long x_begin;
    long long x_end;
    long long y;
};

class Matrix
{
public:
    std::vector<std::vector<long long>> data;

     Matrix(long long n) :
        std::vector<std::vector<long long>> data(n, std::vector<long long>(n, 0))
    {
    }

    ~Matrix() = default;

    friend Matrix operator*(const Matrix& left, const Matrix& right);
}

int main()
{
    long long n = 0;
    long long k = 0;

    std::cin >> n >> k; 

    std::vector<Segment> segments_array(n);

    for (long long i = 0; i < n; ++i)
    {
        std::cin >> segments_array[i].x_begin >> segments_array[i].x_end >> segments_array[i].y;
    }



    return 0;
}