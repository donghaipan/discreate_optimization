#ifndef COLORING_ITER_GREEDY_H
#define COLORING_ITER_GREEDY_H

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "color_solver.h"
#include "graph.h"

class IterGreedy {
public:
  Solution solve(const Graph &graph);

private:
  Solution greedy(const std::vector<size_t> &idx) const;
  std::vector<size_t> permute(const Solution &sol) const;
  std::unordered_map<int, std::unordered_set<int>> edges;
  size_t n_node;
};

#endif
