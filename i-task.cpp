#include <algorithm>
#include <iostream>
#include <vector>

static int LOW = 0;   // sign '<'
static int GREAT = 1; // sign '>'

std::vector<int>
FindLargestAlternatingSequence(const std::vector<int> &src_seq) {
  int n = src_seq.size();

  //  alt_seq_len - vector, which contains max len of alternating sequence, 
  //which ends on some index of src_seq. 
  //  alt_seq_len[i][LOW]   := max len of alternating sequence, 
  //which ends on index i & exists j < i, that a[j] < a[i] 
  //  alt_seq_len[i][GREAT] := max len of alternating sequence, 
  //which ends on index i & exists j < i, that a[j] > a[i]
  std::vector<std::vector<int>> alt_seq_len(n, std::vector<int>(2, 0));

  alt_seq_len[0][LOW] = 1;
  alt_seq_len[0][GREAT] = 1;

  for (int i = 1; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (src_seq[i] < src_seq[j]) {
        alt_seq_len[i][LOW] =
            std::max(alt_seq_len[i][LOW], alt_seq_len[j][GREAT] + 1);
      } else if (src_seq[i] > src_seq[j]) {
        alt_seq_len[i][GREAT] =
            std::max(alt_seq_len[i][GREAT], alt_seq_len[j][LOW] + 1);
      }
    }
  }

  int max_len = 0;
  int pos = 0;
  int sign = 0;

  for (int i = 0; i < n; ++i) {
    if (alt_seq_len[i][GREAT] > max_len) {
      max_len = alt_seq_len[i][GREAT];
      sign = GREAT;
      pos = i;
    }
    if (alt_seq_len[i][LOW] > max_len) {
      max_len = alt_seq_len[i][LOW];
      sign = LOW;
      pos = i;
    }
  }

  std::vector<int> res_seq = {};

  res_seq.push_back(src_seq[pos]);

  int cur_elem = src_seq[pos];

  while (max_len > 1) {
    --max_len;
    sign = 1 - sign; // Change the sign
    --pos;

    while (alt_seq_len[pos][sign] != max_len ||
           !((sign == LOW) ? std::less(src_seq[pos], cur_elem)
                           : std::greater(src_seq[pos], cur_elem))) {
      --pos;
    }

    res_seq.push_back(src_seq[pos]);
    cur_elem = src_seq[pos];
  }

  std::reverse(res_seq.begin(), res_seq.end());

  return res_seq;
}

void ReadVector(std::vector<int> &vec, std::size_t vec_len) {
  int temp_var = 0;

  for (std::size_t i = 0; i < vec_len; ++i) {
    std::cin >> temp_var;
    vec.push_back(temp_var);
  }
}

void PrintVector(const std::vector<int> &vec) {
  std::cout << vec.size() << '\n';

  for (std::size_t i = 0; i < vec.size(); ++i) {
    std::cout << vec[i] << ' ';
  }
  std::cout << '\n';
}

int main() {
  std::size_t number_of_regions = 0;
  std::cin >> number_of_regions;

  std::vector<int> gangster_sequence;

  ReadVector(gangster_sequence, number_of_regions);

  std::vector<int> answer_sequence =
      FindLargestAlternatingSequence(gangster_sequence);

  PrintVector(answer_sequence);

  return 0;
}