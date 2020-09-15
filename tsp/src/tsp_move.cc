#include <algorithm>

#include "tsp_move.h"

namespace tsp_sa {

double TwoOPTMove::propose(const city_order_type &city_order,
                           const CityMap &city_map) {
  rand_city();

  auto i_prev = city_map.get_prev_idx(i_);
  auto j_prev = city_map.get_prev_idx(j_);

  return city_map.dist_between_cities(city_order[i_prev], city_order[j_prev]) +
         city_map.dist_between_cities(city_order[i_], city_order[j_]) -
         city_map.dist_between_cities(city_order[i_prev], city_order[i_]) -
         city_map.dist_between_cities(city_order[j_prev], city_order[j_]);
}

void TwoOPTMove::apply_move(city_order_type &city_order) {
  std::reverse(city_order.begin() + i_, city_order.begin() + j_);
}

double SwapTwoCity::propose(const city_order_type &city_order,
                            const CityMap &city_map) {
  rand_city();

  auto i_prev = city_map.get_prev_idx(i_);
  auto i_next = city_map.get_next_idx(i_);

  auto j_prev = city_map.get_prev_idx(j_);
  auto j_next = city_map.get_next_idx(j_);
  /*
    auto energy_diff = 0.;
    for (auto k : {i_prev, i_next}) {
      energy_diff += city_map.dist_between_cities(city_order[k], city_order[j])
    - city_map.dist_between_cities(city_order[k], city_order[i]);
    }

    for (auto k : {j_prev, j_next}) {
      energy_diff += city_map.dist_between_cities(city_order[k], city_order[i])
    - city_map.dist_between_cities(city_order[k], city_order[j]);
    }
  */
  auto old_val = 0.;
  auto new_val = 0.;

  for (auto k : {i_prev, i_next}) {
    new_val += city_map.dist_between_cities(city_order[k], city_order[j_]);
    old_val += city_map.dist_between_cities(city_order[k], city_order[i_]);
  }

  for (auto k : {j_prev, j_next}) {
    new_val += city_map.dist_between_cities(city_order[k], city_order[i_]);
    old_val += city_map.dist_between_cities(city_order[k], city_order[j_]);
  }
  return new_val - old_val;
}

void SwapTwoCity::apply_move(city_order_type &city_order) {
  std::swap(city_order[i_], city_order[j_]);
}

} // namespace tsp_sa
