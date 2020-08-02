
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "color_solver.h"
#include "graph.h"
#include "iter_greedy.h"

Solution IterGreedy::greedy(const std::vector<size_t> &idx) const {
  std::vector<int> colors(n_node);
  int current_color = 1;
  for (const auto node_idx : idx) {
    if (!colors[node_idx]) {
      colors[node_idx] = current_color;

      auto connected_idx = std::unordered_set<int>();
      if (edges.find(node_idx) != edges.end()) {
        connected_idx.insert(edges.at(node_idx).begin(),
                             edges.at(node_idx).end());
      }

      for (const auto j : idx) {
        if ((!colors[j]) && connected_idx.find(j) == connected_idx.end()) {
          colors[j] = current_color;
          if (edges.find(j) != edges.end()) {
            connected_idx.insert(edges.at(j).begin(), edges.at(j).end());
          }
        }
      }
      current_color++;
    }
  }

  return Solution(current_color - 1, colors, true);
}

std::vector<size_t> IterGreedy::permute(const Solution &sol) const {
  const auto n_color = sol.num_color();
  const auto colors = sol.get_colorings();
  std::unordered_map<int, std::vector<size_t>> color_group;
  for (size_t i = 0; i < colors.size(); i++) {
    color_group[colors[i]].push_back(i);
  }

  std::vector<int> val(n_color);
  for (auto i = 0; i < n_color; ++i) {
    // color number starts with 1
    val[i] = i + 1;
  }

  std::random_shuffle(val.begin(), val.end());

  std::vector<size_t> idx;
  idx.reserve(colors.size());

  for (auto v : val) {
    for (auto i : color_group[v]) {
      idx.push_back(i);
    }
  }
  return idx;
}

Solution IterGreedy::solve() {
  std::srand(static_cast<unsigned int>(std::time(0)));

  std::vector<size_t> idx;
  idx.resize(n_node);
  for (size_t i = 0; i < n_node; ++i) {
    idx[i] = i;
  }

  // start with Welsh-Powell
  // descreasing order of node degree
  sort(idx.begin(), idx.end(), [&](const int lhs, const int rhs) {
    return edges[lhs].size() > edges[rhs].size();
  });

  auto sol = greedy(idx);

  int N = 3000;
  while (N--) {
    auto next_idx = permute(sol);
    auto next_sol = greedy(next_idx);
    sol.update_solution(next_sol);
  }

  return sol;
}
