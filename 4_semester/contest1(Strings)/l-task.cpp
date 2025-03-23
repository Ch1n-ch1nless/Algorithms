#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int FindNumberOfEvenPalindromes(const std::string& src) {
  int len = static_cast<int>(src.size());
  std::vector<int> centers(src.size(), 0);

  int left = -1;
  int right = -1;

  for (int i = 0; i < len - 1; ++i) {
    if (i < right) {
      centers[i] = std::min(right - i, centers[left + right - i - 1]);
    }
    while (i >= centers[i] && i + centers[i] < len &&
           src[i - centers[i]] == src[i + centers[i] + 1]) {
      centers[i]++;
    }
    if (i + centers[i] > right) {
      left = i - centers[i] + 1;
      right = i + centers[i];
    }
  }

  int palindromes_number = 0;
  for (auto center : centers) {
    palindromes_number += center;
  }

  return palindromes_number;
}

int FindNumberOfOddPalindromes(const std::string& src) {
  int len = static_cast<int>(src.size());
  std::vector<int> centers(src.size(), 1);

  int left = 0;
  int right = 0;

  for (int i = 1; i < len; ++i) {
    if (i < right) {
      centers[i] = std::min(right - i + 1, centers[left + right - i]);
    }
    while (i >= centers[i] && i + centers[i] < len &&
           src[i - centers[i]] == src[i + centers[i]]) {
      centers[i]++;
    }
    if (i + centers[i] - left > right) {
      left = i - centers[i] + 1;
      right = i + centers[i] - 1;
    }
  }

  int palindromes_number = 0;
  for (auto center : centers) {
    palindromes_number += center;
  }

  return palindromes_number;
}

int main() {
  std::string s = "";
  std::cin >> s;

  int answer = FindNumberOfEvenPalindromes(s) + FindNumberOfOddPalindromes(s);
  answer -= s.size();

  std::cout << answer << "\n";
}