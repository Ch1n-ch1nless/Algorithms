#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

std::size_t GetCommonSubstringLength(const std::string& string) {
  std::size_t len = string.length();
  std::vector<std::size_t> pref_array(len);

  for (int i = 1; i < len; i++) {
    std::size_t j = pref_array[i - 1];
    while (j > 0 && string[i] != string[j]) j = pref_array[j - 1];
    if (string[i] == string[j]) j++;
    pref_array[i] = j;
  }

  return pref_array[len - 1];
}

void ConcatenateString(std::string& buffer, const std::string& new_suffix) {
  //std::string temp_string = new_suffix + '#' + buffer;

  std::size_t len = std::min(new_suffix.length(), buffer.length());
  
  std::string temp_string;
  temp_string.append(new_suffix.begin(), new_suffix.begin() + len);
  temp_string += '#';
  temp_string.append(buffer.begin() + buffer.size() - len, buffer.end());

  std::size_t common_substring_len = GetCommonSubstringLength(temp_string);

  buffer.append(new_suffix.begin() + common_substring_len, new_suffix.end());
}

int main() {
  std::size_t string_number = 0;
  std::cin >> string_number;
  --string_number;

  std::string result = "";
  std::string new_suffix = "";
  std::cin >> result;

  result.reserve(110'001);

  std::vector<std::string> array(string_number);

  for (std::size_t i = 0; i < string_number; ++i) {
    std::cin >> array[i];
  }

  for (std::size_t i = 0; i < string_number; ++i) {
    ConcatenateString(result, array[i]);
  }

  std::cout << result;
  return 0;
}