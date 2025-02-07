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

int main() {
  std::size_t len = 0;
  std::cin >> len;

  std::vector<std::size_t> prefix_array(len);

  for (std::size_t i = 0; i < len; ++i) {
    std::cin >> prefix_array[i];
  }

  StringBuilder string_builder;
  std::cout << string_builder.buildFromPrefixFunction(prefix_array) << '\n';

  return 0;
}