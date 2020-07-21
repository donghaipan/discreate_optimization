#include "color_solver.h"

Solver::Solver(const Graph &graph) {
  n_node = graph.get_num_node();

  edges.clear();
  for (const auto &edge : graph.get_edges()) {
    edges[edge.left_node].insert(edge.right_node);
    edges[edge.right_node].insert(edge.left_node);
  }
}

void Solution::update_solution(const Solution &sol) {
  if (sol.num_color() < n_color_) {
    std::cout << "got a new solution: " << n_color_ << " -> " << sol.num_color()
              << std::endl;
    update(sol.num_color(), sol.get_colorings());
  }
}

void Solution::update_solution(const std::vector<int> &colors) {
  if (!is_valid_) {
    int n_color = *std::max_element(colors.begin(), colors.end());
    update(n_color, colors);
    is_valid_ = true;
  } else {
    int n_color = *std::max_element(colors.begin(), colors.end());
    if (n_color < n_color_) {
      std::cout << "got a new solution: " << n_color_ << " -> " << n_color
                << std::endl;
      update(n_color, colors);
    } else {
      for (size_t i = 0; i < colors.size(); i++) {
        if (i) {
          std::cout << " ";
        }
        std::cout << colors[i];
      }
      std::cout << std::endl;
    }
  }
}

Solution::Solution(const char *filename) {
  std::ifstream input(filename);
  if (input.good()) {
    input >> n_color_ >> is_optimal_;
    std::string s;
    input >> s;

    std::stringstream ss(s);
    while (ss.good()) {
      std::string substr;
      getline(ss, substr, ' ');
      colors_.push_back(std::stoi(substr));
    }
  }
  is_valid_ = true;
  input.close();
}

std::ostream &operator<<(std::ostream &os, const Solution &sol) {
  os << sol.n_color_ << " " << sol.is_optimal_ << std::endl;

  for (size_t i = 0; i < sol.colors_.size(); ++i) {
    if (i) {
      os << " ";
    }
    os << sol.colors_[i];
  }
  return os;
}
