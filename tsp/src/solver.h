#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include "datatype.h"

namespace tsp {

class Solver {
public:
  Solver(const std::vector<Point> &cities_) : cities(cities_) {
    n_city = cities.size();
    /*
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
    */
  }

  virtual ~Solver(){};
  virtual Solution solve() = 0;

  size_t num_cities() const { return cities.size(); }

  double dist_between_cities(const size_t i, const size_t j) const {
    // return distances[i][j];
    return cities[i].distance_to(cities[j]);
  }

  bool is_valid(const std::vector<size_t> &order) const {
    size_t idx = 0;
    for (size_t i = 0; i < n_city; i++) {
      idx = order[idx];
    }
    return idx == 0;
  }

  double solution_value(const std::vector<size_t> &order) const {
    double val = 0.;
    size_t idx = 0;
    for (size_t i = 0; i < n_city; i++) {
      val += dist_between_cities(idx, order[idx]);
      idx = order[idx];
    }
    return val;
  }

protected:
  std::vector<Point> cities;
  std::vector<std::vector<double>> distances;
  size_t n_city;
};

} // namespace tsp

#endif
