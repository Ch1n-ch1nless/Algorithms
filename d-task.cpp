#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

static constexpr double kPrecision = 1e-7;

// ====================================================================
// Point
// ====================================================================
struct Point {
  double x;
  double y;

  Point() {}
  Point(const double& x, const double& y) : x(x), y(y) {}
  Point(const Point& begin, const Point& end)
      : x(end.x - begin.x), y(end.y - begin.y) {}

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

  Point& operator*=(const double& lambda) {
    x *= lambda;
    y *= lambda;
    return *this;
  }

  Point& operator/=(const double& lambda) {
    x /= lambda;
    y /= lambda;
    return *this;
  }

  double getNorm() const { return std::sqrt(x * x + y * y); }
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

Point operator*(const Point& point, const double& lambda) {
  Point res = point;
  res *= lambda;
  return res;
}

Point operator/(const Point& point, const double& lambda) {
  Point res = point;
  res /= lambda;
  return res;
}

double operator*(const Point& lhs, const Point& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

double operator^(const Point& lhs, const Point& rhs) {
  return lhs.x * rhs.y - lhs.y * rhs.x;
}

double CalculateDistance(const Point& lhs, const Point& rhs) {
  Point diff = lhs - rhs;
  return diff.getNorm();
}

Point Rotate(const Point& vector, const double& angle) {
  Point new_vector{};
  new_vector.x = vector.x * std::cos(angle) - vector.y * std::sin(angle);
  new_vector.y = vector.y * std::cos(angle) + vector.x * std::sin(angle);
  return new_vector;
}

// ====================================================================
// Segment
// ====================================================================
struct Segment {
  Point begin;
  Point end;

  Segment() = default;
  Segment(const Point& begin, const Point& end) : begin(begin), end(end) {}

  double len() {
    Point diff = end - begin;
    return diff.getNorm();
  }
};

double CalculateDistance(const Point& point, const Segment& seg) {
  Point left_point = seg.begin;
  Point right_point = seg.end;

  while (CalculateDistance(left_point, right_point) > kPrecision) {
    Point point1 = left_point + (right_point - left_point) / 3.;
    Point point2 = right_point - (right_point - left_point) / 3.;

    if (CalculateDistance(point, point1) > CalculateDistance(point, point2)) {
      left_point = point1;
    } else {
      right_point = point2;
    }
  }
  
  return CalculateDistance(point, (left_point + right_point) / 2);
}

// ====================================================================

std::vector<Point> ReorderPolygon(const std::vector<Point>& polygon) {
  std::vector<Point> reordered_polygon(polygon.size());

  std::size_t pos = 0;
  for (std::size_t i = 1; i < polygon.size(); ++i) {
    if (polygon[i].y < polygon[pos].y || (polygon[i].y == polygon[pos].y && polygon[i].x < polygon[pos].x)) {
      pos = i;
    }
  }

  for (std::size_t i = pos; i < polygon.size(); ++i) {
    reordered_polygon[i - pos] = polygon[i];
  }

  std::size_t shift = polygon.size() - pos;
  for (std::size_t i = 0; i < pos; ++i) {
    reordered_polygon[shift + i] = polygon[i];
  }

  return reordered_polygon;
}

std::vector<Point> CalculateMinkowskiAddition(std::vector<Point>& lhs,
                                              std::vector<Point>& rhs) {
  lhs.push_back(lhs[0]);
  lhs.push_back(lhs[1]);

  rhs.push_back(rhs[0]);
  rhs.push_back(rhs[1]);

  std::vector<Point> minkowski_sum{};
  std::size_t left_index = 0;
  std::size_t right_index = 0;

  while ((left_index + 2 < lhs.size()) || (right_index + 2 < rhs.size())) {
    minkowski_sum.push_back(lhs[left_index] + rhs[right_index]);

    double cross_product = (lhs[left_index + 1] - lhs[left_index]) ^ (rhs[right_index + 1] - rhs[right_index]);
    if (cross_product + kPrecision >= 0) {
      ++left_index;
    } 
    if (cross_product <= kPrecision) {
      ++right_index;
    }
  }

  return minkowski_sum;
}

std::vector<Point> CalculatePolygonIntersection(std::vector<Point>& lhs,
                                                std::vector<Point>& rhs) {
  for (std::size_t i = 0; i < rhs.size(); ++i) {
    rhs[i] *= -1.0;
  }

  rhs = ReorderPolygon(rhs);

  return CalculateMinkowskiAddition(lhs, rhs);
}

// ====================================================================

double FindMinAirportClearTime(std::vector<Point> airport, 
                               std::vector<Point> cloud) {
  airport = ReorderPolygon(airport);
  cloud = ReorderPolygon(cloud);

  auto intersection = CalculatePolygonIntersection(airport, cloud);

  Point zero_point{0.0, 0.0};

  double min_dist = 1e9;
  for (std::size_t i = 0; i < intersection.size(); ++i) {
    Segment seg{intersection[i], intersection[(i + 1)% intersection.size()]};
    min_dist = std::min(min_dist, CalculateDistance(zero_point, seg));
  }

  return min_dist;
}

int main() {
  std::size_t airport_size = 0;
  std::size_t cloud_size = 0;
  std::cin >> airport_size >> cloud_size;

  std::vector<Point> airport(airport_size);
  std::vector<Point> cloud(cloud_size);

  for (std::size_t i = 0; i < airport_size; ++i) {
    std::cin >> airport[i];
  }

  for (std::size_t i = 0; i < cloud_size; ++i) {
    std::cin >> cloud[i];
  }

  double time = FindMinAirportClearTime(airport, cloud);
  std::cout << std::fixed << std::setprecision(6);
  std::cout << ((time >= 60.0) ? time - 60.0 : 0.0) << '\n';

  return 0;
}
