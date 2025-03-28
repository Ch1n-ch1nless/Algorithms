#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

class ConcatenatedPalindromeChecker {
 public:
  using pair = std::pair<std::size_t, std::size_t>;

  ConcatenatedPalindromeChecker(const std::vector<std::string>& array);
  ~ConcatenatedPalindromeChecker() = default;
  std::vector<pair> getPalindromePairs();

 private:
  void generatePalindromes(const std::string& str, std::size_t index, std::vector<pair>& pairs); 
  void generateReversePalindromes(const std::string& str, std::size_t index, std::vector<pair>& pairs); 
  bool isPalindrome(const std::string& str, std::size_t begin, std::size_t end);
  
  std::vector<std::string> strings_;
  std::unordered_map<std::string, std::vector<std::size_t>> indices_;
};

ConcatenatedPalindromeChecker::ConcatenatedPalindromeChecker(const std::vector<std::string>& array) :
    strings_(array) {
  for (std::size_t i = 0; i < strings_.size(); ++i) {
    indices_[strings_[i]].push_back(i);
  }    
}

std::vector<std::pair<std::size_t, std::size_t>>
ConcatenatedPalindromeChecker::getPalindromePairs() {
  std::vector<pair> palindrome_pairs;

  for (std::size_t i = 0; i < strings_.size(); ++i) {
    generatePalindromes(strings_[i], i, palindrome_pairs);
    generateReversePalindromes(strings_[i], i, palindrome_pairs);
  }

  std::sort(palindrome_pairs.begin(), palindrome_pairs.end());

  return palindrome_pairs;
}

void ConcatenatedPalindromeChecker::generatePalindromes(const std::string& str, std::size_t index, std::vector<pair>& pairs) {
  std::string rev = str;
  std::reverse(rev.begin(), rev.end());
  
  if (indices_.count(rev)) {
    for (auto other_index : indices_.at(rev)) {
      if (other_index != index) {
        pairs.emplace_back(index, other_index);
      }
    }
  }
  
  for (size_t i = 1; i < str.size(); ++i) {
    if (isPalindrome(str, 0, i)) {
      std::string suffix = rev.substr(0, rev.size() - i);
      if (indices_.count(suffix)) {
        for (auto other_index : indices_.at(suffix)) {
          if (other_index != index) {
            pairs.emplace_back(other_index, index);
          }
        }
      }
    }
  }
}

void ConcatenatedPalindromeChecker::generateReversePalindromes(const std::string& str, std::size_t index, std::vector<pair>& pairs) {
  for (size_t i = 0; i < str.size(); ++i) {
    if (isPalindrome(str, i, str.size())) {
      std::string prefix = str.substr(0, i);
      std::reverse(prefix.begin(), prefix.end());
      if (indices_.count(prefix)) {
        for (auto other_index : indices_.at(prefix)) {
          if (other_index != index) {
            pairs.emplace_back(index, other_index);
          }
        }
      }
    }
  }
}

bool ConcatenatedPalindromeChecker::isPalindrome(const std::string& str, std::size_t begin, std::size_t end) {
  if (begin >= end) return true;
  end--;
  while (begin < end) {
    if (str[begin] != str[end]) {
      return false;
    }
    begin++;
    end--;
  }
  return true;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::size_t strings_number = 0;
  std::cin >> strings_number;

  std::vector<std::string> strings(strings_number);

  for (std::size_t i = 0; i < strings_number; ++i) {
    std::cin >> strings[i];
  }

  ConcatenatedPalindromeChecker checker(strings);

  std::vector<std::pair<std::size_t, std::size_t>> answer =
      checker.getPalindromePairs();

  std::cout << answer.size() << "\n";
  for (auto pair : answer) {
    std::cout << pair.first + 1 << " " << pair.second + 1 << "\n";
  }

  return 0;
}