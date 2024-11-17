#include <iostream>
#include <limits>
#include <vector>

static const int64_t INF = 2147483647;

class Line {
 public:
  Line() {}
  Line(const int64_t& k, const int64_t& b) {
    k_ = k;
    b_ = b;
  }

  int64_t GetY(const int64_t& x) const { return k_ * x + b_; }

  int64_t GetK() const { return k_; }
  int64_t GetB() const { return b_; }

 private:
  int64_t k_;
  int64_t b_;
};

int64_t CrossLines(const Line& line1, const Line& line2) {
  int64_t x = (line2.GetB() - line1.GetB()) / (line1.GetK() - line2.GetK());

  if (line2.GetB() < line1.GetB()) x--;

  return x;
}

class ConvexHullTrick {
 public:
  ConvexHullTrick() = default;
  ~ConvexHullTrick() = default;

  void AddLine(Line new_line);
  int64_t GetMin(const int64_t& x);

 private:
  std::vector<int64_t> points_;
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

int64_t ConvexHullTrick::GetMin(const int64_t& x) {
  int64_t line_index =
      std::lower_bound(points_.begin(), points_.end(), x) - points_.begin() - 1;

  return lines_[line_index].GetY(x);
}

class MinSumSquaredLensCalculator {
 public:
  int64_t operator()(const size_t& max_points_number,
                     const size_t& max_segment_number);

 private:
  std::vector<std::vector<int64_t>> min_sum_of_squared_lens;

  const int64_t COEFFICIENT_K = -2;
  const int64_t COEFFICIENT_B = 1;
};

int64_t MinSumSquaredLensCalculator::operator()(
    const size_t& max_points_number, const size_t& max_segment_number) {

  min_sum_of_squared_lens.assign(
      max_segment_number + 1, std::vector<int64_t>(max_points_number + 1, INF));

  for (size_t seg_count = 0; seg_count <= max_segment_number; ++seg_count) {
    min_sum_of_squared_lens[seg_count][0] = 0ll;
  }

  for (size_t seg_count = 1; seg_count <= max_segment_number; ++seg_count) {
    ConvexHullTrick trick;
    for (size_t point_count = 1; point_count <= max_points_number;
         ++point_count) {
      trick.AddLine(
          Line(COEFFICIENT_K * point_count,
               COEFFICIENT_B *
                   (min_sum_of_squared_lens[seg_count - 1][point_count - 1] +
                    (point_count) * (point_count))));

      min_sum_of_squared_lens[seg_count][point_count] =
          trick.GetMin(point_count) + point_count * point_count;
    }
  }

  if (min_sum_of_squared_lens[max_segment_number][max_points_number] < INF) {
    return min_sum_of_squared_lens[max_segment_number][max_points_number];
  }

  return 0;
}

int main() {
  size_t points_number = 0;
  size_t segment_number = 0;

  std::cin >> points_number >> segment_number;

  MinSumSquaredLensCalculator min_sum_of_squared_lens_of_segments;

  int64_t answer =
      min_sum_of_squared_lens_of_segments(points_number, segment_number);

  std::cout << answer << '\n';

  return 0;
}