#include <cassert>
#include <cmath>
#include <cstdio>

const unsigned long long MODULE = 1000003;

class Vector {
 public:
  unsigned long long data[5];

  Vector() : data() {
    for (int i = 0; i < 5; ++i) {
      data[i] = 0;
    }
  }

  ~Vector() = default;

  void CalculateFirstSteps();
};

void Vector::CalculateFirstSteps() {
  data[0] = 1;

  for (int step = 1; step < 5; ++step) {
    for (int i = 1; i <= step; ++i) {
      data[step] += data[step - i];
    }
  }
}

class Matrix {
 private:
  unsigned long long data[5][5];

 public:
  Matrix(unsigned long long new_data[5][5]) : data() {
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        data[i][j] = new_data[i][j];
      }
    }
  }

  Matrix() : data() {
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
        data[i][j] = 0;
      }
    }
  }

  ~Matrix() = default;

  void CreateMoveMatrix();

  friend Matrix operator*(const Matrix& left, const Matrix& right);
  friend Vector operator*(const Matrix& matrix, const Vector& vector);
};

void Matrix::CreateMoveMatrix() {
  data[0][1] = 1;
  data[1][2] = 1;
  data[2][3] = 1;
  data[3][4] = 1;
  data[4][0] = 1;
  data[4][1] = 1;
  data[4][2] = 1;
  data[4][3] = 1;
  data[4][4] = 1;
}

Matrix operator*(const Matrix& left, const Matrix& right) {
  Matrix result = {};

  for (int left_index = 0; left_index < 5; ++left_index) {
    for (int right_index = 0; right_index < 5; ++right_index) {
      result.data[left_index][right_index] = 0;

      for (int i = 0; i < 5; ++i) {
        result.data[left_index][right_index] +=
            (left.data[left_index][i] * right.data[i][right_index]) % MODULE;
        result.data[left_index][right_index] %= MODULE;
      }
    }
  }

  return result;
}

Vector operator*(const Matrix& matrix, const Vector& vector) {
  Vector result = {};

  for (int i = 0; i < 5; ++i) {
    result.data[i] = 0;

    for (int j = 0; j < 5; ++j) {
      result.data[i] += (vector.data[j] * matrix.data[i][j]) % MODULE;
      result.data[i] %= MODULE;
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

int main() {
  unsigned long long number_of_moves = 0;
  unsigned long long answer = 0;

  if (!scanf("%llu", &number_of_moves)) {
    assert(false && "ERROR!!! Program can not read the number!\n");
  }

  Vector grasshopper_moves = {};
  grasshopper_moves.CalculateFirstSteps();

  if (number_of_moves <= 5) {
    answer = grasshopper_moves.data[--number_of_moves];
  } else {
    Matrix move_matrix = {};
    move_matrix.CreateMoveMatrix();

    move_matrix = BinPow(move_matrix, number_of_moves - 5);

    grasshopper_moves = move_matrix * grasshopper_moves;

    answer = grasshopper_moves.data[4];
  }

  printf("%llu\n", answer);

  return 0;
}
