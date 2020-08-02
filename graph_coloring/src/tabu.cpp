#include "tabu.h"
#include "color_solver.h"
#include "graph.h"

#include <random>
#include <unordered_set>

template <typename T> T random_sample(const std::vector<T> &vec) {
  auto n_element = vec.size();
  std::default_random_engine random_gen;
  std::uniform_int_distribution<size_t> dist(0, n_element - 1);
  auto sample_idx = dist(random_gen);
  return vec[sample_idx - 1];
}

Solution TabuSearch::solve() {
  auto sol = warm_start();

  auto feasible_color_number = sol.num_color();
  auto feasible_colors = sol.get_colorings();

  std::cout << "start with solution:" << std::endl;
  std::cout << sol << std::endl;

  for (int num_color = feasible_color_number - 1; num_color > 0; num_color--) {
    auto colors = randomize_init_colors(feasible_colors, num_color);
    auto new_sol = search(colors, num_color);
    if (new_sol.is_valid()) {
      feasible_colors = new_sol.get_colorings();

      std::cout << "Got a new solution with color number: " << num_color
                << std::endl;
    } else {
      break;
    }
  }
  sol.update_solution(feasible_colors);
  return sol;
}

Solution TabuSearch::warm_start() {
  std::vector<size_t> idx(n_node);
  for (size_t i = 0; i < n_node; ++i) {
    idx[i] = i;
  }
  return greedy(idx);
}

Solution TabuSearch::search(const std::vector<int> &init_colors,
                            const int num_color) const {
  auto colors(init_colors);
  auto violation = calculate_violation(colors);
  const auto max_iter = 10000;
  const size_t tabu_size = 500;
  const size_t neighbor_size = 2000;
  auto n_iter = 0;
  auto tabu_list = TabuList(tabu_size);

  while (n_iter < max_iter && violation.get_total_violation() > 0) {
    /* initialize violation nodes */
    std::vector<int> violation_nodes;
    for (int i = 0; i < n_node; i++) {
      if (violation.get_node_violation(i) > 0) {
        violation_nodes.push_back(i);
      }
    }

    /* create rep list */
    std::vector<NodeMove> neighbor_list;

    std::default_random_engine random_gen;
    std::uniform_int_distribution<int> color_dist(1, num_color);
    std::uniform_int_distribution<size_t> node_dist(0,
                                                    violation_nodes.size() - 1);

    auto max_trial = neighbor_size * 5;
    auto curr_trial = 0;
    while (neighbor_list.size() < neighbor_size && curr_trial < max_trial) {
      curr_trial++;
      auto move_node_idx = node_dist(random_gen);
      auto move_node = violation_nodes[move_node_idx];
      auto move_color = color_dist(random_gen);

      if (move_color == colors[move_node]) {
        continue;
      }
      /*
      while (move_color == colors[move_node]) {
        move_color = color_dist(random_gen);
      }
      */

      auto node_move = NodeMove(move_node, move_color);

      if (tabu_list.has_tabu(node_move)) {
        continue;
      }

      node_move.violation_value_ =
          evaluate_violation(node_move, violation, colors);
      neighbor_list.push_back(node_move);
      if (node_move.violation_value_ < violation.get_total_violation()) {
        break;
      }
    }

    /* select best move */
    bool flag = neighbor_list.empty();
    if (flag) {
      std::cout << "neighbor_list empty" << std::endl;
    }

    size_t best_idx = 0;
    int best_val = neighbor_list.front().violation_value_;
    for (size_t i = 1; i < neighbor_list.size(); i++) {
      if (neighbor_list[i].violation_value_ < best_val) {
        best_idx = i;
        best_val = neighbor_list[i].violation_value_;
      }
    }
    /* move */
    apply_move(neighbor_list[best_idx], violation, colors);
    tabu_list.push_tabu(neighbor_list[best_idx]);
    n_iter++;

    if (n_iter % 500 == 0) {
      std::cout << "Iteration: " << n_iter
                << ", violation: " << violation.get_total_violation()
                << std::endl;
    }
  }

  return Solution(num_color, colors, violation.get_total_violation() == 0);
}

void TabuSearch::apply_move(const NodeMove &node_move, Violation &violation,
                            std::vector<int> &colors) const {

  const auto idx = node_move.node_;
  const auto curr_color = colors[idx];
  const auto to_color = node_move.color_;

  for (auto i : edges.at(idx)) {
    if (colors[i] == curr_color) {
      violation.decrease(idx, i);
    } else if (colors[i] == to_color) {
      violation.increase(idx, i);
    }
  }
  colors[idx] = to_color;
}

int TabuSearch::evaluate_violation(const NodeMove &node_move,
                                   const Violation &violation,
                                   const std::vector<int> &colors) const {

  const auto idx = node_move.node_;
  const auto curr_color = colors[idx];
  const auto to_color = node_move.color_;
  auto violation_val = violation.get_total_violation();

  for (auto i : edges.at(idx)) {
    if (colors[i] == curr_color) {
      violation_val -= 2;
    } else if (colors[i] == to_color) {
      violation_val += 2;
    }
  }
  return violation_val;
}

std::vector<int>
TabuSearch::randomize_init_colors(const std::vector<int> &colors,
                                  const int num_color) const {
  std::default_random_engine random_gen;
  std::uniform_int_distribution<int> dist(1, num_color);

  auto cut_off = dist(random_gen);
  std::vector<int> new_color(colors);

  for (size_t i = 0; i < new_color.size(); ++i) {
    if (new_color[i] > cut_off) {
      new_color[i]--;
    }
    /*
    else if (new_color[i] == cut_off) {
      new_color[i] = dist(random_gen);
    }
    */
  }

  for (auto c : new_color) {
    if (c <= 0 || c > num_color) {
      std::cout << "bad initial value" << std::endl;
    }
  }
  return new_color;
}

Violation
TabuSearch::calculate_violation(const std::vector<int> &colors) const {
  std::vector<int> node_violation(n_node, 0);
  int total = 0;

  for (int i = 0; i < n_node; ++i) {
    if (edges.find(i) != edges.end()) {
      for (auto j : edges.at(i)) {
        if (colors[i] == colors[j]) {
          node_violation[i]++;
          total++;
        }
      }
    }
  }
  return Violation(node_violation, total);
}
