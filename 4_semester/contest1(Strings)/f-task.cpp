#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> FindEvenPalindromes(const std::vector<int>& src) {
  int n = src.size();
  std::vector<int> d(src.size(), 0);

  int left = -1;
  int right = -1;

  for (int i = 0; i < n - 1; ++i) {
    if (i < right) {
      d[i] = std::min(right - i, d[left + right - i - 1]);
    }
    while (i >= d[i] && i + d[i] < n - 1 && src[i - d[i]] == src[i + d[i] + 1]) {
      d[i]++;
    }
    if (i + d[i] > right) {
      left = i - d[i] + 1;
      right = i + d[i];
    }
  }

  return d;
}

int main() {
  int N = 0, M = 0;
  std::cin >> N >> M;

  std::vector<int> cubes(N);
  for (int i = 0; i < N; ++i) {
    std::cin >> cubes[i];
  }

  std::vector<int> centers = FindEvenPalindromes(cubes);;

  for (int i = N - 1; i >= 0; --i) {
    if (centers[i] == i + 1) {
      std::cout << N - i - 1 << ' ';
    }
  }
  std::cout << N << "\n";

  return 0;
}