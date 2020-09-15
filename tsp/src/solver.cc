#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

#include "solver.h"

namespace tsp_sa {

city_order_type Solver::solve(const CityMap &city_map) {
  auto init_order = warm_start(city_map);
  return solve(city_map, init_order);
}

city_order_type Solver::solve(const CityMap &city_map,
                              const city_order_type &init_order) {

  const auto n_city = city_map.get_city_number();
  const auto n_move = tsp_moves_.size();

  assert(n_city > 0);
  assert(n_move > 0);

  for (auto tsp_move : tsp_moves_) {
    tsp_move->set_city_number(n_city);
  }

  std::random_device r;
  auto rd_gen = std::mt19937_64(r());
  auto move_dist = std::uniform_int_distribution<size_t>(0, n_move - 1);
  auto acceptable_dist = std::uniform_real_distribution<double>(0.0, 1.0);

  const int max_outer_iter = config_.max_outer_iter_;
  const int max_inner_iter = config_.max_inner_iter_;

  city_order_type curr_sol = init_order;
  //   std::shuffle(curr_sol.begin(), curr_sol.end(), rd_gen);
  city_order_type best_sol = curr_sol;

  auto curr_val = city_map.evaluate(curr_sol);
  auto best_val = curr_val;

  std::cerr << "Start with distance " << curr_val << std::endl;

  for (auto outer_iter = 0; outer_iter < max_outer_iter; outer_iter++) {
    auto curr_temperature = cooling_scheduler_->next_temperature();
    for (auto inner_iter = 0; inner_iter < max_inner_iter; inner_iter++) {
      auto idx = move_dist(rd_gen);

      auto energy_diff = tsp_moves_[idx]->propose(curr_sol, city_map);
      auto acceptable_cutoff = acceptable_dist(rd_gen);

      if (is_acceptable(energy_diff, curr_temperature, acceptable_cutoff)) {
        tsp_moves_[idx]->apply_move(curr_sol);
        // curr_val = city_map.evaluate(curr_sol);
        curr_val = curr_val + energy_diff;
        if (curr_val < best_val) {
          best_val = curr_val;
          best_sol = curr_sol;
        }
      }
    }

    if ((1 + outer_iter) % 100 == 0) {
      std::cerr << "Iteration: " << 1 + outer_iter
                << ", Temperature: " << curr_temperature
                << ", Best sol: " << best_val << "\n";
    }
  }
  return best_sol;
}

city_order_type Solver::warm_start(const CityMap &city_map) const {
  const auto n_city = city_map.get_city_number();
  city_order_type idx(n_city);
  std::iota(idx.begin(), idx.end(), 0);
  return idx;
}

bool Solver::is_acceptable(const double energy_diff, const double temperature,
                           const double acceptable_cutoff) const {
  /* when energy_diff is negative, the exponential is bigger than 1 */
  return std::exp(-energy_diff / temperature) > acceptable_cutoff;
}

void Solver::register_tsp_move(TSPMove *tsp_move) {
  tsp_moves_.push_back(tsp_move);
}

void Solver::register_cooling_schedular(CoolingScheduler *cooler) {
  cooling_scheduler_ = cooler;
}

} // namespace tsp_sa
