#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

const unsigned long long MODULE       = 1000003;
const unsigned long long MAX_JUMP_LEN = 5;

void CalculateFirstStepsOfGrassHopper(std::vector<unsigned long long> &grasshopper_steps)
{
  if (grasshopper_steps.size() == 0) return;

  grasshopper_steps[0] = 1;

  for (std::size_t i = 1; i < grasshopper_steps.size(); ++i) {
    grasshopper_steps[i] = 0;
    for (std::size_t j = 0; j < i; ++j) {
      grasshopper_steps[i] += grasshopper_steps[j];
    }
  }
}

class Matrix {
 private:
  std::size_t                                  n;
  std::vector<std::vector<unsigned long long>> data;

 public:
  Matrix(std::size_t n) : n(n), data(n, std::vector<unsigned long long>(n, 0)) {}

  ~Matrix() = default;

  std::size_t Size() { return n; }

  void CreateMoveMatrix();

  friend Matrix operator*(const Matrix& left, const Matrix& right);

  friend std::vector<unsigned long long> operator*(const Matrix& matrix, const std::vector<unsigned long long>& vector);
};

void Matrix::CreateMoveMatrix() {
  for (std::size_t i = 0; i < n - 1; ++i) {
    data[i][i+1] = 1;
  }

  for (std::size_t i = 0; i < n; ++i) {
    data[n-1][i] = 1;
  }
}

Matrix operator*(const Matrix& left, const Matrix& right) {
  Matrix result(left.n);

  for (int left_index = 0; left_index < result.n; ++left_index) {
    for (int right_index = 0; right_index < result.n; ++right_index) {
      for (int i = 0; i < result.n; ++i) {
        result.data[left_index][right_index] += (left.data[left_index][i] * right.data[i][right_index]) % MODULE;
        result.data[left_index][right_index] %= MODULE;
      }
    }
  }

  return result;
}

std::vector<unsigned long long> operator*(const Matrix& matrix, const std::vector<unsigned long long>& vector) {
  std::vector<unsigned long long> result(vector.size());

  for (int i = 0; i < 5; ++i) {
    result[i] = 0;

    for (int j = 0; j < 5; ++j) {
      result[i] += (vector[j] * matrix.data[i][j]) % MODULE;
      result[i] %= MODULE;
    }
  }

  return result;
}

Matrix BinPow(const Matrix& src, unsigned long long pow) {
  if (pow == 1) return src;

  Matrix result = BinPow(src, pow / 2);
  result = result * result;

  if (pow % 2 == 1) {
    result = result * src;
  }

  return result;
}

unsigned long long CalculateNumberOfSteps(std::vector<unsigned long long>& grasshopper_moves, unsigned long long& number_of_moves) {
  unsigned long long max_jump_len = grasshopper_moves.size();
  
  CalculateFirstStepsOfGrassHopper(grasshopper_moves);

  if (number_of_moves <= max_jump_len) {
    return grasshopper_moves[--number_of_moves];
  } else {
    Matrix move_matrix(max_jump_len);
    move_matrix.CreateMoveMatrix();

    move_matrix = BinPow(move_matrix, number_of_moves - max_jump_len);

    grasshopper_moves = move_matrix * grasshopper_moves;

    return grasshopper_moves[max_jump_len-1];
  }
}

int main() {
  unsigned long long number_of_moves = 0;

  std::cin >> number_of_moves;

  std::vector<unsigned long long> grasshopper_moves(MAX_JUMP_LEN);
  
  unsigned long long answer = CalculateNumberOfSteps(grasshopper_moves, number_of_moves);

  std::cout << answer << '\n';

  return 0;
}
