#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

std::vector<int>
BuildLCP(const std::string& str, const std::vector<int>& suf_array) {
  int len = static_cast<int>(suf_array.size());
  std::vector<int> lcp_array(len);
  std::vector<int>  pos(len);
  
  for (int i = 0; i < len; ++i) {
    pos[suf_array[i]] = i;
  }

  int k = 0;
  
  for (int i = 0; i < len; ++i) {
    if (k > 0) { k--; }
    
    if (pos[i] == len - 1) {
      lcp_array[pos[i]] = -1;
      k = 0;
    } else {
      int j = suf_array[pos[i] + 1];
      while (std::max(i + k, j + k) < len && str[i + k] == str[j + k]) {
        ++k;
      }
      lcp_array[pos[i]] = k;
    }
  }

  return lcp_array;
}

int main() {
  std::size_t len = 0;
  std::cin >> len;

  std::string str;
  std::cin >> str;

  std::vector<int> suffix_array(len);

  for (std::size_t i = 0; i < len; ++i) {
    std::cin >> suffix_array[i];
    --suffix_array[i];
  }

  std::vector<int> lcp_array = BuildLCP(str, suffix_array);

  for (std::size_t i = 0; i < lcp_array.size() - 1; ++i) {
    std::cout << lcp_array[i] << " ";
  }
  std::cout << "\n";

  return 0;
}
