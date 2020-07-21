#ifndef COLORING_TABU_H
#define COLORING_TABU_H

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "color_solver.h"
#include "graph.h"
#include "iter_greedy.h"

struct NodeMove {
  int node_;            // index of the node
  int color_;           // to color
  int violation_value_; // total violation value after move
  NodeMove(const int node, const int color)
      : node_(node), color_(color), violation_value_(-1) {}
  NodeMove(const int node, const int color, const int violation_value)
      : node_(node), color_(color), violation_value_(violation_value) {}
};

class Violation {
public:
  Violation(const std::vector<int> node_violation, const int total_violation)
      : total_violation_(total_violation), node_violation_(node_violation) {}

  void decrease(size_t i, size_t j) {
    node_violation_[i]--;
    node_violation_[j]--;
    total_violation_ -= 2;
  }
  void increase(size_t i, size_t j) {
    node_violation_[i]++;
    node_violation_[j]++;
    total_violation_ += 2;
  }

  int get_total_violation() const { return total_violation_; }
  int get_node_violation(const size_t idx) const {
    return node_violation_[idx];
  }

private:
  int total_violation_;
  std::vector<int> node_violation_;
};

class TabuList {
public:
  TabuList(const size_t tabu_size) : tabu_size_(tabu_size) {}
  bool has_tabu(const NodeMove &node) const {
    const auto idx = node.node_;
    const auto color = node.color_;
    return tabu_hash.find(idx) != tabu_hash.end() &&
           tabu_hash.at(idx).find(color) != tabu_hash.at(idx).end();
  }

  void push_tabu(const NodeMove &node) {
    if (tabu_list.size() == tabu_size_) {
      auto p = tabu_list.front();
      tabu_list.pop();
      tabu_hash[p.first].erase(p.second);
    }
    tabu_hash[node.node_].insert(node.color_);
    tabu_list.push(std::make_pair(node.node_, node.color_));
  };

private:
  const size_t tabu_size_;
  std::queue<std::pair<int, int>> tabu_list;
  std::unordered_map<int, std::unordered_set<int>> tabu_hash;
};

class TabuSearch : public IterGreedy {
public:
  TabuSearch(const Graph &graph) : IterGreedy(graph) {}
  virtual Solution solve();

private:
  Solution warm_start();
  std::vector<int> randomize_init_colors(const std::vector<int> &colors,
                                         const int num_color) const;
  Solution search(const std::vector<int> &colors, const int num_color) const;
  Violation calculate_violation(const std::vector<int> &colors) const;
  int find_next_node(const Violation &violation,
                     const TabuList &tabu_list) const;
  int evaluate_violation(const NodeMove &node_move, const Violation &violation,
                         const std::vector<int> &colors) const;
  void apply_move(const NodeMove &node_move, Violation &violation,
                  std::vector<int> &colors) const;
};
#endif
