#include <algorithm>
#include <iostream>
#include <vector>

const unsigned int MODULE = static_cast<unsigned int>(1e9 + 7);

/*===========================< ModuleInt interface >==========================*/

template <unsigned int Module>
class ModuleInt {
 public:
  ModuleInt();
  ModuleInt(unsigned int number);
  ModuleInt(int number);
  ModuleInt(const ModuleInt& other);

  ~ModuleInt() = default;

  ModuleInt& operator=(const ModuleInt& other);
  ModuleInt& operator=(unsigned int number);
  ModuleInt& operator=(int number);

  ModuleInt& operator+=(const ModuleInt& other);
  ModuleInt& operator-=(const ModuleInt& other);
  ModuleInt& operator*=(const ModuleInt& other);

  unsigned int getNumber() const;
  unsigned int getModule() const;

 private:
  unsigned int number_;

  template <unsigned int OtherModule>
  friend std::istream& operator>>(std::istream& in,
                                  ModuleInt<OtherModule>& number);
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
ModuleInt<Module>::ModuleInt(int number) {
  while (number < 0) {
    number += static_cast<int>(Module);
  }
  
  number_ = static_cast<unsigned int>(number) % Module;
}

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
ModuleInt<Module>& ModuleInt<Module>::operator=(int number) {
  while (number < 0) {
    number += static_cast<int>(Module);
  }
  
  number_ = static_cast<unsigned int>(number) % Module;
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

template <unsigned int Module>
std::istream& operator>>(std::istream& in, ModuleInt<Module>& number) {
  int read_number;
  in >> read_number;

  number.number_ = (read_number < 0)
                       ? static_cast<unsigned int>(Module + read_number)
                       : static_cast<unsigned int>(read_number);
  return in;
}

template <unsigned int Module>
std::ostream& operator<<(std::ostream& os, const ModuleInt<Module>& number) {
  return os << number.getNumber();
}

template <unsigned int Module>
ModuleInt<Module> FastExponentiation(const ModuleInt<Module>& number,
                                     const unsigned int pow) {
  if (pow == 0) {
    return 1;
  }

  ModuleInt<Module> temp_result = FastExponentiation(number, pow / 2);
  temp_result *= temp_result;

  return (pow % 2 == 1) ? temp_result * number : temp_result;
}

template <unsigned int Module>
ModuleInt<Module> CalculateModularMultiplicativeInverseNumber(
    const ModuleInt<Module>& number) {
  return FastExponentiation(number, MODULE - 2);
}

/*============================================================================*/

template <unsigned int Module>
class CombinatoricsCalculator {
 public:
  using module_t = ModuleInt<Module>;

  CombinatoricsCalculator(int max_number) {
    factorials_.resize(max_number + 1);
    inverse_factorials_.resize(max_number + 1);

    factorials_[0] = 1;
    for (int i = 1; i <= max_number; ++i) {
      factorials_[i] = factorials_[i - 1] * static_cast<module_t>(i);
    }

    inverse_factorials_[max_number] =
        CalculateModularMultiplicativeInverseNumber<Module>(
            factorials_[max_number]);
    
    for (int i = max_number - 1; i > 0; --i) {
      inverse_factorials_[i] = inverse_factorials_[i + 1] * static_cast<module_t>(i + 1);
    }

    inverse_factorials_[0] = 1;
  }

  module_t calculateCombinationNumber(int n, int k) {
    return factorials_[n] * inverse_factorials_[k] * inverse_factorials_[n - k];
  }

  module_t calculateStirlingNumber(int n, int k) {
    if (k == 0) return n == 0;

    module_t sum = 0;

    for (int i = 0; i <= k; ++i) {
      module_t coefficient = calculateCombinationNumber(k, i);
      module_t sign = static_cast<module_t>((k - i) % 2 ? -1 : 1);
      module_t term = FastExponentiation<MODULE>(static_cast<module_t>(i), static_cast<unsigned int>(n));
      sum += term * coefficient * sign;
    }

    return sum * inverse_factorials_[k];
  }

 private:
  std::vector<module_t> factorials_;
  std::vector<module_t> inverse_factorials_;
};

int main() {
  int n = 0, k = 0;
  std::cin >> n >> k;

  ModuleInt<MODULE> sum = 0;
  for (int i = 0; i < n; ++i) {
    ModuleInt<MODULE> temp_var;
    std::cin >> temp_var;
    sum += temp_var;
  }

  if ((k == 0) || k > n) {
    std::cout << 0 << '\n';
    return 0;
  }

  CombinatoricsCalculator<MODULE> calculator(n);

  auto S_nk = calculator.calculateStirlingNumber(n, k);
  auto S_n1k = calculator.calculateStirlingNumber(n - 1, k);
  auto coefficient = S_nk + static_cast<ModuleInt<MODULE>>(n - 1) * S_n1k;

  auto answer = coefficient * sum;
  std::cout << answer << '\n';

  return 0;
}