#include <algorithm>
#include <iostream>

const unsigned int MODULE = static_cast<unsigned int>(1e9 + 7);

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

int main() {
  ModuleInt<MODULE> a, b, c, d;

  std::cin >> a >> b >> c >> d;

  ModuleInt<MODULE> divisor =
      CalculateModularMultiplicativeInverseNumber(b * d);

  std::cout << (a * d + b * c) * divisor;

  return 0;
}