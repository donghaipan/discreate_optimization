#ifndef COLORING_SOLVER_H
#define COLORING_SOLVER_H

#include <algorithm>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"

class Solution {
public:
  Solution(const int n_color, const std::vector<int> &colors,
           const bool is_valid)
      : n_color_(n_color), colors_(colors), is_valid_(is_valid) {}

  Solution(const char *filename);

  void update_solution(const Solution &sol);
  void update_solution(const std::vector<int> &colors);

  void set_optimal() { is_optimal_ = true; }
  bool is_valid() const { return is_valid_; }
  bool is_optimal() const { return is_optimal_; }
  int num_color() const { return n_color_; }
  const std::vector<int> &get_colorings() const { return colors_; }

  friend std::ostream &operator<<(std::ostream &os, const Solution &sol);

private:
  void update(const int n_color, const std::vector<int> &colors) {
    n_color_ = n_color;
    colors_ = colors;
  }
  bool is_valid_;
  bool is_optimal_ = false;
  int n_color_;
  std::vector<int> colors_;
};

class Solver {
public:
  Solver(const Graph &graph);
  virtual ~Solver(){};
  virtual Solution solve() = 0;

protected:
  std::unordered_map<int, std::unordered_set<int>> edges;
  size_t n_node;
};

#endif
