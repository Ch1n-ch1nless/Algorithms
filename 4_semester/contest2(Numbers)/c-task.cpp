#include <algorithm>
#include <iostream>
#include <vector>

int GreatCommonDivisor(int a, int b) {
  int temp_var = 0;

  while (a != 0 && b != 0) {
    temp_var = a;
    a = b % a;
    b = temp_var;
  }

  return (a == 0) ? b : a;
}

int FindOperationsNumberToChangeOnGCD(const std::vector<int>& array,
                                              const std::size_t& elem_number) {
  int common_gcd = array[0];
  for (std::size_t i = 1; i < elem_number; ++i) {
    common_gcd = GreatCommonDivisor(common_gcd, array[i]);
  }

  if (common_gcd != 1) {
    return -1;
  }

  int operation_count = 0;
  bool one_flag = false;

  for (std::size_t i = 0; i < elem_number; ++i) {
    if (array[i] != 1) {
      ++operation_count;
    } else {
      one_flag = true;
    }
  }

  if (one_flag) {
    return operation_count;
  }

  int min_subarray_len = elem_number;

  for (std::size_t begin = 0; begin < elem_number - 1; ++begin) {
    int subarray_gcd = array[begin];
    int counter = 0;

    for (std::size_t end = begin + 1; end < elem_number && subarray_gcd != 1; ++end) {
      subarray_gcd = GreatCommonDivisor(subarray_gcd, array[end]);
      ++counter;
    }

    if (subarray_gcd == 1) {
      min_subarray_len = std::min(min_subarray_len, counter + 1);
    }
  }

  return elem_number + min_subarray_len - 2;
}

int main() {
  std::size_t elem_number = 0;
  std::cin >> elem_number;

  std::vector<int> array(elem_number, 0);

  for (std::size_t i = 0; i < elem_number; ++i) {
    std::cin >> array[i];
  }

  int counter = FindOperationsNumberToChangeOnGCD(array, elem_number);

  std::cout << counter << '\n';
  return 0;
}