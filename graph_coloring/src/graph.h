#ifndef COLORING_GRAPH_H
#define COLORING_GRAPH_H

#pragma once

#include <fstream>
#include <iostream>
#include <vector>

struct Edge {
  int left_node;
  int right_node;
  Edge(const int left, const int right) : left_node(left), right_node(right) {}
};

class Graph {
 public:
  Graph(const char *filename) {
    std::ifstream input(filename);

    input >> n_node >> n_edge;

    edges.clear();
    edges.reserve(n_edge);

    for (size_t i = 0; i < n_edge; ++i) {
      int left, right;
      input >> left >> right;
      edges.emplace_back(left, right);
    }

    input.close();
  }
  
  void check_coloring(const std::vector<int> &colors) const {
    if (colors.size() != get_num_node()) {
      std::cout << "Size does not match: number of graph nodes ("
                << get_num_node() << ") != size of color vector ("
                << colors.size() << ")" << std::endl;
    }

    for (const auto &edge : get_edges()) {
      if (colors[edge.left_node] == colors[edge.right_node]) {
        std::cout << "conflict color at " << edge.left_node << " and "
                  << edge.right_node << std::endl;
      }
    }
  }

  size_t get_num_node() const { return n_node; }
  size_t get_num_edge() const { return n_edge; }
  const std::vector<Edge> &get_edges() const { return edges; }

 private:
  size_t n_node;
  size_t n_edge;
  std::vector<Edge> edges;
};

#endif