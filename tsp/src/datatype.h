#ifndef TSP_POINT_H
#define TSP_POINT_H

#include <cmath>
#include <iomanip>
#include <ostream>
#include <vector>

namespace tsp {

struct Point {
  double x;
  double y;
  Point(const double x, const double y) : x(x), y(y) {}
  double distance_to(const Point &other) const {
    return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
  }
  friend std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
  }
};

class CityMap {
public:
  CityMap(const std::vector<Point> &cities_) : cities(cities_) {
    const auto n_city = cities.size();
    distances.resize(n_city);
    for (size_t i = 0; i < n_city; i++) {
      distances[i].resize(n_city);
    }

    for (size_t i = 0; i < n_city; i++) {
      distances[i][i] = 0.;
      for (size_t j = i + 1; j < n_city; j++) {
        const auto dist = cities_[i].distance_to(cities_[j]);
        distances[i][j] = dist;
        distances[j][i] = dist;
      }
    }
  };
  size_t num_cities() const { return cities.size(); }
  double dist_between_cities(const size_t i, const size_t j) const {
    return distances[i][j];
  }

private:
  std::vector<Point> cities;
  std::vector<std::vector<double>> distances;
};

class Solution {
public:
  Solution(const std::vector<size_t> &order_, const double dist_)
      : dist(dist_), order(order_) {}
  const std::vector<size_t> &get_order() const { return order; }
  double get_dist() const { return dist; }
  bool is_valid() const {
    auto n_city = order.size();

    size_t start_idx = 0;
    for (size_t i = 0; i < n_city; i++) {
      start_idx = order[start_idx];
    }

    return start_idx == 0;
  }

  friend std::ostream &operator<<(std::ostream &os, const Solution &sol) {
    os << std::fixed << std::setprecision(2) << sol.dist << " " << 0
       << std::endl;

    auto idx = 0;
    for (size_t i = 0; i < sol.order.size(); i++) {
      if (i > 0) {
        os << " ";
      }
      os << idx;
      idx = sol.order[idx];
    }
    return os;
  }

private:
  double dist;
  std::vector<size_t> order;
};

} // namespace tsp

#endif
