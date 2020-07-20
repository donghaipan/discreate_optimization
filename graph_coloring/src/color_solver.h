#ifndef COLORING_SOLVER_H
#define COLORING_SOLVER_H

#include <algorithm>
#include <iostream>
#include <vector>

class Solution {
public:
  Solution(const int n_color, const std::vector<int> &colors,
           const bool is_valid)
      : n_color_(n_color), colors_(colors), is_valid_(is_valid) {}

  void update_solution(const Solution &sol) {
    if (sol.num_color() < n_color_) {
      std::cout << "got a new solution: " << n_color_ << " -> "
                << sol.num_color() << std::endl;
      update(sol.num_color(), sol.get_colorings());
    }
  }

  void update_solution(const std::vector<int> &colors) {
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

  void set_optimal() { is_optimal_ = true; }

  bool is_valid() const { return is_valid_; }
  bool is_optimal() const { return is_optimal_; }
  int num_color() const { return n_color_; }
  const std::vector<int> &get_colorings() const { return colors_; }

  friend std::ostream &operator<<(std::ostream &os, const Solution &sol) {
    os << sol.n_color_ << " " << sol.is_optimal_ << std::endl;

    for (size_t i = 0; i < sol.colors_.size(); ++i) {
      if (i) {
        os << " ";
      }
      os << sol.colors_[i];
    }
    return os;
  }

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

#endif
