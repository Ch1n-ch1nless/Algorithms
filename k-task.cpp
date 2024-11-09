#include <iostream>
#include <limits>
#include <vector>

static const long long INF =
    static_cast<long long>(std::numeric_limits<int>::max());
static const long long COEFFICIENT_K = -2;
static const long long COEFFICIENT_B = 1;

class Line {
 public:
  Line() {}
  Line(const long long& k, const long long& b) {
    k_ = k;
    b_ = b;
  }

  long long GetY(const long long& x) { return k_ * x + b_; }

  long long GetK() { return k_; }
  long long GetB() { return b_; }

 private:
  long long k_;
  long long b_;
};

long long CrossLines(Line& line1, Line& line2) {
  long long x = (line2.GetB() - line1.GetB()) / (line1.GetK() - line2.GetK());

  if (line2.GetB() < line1.GetB()) x--;

  return x;
}

class ConvexHullTrick {
 public:
  ConvexHullTrick() = default;
  ~ConvexHullTrick() = default;

  void AddLine(Line new_line);
  long long GetMin(const long long& x);

 private:
  std::vector<long long> points_;
  std::vector<Line> lines_;
};

void ConvexHullTrick::AddLine(Line new_line) {
  while (lines_.size() > 0 &&
         (lines_.back().GetY(points_.back()) > new_line.GetY(points_.back()))) {
    lines_.pop_back();
    points_.pop_back();
  }

  if (lines_.empty()) {
    points_.push_back(-INF);
  } else {
    points_.push_back(CrossLines(lines_.back(), new_line));
  }
  lines_.push_back(new_line);
}

long long ConvexHullTrick::GetMin(const long long& x) {
  long long line_index =
      std::lower_bound(points_.begin(), points_.end(), x) - points_.begin() - 1;

  return lines_[line_index].GetY(x);
}

long long FindMinSumSquareLens(const long long& max_points_number,
                               const long long& max_segment_number) {
  std::vector<std::vector<long long>> sum_lens_dp(
      max_segment_number + 1,
      std::vector<long long>(max_points_number + 1, INF));

  for (long long seg_count = 0; seg_count <= max_segment_number; ++seg_count) {
    sum_lens_dp[seg_count][0] = 0ll;
  }

  for (long long seg_count = 1; seg_count <= max_segment_number; ++seg_count) {
    ConvexHullTrick trick;
    for (long long point_count = 1; point_count <= max_points_number;
         ++point_count) {
      trick.AddLine(
          Line(COEFFICIENT_K * point_count,
               COEFFICIENT_B * (sum_lens_dp[seg_count - 1][point_count - 1] +
                                (point_count) * (point_count))));
      sum_lens_dp[seg_count][point_count] =
          trick.GetMin(point_count) + point_count * point_count;
    }
  }

  if (sum_lens_dp[max_segment_number][max_points_number] < INF) {
    return sum_lens_dp[max_segment_number][max_points_number];
  }

  return 0;
}

int main() {
  long long segment_number = 0;
  long long points_number = 0;

  std::cin >> points_number >> segment_number;

  long long answer = FindMinSumSquareLens(points_number, segment_number);

  std::cout << answer << '\n';

  return 0;
}