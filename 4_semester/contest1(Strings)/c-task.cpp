#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class StringBuilder {
 public:
  StringBuilder() = default;
  ~StringBuilder() = default;

  const std::string& buildFromPrefixFunction(
      const std::vector<std::size_t>& pref_array);

  std::vector<size_t> buildPrefixFunctionFromZFunction(
      const std::vector<std::size_t>& z_array);

 private:
  std::string builded_string;
  std::vector<bool> used;
  const std::size_t ALPHABET_LENGTH = 'z' - 'a';
  const char START_LETTER = 'a';
};

const std::string& StringBuilder::buildFromPrefixFunction(
    const std::vector<std::size_t>& pref_array) {
  builded_string.resize(pref_array.size(), '\0');
  used.resize(ALPHABET_LENGTH, false);

  std::size_t counter = 1;
  std::size_t len = pref_array.size();
  builded_string[0] = START_LETTER;

  for (std::size_t i = 1; i < len; ++i) {
    if (pref_array[i] == 0) {
      std::fill(used.begin(), used.end(), false);
      std::size_t k = pref_array[i - 1];

      while (k > 0) {
        used[builded_string[k] - START_LETTER] = true;
        k = pref_array[k - 1];
      }

      builded_string[i] = '\0';
      for (std::size_t j = 1; j <= counter; ++j) {
        if (!used[j]) {
          builded_string[i] = j + START_LETTER;
          break;
        }
      }
      if (builded_string[i] == '\0') {
        builded_string[i] = ++counter + START_LETTER;
      }

    } else {
      builded_string[i] = builded_string[pref_array[i] - 1];
    }
  }

  return builded_string;
}

std::vector<size_t> StringBuilder::buildPrefixFunctionFromZFunction(
    const std::vector<std::size_t>& z_array) {
  std::size_t len = z_array.size();
  std::vector<std::size_t> pref_array(len, 0);

  for (std::size_t i = 1; i < len; ++i) {
    for (std::size_t j = 0; j < z_array[i]; ++j) {
      if (pref_array[i + z_array[i] - j - 1] > 0) {
        break;
      } else {
        pref_array[i + z_array[i] - j - 1] = z_array[i] - j;
      }
    }
  }

  return pref_array;
}

int main() {
  std::size_t len = 0;
  std::cin >> len;

  std::vector<std::size_t> z_array(len);

  for (std::size_t i = 0; i < len; ++i) {
    std::cin >> z_array[i];
  }

  StringBuilder string_builder;
  std::vector<std::size_t> pref_array =
      string_builder.buildPrefixFunctionFromZFunction(z_array);

  std::cout << string_builder.buildFromPrefixFunction(pref_array) << '\n';

  return 0;
}