#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <unordered_set>
#include <vector>

// ====================================================================
// Point
// ====================================================================
struct Point {
  int64_t x;
  int64_t y;

  Point& operator+=(const Point& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Point& operator-=(const Point& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Point& operator*=(const int64_t& lambda) {
    x *= lambda;
    y *= lambda;
    return *this;
  }

  Point& operator/=(const int64_t& lambda) {
    x /= lambda;
    y /= lambda;
    return *this;
  }
};

std::istream& operator>>(std::istream& in, Point& p) {
  in >> p.x >> p.y;
  return in;
}

std::ostream& operator<<(std::ostream& out, const Point& p) {
  out << p.x << ", " << p.y;
  return out;
}

Point operator+(const Point& lhs, const Point& rhs) {
  Point res = lhs;
  res += rhs;
  return res;
}

Point operator-(const Point& lhs, const Point& rhs) {
  Point res = lhs;
  res -= rhs;
  return res;
}

Point operator*(const Point& point, const int64_t& lambda) {
  Point res = point;
  res *= lambda;
  return res;
}

Point operator/(const Point& point, const int64_t& lambda) {
  Point res = point;
  res /= lambda;
  return res;
}

int64_t operator*(const Point& lhs, const Point& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

int64_t operator^(const Point& lhs, const Point& rhs) {
  return lhs.x * rhs.y - lhs.y * rhs.x;
}

bool operator==(const Point& lhs, const Point& rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool operator!=(const Point& lhs, const Point& rhs) { return !(lhs == rhs); }

int64_t GetCrossProduct(const Point& origin, const Point& lhs,
                        const Point& rhs) {
  Point vec1 = lhs - origin;
  Point vec2 = rhs - origin;
  return vec1 ^ vec2;
}

int64_t GetDistance(const Point& lhs, const Point& rhs = Point{0, 0}) {
  Point vec = rhs - lhs;
  return vec * vec;
}

namespace std {
template <>
struct hash<Point> {
  uint64_t operator()(const Point& p) const {
    return hash<uint64_t>()((static_cast<uint64_t>(p.x) << 32) |
                            static_cast<uint64_t>(p.y));
  }
};
}  // namespace std

// ====================================================================
// ConvexHullBuilder
// ====================================================================
class ConvexHullBuilder {
 public:
  ConvexHullBuilder(const std::vector<Point>& points);

  std::size_t getHullIndex(const Point& point);

 private:
  std::unordered_set<Point> points_set_;
  std::vector<std::vector<Point>> hulls_;

  std::vector<Point> buildConvexHullByJarvis(const std::vector<Point>& points);
  void buildConvexHulls();
  bool isPointInsideHull(const Point& point, const std::vector<Point>& hull);
  bool isPointInHull(const Point& point, const std::vector<Point>& hull);
};

ConvexHullBuilder::ConvexHullBuilder(const std::vector<Point>& points)
    : points_set_(points.begin(), points.end()) {
  buildConvexHulls();
}

void ConvexHullBuilder::buildConvexHulls() {
  while (points_set_.size() > 2) {
    std::vector<Point> internal_points(points_set_.begin(), points_set_.end());
    std::vector<Point> hull = buildConvexHullByJarvis(internal_points);
    if (hull.size() < 3) {
      break;
    }

    hulls_.push_back(hull);
    
    std::vector<Point> erased_points;
    for (const auto& point : points_set_) {
        if (isPointInHull(point, hull)) {
            erased_points.push_back(point);
        }
    }
    
    for (const auto& point : erased_points) {
        points_set_.erase(point);
    }
  }
}

std::vector<Point> ConvexHullBuilder::buildConvexHullByJarvis(
    const std::vector<Point>& points) {
  std::vector<Point> hull;
  if (points.empty()) return hull;

  Point base_point = points[0];
  for (const auto& p : points) {
    if ((p.y < base_point.y) || (p.y == base_point.y && p.x < base_point.x)) {
      base_point = p;
    }
  }
  hull.push_back(base_point);

  Point current_point = base_point;
  Point previous_point = base_point;
  previous_point.x -= 1;

  do {
    Point next_point = current_point;
    double min_angle = std::numeric_limits<double>::max();
    double max_distance = 0;

    for (const auto& candidate : points) {
      if (candidate == current_point) continue;

      Point prev_dir = current_point - previous_point;
      Point curr_dir = candidate - current_point;

      double angle = atan2(curr_dir.y, curr_dir.x) - atan2(prev_dir.y, prev_dir.x);
      if (angle < 0) angle += 2 * M_PI;

      int64_t distance = GetDistance(current_point, candidate);

      if (angle < min_angle || (angle == min_angle && distance > max_distance)) {
        min_angle = angle;
        max_distance = distance;
        next_point = candidate;
      }
    }

    previous_point = current_point;
    current_point = next_point;

    if (current_point != hull[0]) {
      hull.push_back(current_point);
    }

  } while (current_point != hull[0]);

  return hull;
}

bool ConvexHullBuilder::isPointInsideHull(const Point& point,
                                          const std::vector<Point>& hull) {
  for (std::size_t i = 0; i < hull.size(); ++i) {
    Point seg_begin = hull[i];
    Point seg_end = hull[(i + 1) % hull.size()];

    if (GetCrossProduct(seg_begin, seg_end, point) < 0) {
      return false;
    }
  }

  return true;
}

bool ConvexHullBuilder::isPointInHull(const Point& point,
                                      const std::vector<Point>& hull) {
  for (std::size_t i = 0; i < hull.size(); ++i) {
    Point seg_begin = hull[i];
    Point seg_end = hull[(i + 1) % hull.size()];

    if (GetCrossProduct(seg_begin, seg_end, point) == 0) {
      return true;
    }
  }

  return false;
}

std::size_t ConvexHullBuilder::getHullIndex(const Point& point) {
  std::size_t hull_count = 0;
  for (const auto& hull : hulls_) {
    if (isPointInsideHull(point, hull)) {
      ++hull_count;
    } else {
      break;
    }
  }

  return (hull_count > 0) ? hull_count - 1 : 0;
}

int main() {
  std::size_t n = 0;
  std::cin >> n;

  std::vector<Point> points(n);
  for (std::size_t i = 0; i < n; ++i) {
    std::cin >> points[i];
  }

  ConvexHullBuilder hull_builder(points);

  std::size_t requests_number = 0;
  std::cin >> requests_number;

  Point point;
  for (std::size_t request = 0; request < requests_number; ++request) {
    std::cin >> point;

    std::cout << hull_builder.getHullIndex(point) << '\n';
  }

  return 0;
}