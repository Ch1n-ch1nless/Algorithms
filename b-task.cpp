#include <algorithm>
#include <iostream>
#include <vector>

long long calculateDivends(std::vector<bool>& is_prime, const long long& begin_index, const long long& end_index) {
  long long index = begin_index * 2;
  long long counter = 0;

  while (index <= end_index) {
    if (is_prime[index]) {
      counter += begin_index;
      is_prime[index] = false;
    }
    index += begin_index;
  }

  return counter;
}

long long CalculateSumOfPrimes(const long long& number) {
  std::vector<bool> is_prime(number + 1, true);

  long long sum = 0;

  for (long long index = 2; index * index <= number; index++) {
    if (is_prime[index]) {
      sum += calculateDivends(is_prime, index, number);
    }
  }

  return sum;
}

int main() {
  long long N = 0;
  std::cin >> N;

  long long answer = CalculateSumOfPrimes(N);

  std::cout << answer << '\n';
  
  return 0;
}