#ifndef COLORING_ITER_GREEDY_H
#define COLORING_ITER_GREEDY_H

#pragma once

#include <vector>

#include "color_solver.h"
#include "graph.h"

class IterGreedy : public Solver {
public:
  IterGreedy(const Graph &graph) : Solver(graph) {}
  virtual Solution solve();

protected:
  Solution greedy(const std::vector<size_t> &idx) const;

private:
  std::vector<size_t> permute(const Solution &sol) const;
};

#endif
