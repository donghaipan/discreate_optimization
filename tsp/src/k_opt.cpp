#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <random>
#include <unordered_set>

#include "k_opt.h"

namespace tsp {

class TabuList {
public:
  TabuList(const size_t n) : n(n) {}
  bool has_key(const size_t key) const { return m.find(key) != m.end(); }

  void add_key(const size_t key) {

    if (q.size() > n) {
      auto k = q.front();
      q.pop();
      m.erase(k);
    }

    q.push(key);
    m.insert(key);
  }

private:
  std::unordered_set<size_t> m;
  std::queue<size_t> q;
  size_t n;
};

Solution KOpt::solve() {
  double best_val = -1;
  order_type best_order;
  auto rng = std::default_random_engine{};

  for (size_t rand_iter = 0; rand_iter < 1000; rand_iter++) {
    auto order = random_init(rng);
    // auto order = warm_start();
    // assert(is_valid(order));
    // std::cerr << "start with random solution, value: " <<
    // solution_value(order)
    //           << std::endl;

    auto tabu = TabuList(n_city / 10);
    std::uniform_int_distribution<size_t> dist(0, n_city - 1);

    auto max_iter = n_city * 10;
    for (size_t iter = 0; iter < max_iter; iter++) {
      auto idx = dist(rng);

      while (tabu.has_key(idx)) {
        idx = dist(rng);
      }

      tabu.add_key(idx);
      order = search(order, idx);

      //   assert(is_valid(order));
      // if ((iter + 1) % 100 == 0) {
      //   std::cerr << "iteration: " << iter + 1
      //             << ", value: " << solution_value(order) << std::endl;
      // }
    }

    auto val = solution_value(order);
    if (best_val < 0 || val < best_val) {
      best_val = val;
      best_order = order;
    }
    std::cerr << "iteration: " << rand_iter + 1 << ", value: " << val
              << " best val: " << best_val << std::endl;
  }
  return Solution(best_order, best_val);
}

order_type KOpt::warm_start() const {
  order_type order;
  order.resize(n_city);
  for (size_t i = 0; i < n_city - 1; i++) {
    order[i] = i + 1;
  }
  order.back() = 0;
  return order;
}

order_type KOpt::random_init(std::default_random_engine &rng) const {
  order_type idx;
  idx.resize(n_city - 1);

  for (size_t i = 0; i + 1 < n_city; i++) {
    idx[i] = i + 1;
  }

  std::shuffle(idx.begin(), idx.end(), rng);

  order_type order;
  order.resize(n_city);
  order[0] = idx.front();
  for (size_t i = 0; i + 2 < n_city; i++) {
    order[idx[i]] = idx[i + 1];
  }
  order[idx.back()] = 0;
  return order;
}

void KOpt::reverse_order(const size_t start0, const size_t start1,
                         order_type &order) const {
  auto end0 = order[start0];
  auto end1 = order[start1];
  auto idx = order[start0];
  auto idx_next = order[idx];
  while (idx != start1) {
    auto tmp = order[idx_next];
    order[idx_next] = idx;
    idx = idx_next;
    idx_next = tmp;
  }

  order[start0] = start1;
  order[end0] = end1;
}

order_type KOpt::search(const order_type &order, const size_t start_idx) const {
  double g_opt = 0., g_curr = 0.;
  double g_possible;

  order_type curr_order = order;
  order_type best_order = order;

  size_t end_idx = curr_order[start_idx];

  size_t possible_start = curr_order[end_idx];
  size_t possible_end = curr_order[possible_start];

  std::unordered_set<size_t> used_points({start_idx, end_idx});

  while (possible_end != start_idx) {
    g_possible = dist_between_cities(start_idx, end_idx) -
                 dist_between_cities(end_idx, possible_end);

    if (used_points.find(possible_end) == used_points.end() && g_possible > 0) {
      /* check the distance after close the tour */
      double g_tmp = g_curr + g_possible +
                     dist_between_cities(possible_start, possible_end) -
                     dist_between_cities(start_idx, possible_start);

      if (g_tmp > 0) {
        /* update the current tour */
        reverse_order(start_idx, possible_start, curr_order);

        used_points.insert({possible_start, possible_end});

        /* update the best tour */
        if (g_tmp > g_opt) {
          g_opt = g_tmp;
          best_order = curr_order;
        }

        /* update position index for next iteration */
        end_idx = possible_start;
        possible_start = curr_order[end_idx];
        possible_end = curr_order[possible_start];
        g_curr = g_tmp;
      }
    }

    possible_start = possible_end;
    possible_end = curr_order[possible_end];
  }

  return best_order;
}

} // namespace tsp
