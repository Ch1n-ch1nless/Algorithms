#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

const unsigned int MODULE = 1000003;
const unsigned int MAX_JUMP_LEN = 5;

/*===========================< ModuleInt interface >==========================*/

template <unsigned int Module>
class ModuleInt {
 public:
  ModuleInt();
  ModuleInt(unsigned int number);
  ModuleInt(const ModuleInt& other);

  ~ModuleInt() = default;

  ModuleInt& operator=(const ModuleInt& other);
  ModuleInt& operator=(unsigned int number);

  ModuleInt& operator+=(const ModuleInt& other);
  ModuleInt& operator-=(const ModuleInt& other);
  ModuleInt& operator*=(const ModuleInt& other);

  unsigned int getNumber() const;
  unsigned int getModule() const;

 private:
  unsigned int number_;
};

template <unsigned int Module>
ModuleInt<Module> operator+(const ModuleInt<Module>& left,
                            const ModuleInt<Module>& right);
template <unsigned int Module>
ModuleInt<Module> operator-(const ModuleInt<Module>& left,
                            const ModuleInt<Module>& right);
template <unsigned int Module>
ModuleInt<Module> operator*(const ModuleInt<Module>& left,
                            const ModuleInt<Module>& right);

/*========================< ModuleInt implementation >========================*/

template <unsigned int Module>
ModuleInt<Module>::ModuleInt() : number_(0) {}

template <unsigned int Module>
ModuleInt<Module>::ModuleInt(unsigned int number) : number_(number % Module) {}

template <unsigned int Module>
ModuleInt<Module>::ModuleInt(const ModuleInt& other) : number_(other.number_) {}

template <unsigned int Module>
ModuleInt<Module>& ModuleInt<Module>::operator=(
    const ModuleInt<Module>& other) {
  if (this != &other) {
    number_ = other.number_;
  }

  return *this;
}

template <unsigned int Module>
ModuleInt<Module>& ModuleInt<Module>::operator=(unsigned int number) {
  number_ = number % Module;
  return *this;
}

template <unsigned int Module>
unsigned int ModuleInt<Module>::getNumber() const {
  return number_;
}

template <unsigned int Module>
unsigned int ModuleInt<Module>::getModule() const {
  return Module;
}

template <unsigned int Module>
ModuleInt<Module>& ModuleInt<Module>::operator+=(
    const ModuleInt<Module>& other) {
  // Небольшая ремарка к действию тут:
  // Так как number_ + other.number_ может быть > 2^32, то есть произойдет
  // переполнение, то может вернуться некорректная сумма. Переполнение
  // происходит в случае, когда (number_ + other.number_) > module_, но тут
  // опять же надо корректно проверить это тогда сделаем следующее: number >
  // module_ - other.number_ - это выражение математически корректно! А ещё так
  // как other.number_ < module, то  module_ - other.number_ не переполниться!
  // ну и далее просто чекаем на истинность наше неравенство, если не истинно,
  // то number_ + other.number_ === number + other.number_ - module_(mod
  // module_) number_ + other.number_ === number - (module_ - other.number_)
  // (mod module_)

  unsigned int tmp = Module - other.number_;

  number_ = (number_ < tmp) ? number_ + other.number_ : number_ - tmp;

  return *this;
}

template <unsigned int Module>
ModuleInt<Module>& ModuleInt<Module>::operator-=(
    const ModuleInt<Module>& other) {
  // Заметим, что a - b === a + (module - b) (mod module)
  // Далее пишем как в операторе +=

  unsigned int tmp = Module - other.number_;

  number_ = (number_ < other.number_) ? number_ + tmp : number_ - other.number_;

  return *this;
}

template <unsigned int Module>
ModuleInt<Module>& ModuleInt<Module>::operator*=(
    const ModuleInt<Module>& other) {
  // Была идея написать тут цикл, который несколько раз умножает число, но решил
  // пойти через касты, если что могу переписать)

  unsigned long long result =
      ((static_cast<unsigned long long>(number_)) *
       (static_cast<unsigned long long>(other.number_))) %
      (static_cast<unsigned long long>(Module));

  number_ = static_cast<unsigned int>(result);

  return *this;
}

template <unsigned int Module>
ModuleInt<Module> operator+(const ModuleInt<Module>& left,
                            const ModuleInt<Module>& right) {
  ModuleInt<Module> result = left;
  result += right;
  return result;
}

template <unsigned int Module>
ModuleInt<Module> operator-(const ModuleInt<Module>& left,
                            const ModuleInt<Module>& right) {
  ModuleInt<Module> result = left;
  result -= right;
  return result;
}

template <unsigned int Module>
ModuleInt<Module> operator*(const ModuleInt<Module>& left,
                            const ModuleInt<Module>& right) {
  ModuleInt<Module> result = left;
  result *= right;
  return result;
}

/*============================================================================*/

/*============================< Matrix interface >============================*/

template <typename T>
class Matrix {
 public:
  Matrix(std::size_t size);
  Matrix(const std::vector<std::vector<T>>& new_data);
  Matrix(const Matrix& other);

  ~Matrix() = default;

  Matrix& operator=(const Matrix& other);

  std::vector<T>& operator[](std::size_t index);
  const std::vector<T>& operator[](std::size_t index) const;

  Matrix& operator+=(const Matrix& other);
  Matrix& operator-=(const Matrix& other);
  Matrix& operator*=(const Matrix& other);

  std::size_t size() const;

 private:
  std::vector<std::vector<T>> data_;
  std::size_t size_;

  template <typename U>
  friend std::vector<U> operator*(const Matrix<U>& matrix,
                                  const std::vector<U>& vec);
};

template <typename T>
Matrix<T> operator+(const Matrix<T>& left, const Matrix<T>& right);

template <typename T>
Matrix<T> operator-(const Matrix<T>& left, const Matrix<T>& right);

template <typename T>
Matrix<T> operator*(const Matrix<T>& left, const Matrix<T>& right);

template <typename T>
std::vector<T> operator*(const Matrix<T>& matrix, const std::vector<T>& vec);

/*===========================< Matrix implementation >========================*/

template <typename T>
std::size_t Matrix<T>::size() const {
  return size_;
}

template <typename T>
Matrix<T>::Matrix(std::size_t size)
    : data_(size, std::vector<T>(size)), size_(size) {}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other)
    : data_(other.data_), size_(other.size_) {}

template <typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& new_data) {
  size_ = new_data.size;

  data_.resize(size_);

  for (std::size_t i = 0; i < size_; i++) {
    assert(size_ == new_data[i].size());
    data_.resize(size_);

    for (std::size_t j = 0; j < size_; j++) {
      data_[i][j] = new_data[i][j];
    }
  }
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
  if (this != &other) {
    data_ = other.data_;
    size_ = other.size_;
  }

  return *this;
}

template <typename T>
std::vector<T>& Matrix<T>::operator[](std::size_t index) {
  assert((index < size_) && "ERROR!!! Access denied! Invalid index\n");
  return data_[index];
}

template <typename T>
const std::vector<T>& Matrix<T>::operator[](std::size_t index) const {
  assert((index < size_) && "ERROR!!! Access denied! Invalid index\n");
  return data_[index];
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other) {
  assert((size_ == other.size_) && "ERROR!!! Different sizes of matrices!\n");

  for (std::size_t i = 0; i < size_; i++) {
    for (std::size_t j = 0; j < size_; j++) {
      data_[i][j] += other.data_[i][j];
    }
  }

  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& other) {
  assert((size_ == other.size_) && "ERROR!!! Different sizes of matrices!\n");

  for (std::size_t i = 0; i < size_; i++) {
    for (std::size_t j = 0; j < size_; j++) {
      data_[i][j] -= other.data_[i][j];
    }
  }

  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& other) {
  assert((size_ == other.size_) && "ERROR!!! Different sizes of matrices!\n");

  Matrix<T> result = operator*(*this, other);
  *this = result;

  return *this;
}

template <typename T>
Matrix<T> operator+(const Matrix<T>& left, const Matrix<T>& right) {
  Matrix<T> result = left;
  result += right;
  return result;
}

template <typename T>
Matrix<T> operator-(const Matrix<T>& left, const Matrix<T>& right) {
  Matrix<T> result = left;
  result -= right;
  return result;
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& left, const Matrix<T>& right) {
  assert(left.size() == right.size());

  Matrix<T> result(left.size());

  for (std::size_t i = 0; i < result.size(); i++) {
    for (std::size_t j = 0; j < result.size(); j++) {
      result[i][j] = 0;

      for (std::size_t k = 0; k < result.size(); k++) {
        result[i][j] += left[i][k] * right[k][j];
      }
    }
  }

  return result;
}

template <typename T>
std::vector<T> operator*(const Matrix<T>& matrix, const std::vector<T>& vec) {
  assert(vec.size() == matrix.size());

  std::vector<T> result(vec.size());

  for (std::size_t i = 0; i < vec.size(); i++) {
    result[i] = 0;
    for (std::size_t j = 0; j < vec.size(); j++) {
      result[i] += matrix[i][j] * vec[j];
    }
  }

  return result;
}

/*=============================================================================*/

template <unsigned int Module>
Matrix<ModuleInt<Module>> CreateIdentityMatrix(std::size_t size) {
  Matrix<ModuleInt<Module>> identity_matrix(size);

  for (std::size_t i = 0; i < size; i++) {
    for (std::size_t j = 0; j < size; j++) {
      identity_matrix[i][j] = (i == j) ? 1 : 0;
    }
  }

  return identity_matrix;
}

template <unsigned int Module>
Matrix<ModuleInt<Module>> CreateMoveMatrix(std::size_t size) {
  Matrix<ModuleInt<Module>> move_matrix(size);

  for (std::size_t i = 0; i < (size - 1); i++) {
    for (std::size_t j = 0; j < size; j++) {
      move_matrix[i][j] = (i + 1 == j) ? 1 : 0;
    }
  }

  for (std::size_t i = 0; i < size; i++) {
    move_matrix[size - 1][i] = 1;
  }

  return move_matrix;
}

template <unsigned int Module>
Matrix<ModuleInt<Module>> BinPow(Matrix<ModuleInt<Module>> src,
                                 unsigned long long pow) {
  Matrix<ModuleInt<Module>> result = CreateIdentityMatrix<Module>(src.size());

  while (pow != 0) {
    if ((pow % 2) == 1) {
      result *= src;
    }
    src *= src;
    pow /= 2;
  }

  return result;
}

template <unsigned int Module>
void CalculateFirstStepsOfGrassHopper(
    std::vector<ModuleInt<Module>>& grasshopper_steps) {
  if (grasshopper_steps.size() == 0) return;

  grasshopper_steps[0] = 1;

  for (std::size_t i = 1; i < grasshopper_steps.size(); ++i) {
    grasshopper_steps[i] = 0;
    for (std::size_t j = 0; j < i; ++j) {
      grasshopper_steps[i] += grasshopper_steps[j];
    }
  }
}

template <unsigned int Module>
ModuleInt<Module> FindNumberOfSteps(
    std::vector<ModuleInt<Module>> grasshopper_steps,
    unsigned long long number_of_steps) {
  ModuleInt<Module> ans(0);

  unsigned long long max_jump_len = grasshopper_steps.size();

  CalculateFirstStepsOfGrassHopper<Module>(grasshopper_steps);

  if (number_of_steps <= max_jump_len) {
    return grasshopper_steps[--number_of_steps];
  }

  Matrix<ModuleInt<Module>> move_matrix =
      CreateMoveMatrix<Module>(max_jump_len);

  move_matrix = BinPow(move_matrix, number_of_steps - max_jump_len);

  grasshopper_steps = move_matrix * grasshopper_steps;

  return grasshopper_steps[max_jump_len - 1];
}

int main() {
  unsigned long long number_of_moves = 0;

  std::cin >> number_of_moves;

  std::vector<ModuleInt<MODULE>> grasshopper_moves(MAX_JUMP_LEN);

  ModuleInt<MODULE> answer =
      FindNumberOfSteps(grasshopper_moves, number_of_moves);

  std::cout << answer.getNumber() << '\n';

  return 0;
}
