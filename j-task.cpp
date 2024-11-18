#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct LargestCommonSequence {
  size_t              number_of_elements  = 0;
  std::vector<size_t> indexes1            = {};
  std::vector<size_t> indexes2            = {};
};

LargestCommonSequence RestoreAnswer(std::string& string1, std::string string2, 
                                    std::vector<std::vector<size_t>>& lcs_dp,
                                    int i_begin, int j_begin,
                                    std::size_t count)
{
  LargestCommonSequence answer = {};

  answer.number_of_elements = count;

  while (count != 0) {
    if (string1[i_begin - 1] == string2[j_begin - 1]) {
      answer.indexes1.push_back(i_begin);
      answer.indexes2.push_back(j_begin);

      i_begin--;
      j_begin--;
      count--;
    } else {
      if (lcs_dp[i_begin - 1][j_begin] < lcs_dp[i_begin][j_begin - 1]) {
        j_begin--;
      } else {
        i_begin--;
      }
    }
  }

  std::reverse(answer.indexes1.begin(), answer.indexes1.end());
  std::reverse(answer.indexes2.begin(), answer.indexes2.end());

  return answer;
}

LargestCommonSequence 
FindLargestCommonSequence(std::string& string1, std::string& string2) {
  int i_begin = 0;
  int j_begin = 0;

  std::vector<std::vector<size_t>> lcs_dp(string1.size() + 1, std::vector<size_t>(string2.size() + 1, 0));
  std::size_t max_number_of_elements = 0;

  for (std::size_t i = 1; i <= string1.size(); ++i) {
    for (std::size_t j = 1; j <= string2.size(); ++j) {
      lcs_dp[i][j] = std::max(lcs_dp[i - 1][j], lcs_dp[i][j - 1]);

      if (string1[i - 1] == string2[j - 1]) {
        lcs_dp[i][j] = lcs_dp[i - 1][j - 1] + 1;
      }

      if (lcs_dp[i][j] > max_number_of_elements) {
        max_number_of_elements = lcs_dp[i][j];
        i_begin = i;
        j_begin = j;
      }
    }
  }

  return RestoreAnswer(string1, string2, lcs_dp, i_begin, j_begin, max_number_of_elements);
}

template <typename T>
void PrintVector(const std::vector<T> &vector)
{
  for (std::size_t i = 0; i < vector.size(); i++)
  {
    std::cout << vector[i] << ' ';
  }
}

void PrintLargestCommonSequence(LargestCommonSequence& lcs)
{
  std::cout << lcs.number_of_elements << '\n';

  PrintVector(lcs.indexes1);
  std::cout << '\n';

  PrintVector(lcs.indexes2);
  std::cout << '\n';
}

int main() {
  std::string cj_dance = {};
  std::string caesar_dance = {};

  std::cin >> cj_dance;
  std::cin >> caesar_dance;

  LargestCommonSequence answer =
      FindLargestCommonSequence(cj_dance, caesar_dance);

  PrintLargestCommonSequence(answer);

  return 0;
}