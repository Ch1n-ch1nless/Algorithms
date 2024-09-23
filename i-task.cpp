#include <algorithm>
#include <iostream>
#include <vector>

class DynamicArray
{
 private:
  int   n_    = 0;
  int   m_    = 0; 
  int** data_ = nullptr;

 public:
  DynamicArray  (int n, int m);
  ~DynamicArray ();

  int* operator [](int i);

  void Clear();
};

DynamicArray::DynamicArray(int n, int m) : n_(n), m_(m), data_(nullptr)  {
  data_ = new int*[n_];

  for (int i = 0; i < n_; ++i) {
    data_[i] = new int[m_];
  }  

  Clear();
}

DynamicArray::~DynamicArray() {
  for (int i = 0; i < n_; ++i) {
    delete[] data_[i];
  }

  delete[] data_;
}

int* DynamicArray::operator[](int i) {
  return data_[i];
}

void DynamicArray::Clear() {
  for (int i = 0; i < n_; ++i) {
    for (int j = 0; j < m_; ++j) {
      data_[i][j] = 0;
    }
  }
}

bool Compare(int sign, int elem1, int elem2) {
  return (sign == 0) ? elem1 < elem2 : elem1 > elem2;
}

void FindLargestAlternatingSequence(const std::vector<int>& src_seq,
                                          std::vector<int>& res_seq) {
  int n     = src_seq.size();
  int low   = 0;
  int great = 1;

  DynamicArray dp(n, 2);

  dp[0][low]   = 1;
  dp[0][great] = 1;

  for (int i = 1; i < n; ++i) {
    for (int j = 0; j < i; ++j) {
      if (src_seq[i] < src_seq[j]) {
        dp[i][low] = std::max(dp[i][low], dp[j][great] + 1);
      } else if (src_seq[i] > src_seq[j]) {
        dp[i][great] = std::max(dp[i][great], dp[j][low] + 1);
      }
    }
  }

  int max_len  = 0;
  int pos      = 0;
  int sign     = 0;

  for (int i = 0; i < n; ++i) {
    if (dp[i][great] > max_len) {
      max_len  = dp[i][great];
      sign     = great;
      pos      = i;
    }
    if (dp[i][low] > max_len) {
      max_len  = dp[i][low];
      sign     = low;
      pos      = i;
    }
  }

  res_seq.push_back(src_seq[pos]);
  
  int cur_elem = src_seq[pos];

  while (max_len > 1) {
    --max_len;
    sign = 1 - sign;
    --pos;

    while (dp[pos][sign] != max_len || !Compare(sign, src_seq[pos], cur_elem)) {
      --pos;
    }

    res_seq.push_back(src_seq[pos]);
    cur_elem = src_seq[pos];
  }

  std::reverse(res_seq.begin(), res_seq.end());
}

void ReadVector(std::vector<int>& vec, int vec_len) {
  int temp_var = 0;

  for (int i = 0; i < vec_len; ++i) {
    std::cin >> temp_var;
    vec.push_back(temp_var);
  }
}

void PrintVector(const std::vector<int>& vec, int vec_len) {
  std::cout << vec_len << '\n';

  for (int i = 0; i < vec_len; ++i) {
    std::cout << vec[i] << ' ';
  }
}

int main() {
  int number_of_regions = 0;
  std::cin >> number_of_regions;

  std::vector<int> gangster_sequence;
  std::vector<int> answer_sequence;

  ReadVector(gangster_sequence, number_of_regions);

  FindLargestAlternatingSequence(gangster_sequence, answer_sequence);

  PrintVector(answer_sequence, answer_sequence.size());

  return 0;
}