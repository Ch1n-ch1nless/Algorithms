#include <cassert>
#include <iostream>
#include <string>
#include <vector>

class JoinedStringsIterator {
 public:
  JoinedStringsIterator(const std::string& s1, const std::string& s2);
  ~JoinedStringsIterator() = default;

  char operator[](std::size_t index);

 private:
  std::size_t len_s1;
  std::size_t len_s2;
  const std::string& s1_it;
  const std::string& s2_it;
};

JoinedStringsIterator::JoinedStringsIterator(const std::string& s1,
                                             const std::string& s2)
    : len_s1(s1.length()),
      len_s2(s2.length()),
      s1_it(s1),
      s2_it(s2) {}

char JoinedStringsIterator::operator[](std::size_t index) {
  if (index > len_s1 + len_s2) {
    assert(false && "ERROR!!! Incorrect index!");
    return '\0';
  }
  if (index == len_s1) {
    return '\0';
  } else if (index < len_s1) {
    return s1_it[index];
  }
  return s2_it[index-1-len_s1];
}

class PatternFinder {
 public:
  PatternFinder() = default;
  ~PatternFinder() = default;

  const std::vector<std::size_t>& getIndices(const std::string& text,
                                             const std::string& pattern);

 private:
  std::vector<std::size_t> indices;
  std::vector<std::size_t> pref_array;

  void buildPrefixFunction(JoinedStringsIterator& it, const std::size_t len);
};

void PatternFinder::buildPrefixFunction(JoinedStringsIterator& it, const std::size_t len) {
  pref_array.resize(len+1, 0);

  std::size_t k = 0;
  for (std::size_t i = 1; i <= len; ++i) {
    k = pref_array[i-1];
    while (k > 0 && it[i] != it[k]) {
      k = pref_array[k-1];
    }
    if (it[i] == it[k]) {
      ++k;
    }
    pref_array[i] = k;
  }
}

const std::vector<std::size_t>& PatternFinder::getIndices(
    const std::string& text, const std::string& pattern) {
  std::size_t pattern_len = pattern.length();
  std::size_t text_len = text.length();
  indices.clear();

  JoinedStringsIterator joined_iterator(pattern, text);
  buildPrefixFunction(joined_iterator, pattern_len + text_len);

  for (std::size_t i = 0; i <= text_len; ++i) {
    if (pref_array[pattern_len + i] == pattern_len) {
      indices.push_back(i-pattern_len);
    }
  }
  return indices;
}

int main() { 
  std::string text;
  std::string pattern;
  std::cin >> text >> pattern;

  PatternFinder pattern_finder;
  const std::vector<std::size_t> &indices = pattern_finder.getIndices(text, pattern);
  
  for (auto index : indices) {
    std::cout << index << '\n';
  }
  return 0; 
}