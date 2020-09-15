#ifndef __SA_DATATYPE_H__
#define __SA_DATATYPE_H__

#include <cmath>
#include <vector>

namespace tsp_sa {

typedef std::vector<size_t> city_order_type;

struct City {
  double x_;
  double y_;
  City(const double x, const double y) : x_(x), y_(y) {}
  double dist_to(const City &other) const {
    return std::sqrt(std::pow(x_ - other.x_, 2) + std::pow(y_ - other.y_, 2));
  }
};

class CityMap {
public:
  CityMap(const std::vector<City> &cities)
      : cities_(cities), n_city_(cities.size()) {}

  double dist_between_cities(const size_t i, const size_t j) const {
    return cities_.at(i).dist_to(cities_.at(j));
  }
  size_t get_city_number() const { return n_city_; }
  size_t get_next_idx(const size_t idx) const {
    return (idx + 1) == n_city_ ? 0 : idx + 1;
  }
  size_t get_prev_idx(const size_t idx) const {
    return idx ? idx - 1 : n_city_ - 1;
  }

  double evaluate(const city_order_type &sol) const {
    double result = 0.;
    for (size_t i = 0; i < sol.size() - 1; i++) {
      result += dist_between_cities(sol.at(i), sol.at(i + 1));
    }
    result += dist_between_cities(sol.front(), sol.back());
    return result;
  }

private:
  std::vector<City> cities_;
  size_t n_city_;
};

} // namespace tsp_sa

#endif
