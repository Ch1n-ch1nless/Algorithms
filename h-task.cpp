#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

class PalindromeMultiplier {
 public:
  using ftype = std::complex<double>;

 private:
  static constexpr ftype ZERO = ftype{0., 0.};
  static constexpr double TAU = 2. * M_PI;      // value of 2pi

 public:
  PalindromeMultiplier(const std::vector<double>& a,
                       const std::vector<double>& b) {
    size_ = findCeilPowerOfTwo(std::max(a.size(), b.size()));
    max_pow_ = a.size() + b.size() - 1;

    first_.resize(size_, ZERO);
    second_.resize(size_, ZERO);

    copyDoubleCoefficients(first_, a);
    copyDoubleCoefficients(second_, b);
  }

  std::vector<double> multiply() {
    FastFourierTransform<false>(first_);
    FastFourierTransform<false>(second_);

    for (std::size_t i = 0; i < size_; ++i) {
      first_[i] *= second_[i];
    }

    FastFourierTransform<true>(first_);

    return transformToDoubleCoefficients(first_);
  }

 private:
  std::size_t findCeilPowerOfTwo(std::size_t number) {
    std::size_t pow = 1;
    while (pow < number) {
      pow <<= 1;
    }
    return pow << 1;
  }

  void copyDoubleCoefficients(std::vector<ftype>& dest, const std::vector<double>& src) {
    std::size_t src_size = src.size();
    for (std::size_t i = 0; i < src_size; ++i) {
      dest[i] = {src[i], 0.0};
    }
  }

  std::vector<double> transformToDoubleCoefficients(const std::vector<ftype>& complex_coefficients) {
    std::vector<double> double_coefficients(max_pow_);
    
    for (std::size_t i = 0; i < max_pow_; ++i) {
      double_coefficients[i] = complex_coefficients[i].real();
    }

    return double_coefficients;
  }

  template <bool IsInverse>
  void FastFourierTransform(std::vector<ftype>& poly) {
    constexpr double sign = (IsInverse) ? -1. : 1.;
    
    std::size_t size = poly.size();
    if (size == 1) {
      return;
    }

    std::vector<ftype> even_poly(size / 2, ZERO);
    std::vector<ftype> odd_poly(size / 2, ZERO);

    for (std::size_t i = 0; 2 * i < size; ++i) {
      even_poly[i] = poly[2 * i];
      odd_poly[i] = poly[2 * i + 1];
    }

    FastFourierTransform<IsInverse>(even_poly);
    FastFourierTransform<IsInverse>(odd_poly);

    double angle = TAU / (size + .0) * sign;
    ftype w(1.), wn(cos(angle), sin(angle));

    for (std::size_t i = 0; i < size / 2; ++i) {
      poly[i] = even_poly[i] + w * odd_poly[i];
      poly[i + size / 2] = even_poly[i] - w * odd_poly[i];  
      if constexpr (IsInverse) {
        poly[i] /= 2.;
        poly[i + size / 2] /= 2.;
      }
      w *= wn;
    }
  }

  std::vector<ftype> first_;
  std::vector<ftype> second_;
  std::size_t size_;
  std::size_t max_pow_;
};

std::vector<double> ReadPalindromeCoefficients() {
  std::size_t max_pow = 0;
  std::cin >> max_pow;
  ++max_pow;

  std::vector<double> cf(max_pow);
  for (std::size_t i = 0; i < max_pow; ++i) {
    std::cin >> cf[i];
  }

  return cf;
}

void PrintPalindromeCoefficients(const std::vector<double>& poly) {
  std::cout << poly.size() - 1 << ' ';
  for (auto& coefficient : poly) {
    std::cout << static_cast<int>(std::round(coefficient)) << ' ';
  }  
  std::cout << '\n';
}

int main() {
  auto first_poly = ReadPalindromeCoefficients();
  auto second_poly = ReadPalindromeCoefficients();

  PalindromeMultiplier solver(first_poly, second_poly);

  auto answer = solver.multiply();

  PrintPalindromeCoefficients(answer);
  
  return 0;
}