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
        data(n, std::vector<long long>(n, 0))
    {
    }

    ~Matrix() = default;

    friend Matrix operator*(const Matrix& left, const Matrix& right);
    friend std::vector<long long> operator*(const Matrix& matrix, const std::vector<long long>& vector);
};

Matrix operator*(const Matrix& left, const Matrix& right)
{
    std::size_t n = left.data.size();

    Matrix result(n);

    for (std::size_t i = 0; i < n; i++)
    {
        for (std::size_t j = 0; j < n; j++)
        {
            for (std::size_t k = 0; k < n; k++)
            {
                result.data[i][j] += (left.data[i][k] * right.data[k][j]) % MODULE;
                result.data[i][j] %= MODULE;
            }
        }
    }

    return result;
}

std::vector<long long> operator*(const Matrix& matrix, const std::vector<long long>& vector)
{
    std::size_t n = vector.size();

    std::vector<long long> result(n);

    for (std::size_t i = 0; i < n; i++)
    {
        for (std::size_t j = 0; j < n; j++)
        {
            result[i] += (vector[j] * matrix.data[i][j]) % MODULE;
            result[i] %= MODULE;
        }
    }

    return result;
}

void FillMoveMatrix(Matrix& move_matrix)
{
    for (std::size_t i = 0; i < move_matrix.data.size(); ++i)
    {
        for (std::size_t j = 0; j < move_matrix.data.size(); ++j)
        {
            if (abs(i-j) <= 1)
            {
                move_matrix.data[i][j] = 1;
            }
        }
    }
}

Matrix CreateEMatrix(const Matrix& matrix)
{
    Matrix new_matrix(matrix.data.size());

    for (std::size_t i = 0; i < matrix.data.size(); ++i)
    {
        for (std::size_t j = 0; j < matrix.data.size(); ++j)
        {
            new_matrix.data[i][j] = (i == j) ? 1 : 0;
        }
    } 

    return new_matrix;
}

Matrix BinPow(const Matrix& src, long long pow) {
  if (pow == 0) return CreateEMatrix(src);

  Matrix result = BinPow(src, pow / 2);
  result = result * result;

  if (pow % 2 == 1) {
    result = result * src;
  }

  return result;
}

int main()
{
    long long n = 0;
    long long k = 0;

    std::cin >> n >> k; 

    Segment new_seg = {};

    std::vector<long long> answer = {};

    for (long long i = 0; i < n; ++i)
    {
        std::cin >> new_seg.x_begin >> new_seg.x_end >> new_seg.y;

        answer.resize(new_seg.y + 1);

        if (i == 0)
        {
            answer[0] = 1;
        }

        Matrix move_matrix(new_seg.y + 1);

        FillMoveMatrix(move_matrix);

        Matrix res_matrix = BinPow(move_matrix, std::min(k, new_seg.x_end) - new_seg.x_begin);
        answer = res_matrix * answer;
    }

    std::cout << answer[0] << '\n';

    return 0;
}