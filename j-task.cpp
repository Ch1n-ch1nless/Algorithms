#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct LargestCommonSequence {
  size_t              number_of_elements  = 0;
  std::vector<size_t> indexes1            = {};
  std::vector<size_t> indexes2            = {};
};

LargestCommonSequence 
FindLargestCommonSequence(std::string& string1, std::string& string2) {
  
  LargestCommonSequence answer = {};

  int i_begin = 0;
  int j_begin = 0;

  std::vector<std::vector<size_t>> lcs_dp(string1.size() + 1, std::vector<size_t>(string2.size() + 1, 0));

  for (int i = 1; i <= string1.size(); ++i) {
    for (int j = 1; j <= string2.size(); ++j) {
      lcs_dp[i][j] = std::max(lcs_dp[i - 1][j], lcs_dp[i][j - 1]);

      if (string1[i - 1] == string2[j - 1]) {
        lcs_dp[i][j] = lcs_dp[i - 1][j - 1] + 1;
      }

      if (lcs_dp[i][j] > answer.number_of_elements) {
        answer.number_of_elements = lcs_dp[i][j];
        i_begin = i;
        j_begin = j;
      }
    }
  }

  int count = answer.number_of_elements;

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

void PrintLargestCommonSequence(LargestCommonSequence& lcs)
{
  std::cout << lcs.number_of_elements << '\n';

  for (size_t i = 0; i < lcs.indexes1.size(); ++i)
  {
    std::cout << lcs.indexes1[i] << ' ';
  }
  std::cout << '\n';

  for (size_t i = 0; i < lcs.indexes2.size(); ++i)
  {
    std::cout << lcs.indexes2[i] << ' ';
  }
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