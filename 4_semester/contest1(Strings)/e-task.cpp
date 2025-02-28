#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> BuildZFunction(const std::string& s1, const std::string& s2) {
  std::string buffer = s1 + "#" + s2;
  std::vector<int> z_func(buffer.length(), 0);
  int left = 0;
  int right = 0;
  int len = buffer.length();

  for (int i = 1; i < len; ++i) {
    z_func[i] = std::max(0, std::min(right - i, z_func[i - left]));
    while ((i + z_func[i] < len) &&
           (buffer[z_func[i]] == buffer[i + z_func[i]])) {
      ++z_func[i];
    }
    if (i + z_func[i] > right) {
      left = i;
      right = i + z_func[i];
    }
  }
  return z_func;
}

std::vector<int>
IsPatternBuildedByPrefixesOfText(const std::string& pattern, const std::string& text) {
  std::vector<int> z_func = BuildZFunction(text, pattern);
  std::vector<int> dp(pattern.length()+1, -1);
  dp[pattern.length()] = 1;

  int len = pattern.length()-1;
  int k = pattern.length();
  for (int dp_index = len, z_index = z_func.size() - 1; dp_index >= 0; --dp_index, --z_index) {
    if ((z_func[z_index] != 0) && (dp_index + z_func[z_index] >= k)) {
      dp[dp_index] = k;
      k = dp_index;
    }
  }

  int index = 0;
  std::vector<int> result = {};
  while (index < pattern.length()) {
    if (dp[index] == -1) {
      result.clear();
      break;
    }
    result.push_back(index);
    index = dp[index];
  }

  return result;
}

int main() { 
  std::string fiona_word;
  std::string donkey_word;

  std::cin >> fiona_word >> donkey_word;

  std::vector<int> indices = IsPatternBuildedByPrefixesOfText(donkey_word, fiona_word);

  if (indices.empty()) {
    std::cout << "Yes\n";
  } else {
    std::cout << "No\n";

    std::size_t cur_index = 0;
    std::string output;
    for (std::size_t i = 1; i < indices.size(); ++i) {
      while (cur_index < indices[i]) {
        output += donkey_word[cur_index++];
      }
      output += ' ';
    }

    while (cur_index < donkey_word.length()) {
      output += donkey_word[cur_index++];
    }
    output += '\n';
    
    std::cout << output;
  }

  return 0; 
}