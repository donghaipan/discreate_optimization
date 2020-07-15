#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

// Item
struct Item {
  int weight;
  int value;
  size_t index;
  Item(const int weight_, const int value_, const size_t index_)
      : weight(weight_), value(value_), index(index_) {}

  double value_density() const { return static_cast<double>(value) / weight; }
};

class Solution {
public:
  void solve(const std::vector<int> &weights, const std::vector<int> &values,
             const int K) {
    const size_t n = weights.size();

    std::vector<Item> items;
    items.reserve(n);
    for (size_t i = 0; i < n; i++) {
      items.emplace_back(weights[i], values[i], i);
    }

    std::sort(items.begin(), items.end(),
              [&](const Item &lhs, const Item &rhs) {
                return lhs.value_density() > rhs.value_density();
              });

    std::stack<TreeNode> tree;
    tree.push(TreeNode(K, 0, 0, std::vector<bool>(n)));

    int best_val = -1;
    std::vector<bool> best_choice(n, false);

    // branch and bound
    while (!tree.empty()) {
      auto node = tree.top();
      tree.pop();

      // invalid node
      if (node.capacity < 0) {
        continue;
      }

      auto est_val = static_cast<double>(node.val) +
                     evaluate(node.capacity, node.start_idx, items);

      // prune
      if (est_val < static_cast<double>(best_val)) {
        continue;
      }

      const auto idx = node.start_idx;
      if (idx >= n) {
        // leaf
        if (node.val > best_val) {
          best_val = node.val;
          best_choice = node.choice;
        }
      } else {
        std::vector<bool> choice(node.choice);
        tree.push(TreeNode(node.capacity, node.val, idx + 1, choice));
        choice[idx] = true;
        tree.push(TreeNode(node.capacity - items[idx].weight,
                           node.val + items[idx].value, idx + 1, choice));
      }
    }

    std::cout << best_val << " 1" << std::endl;
    std::vector<bool> original_choice(n);

    for (size_t i = 0; i < n; i++) {
      original_choice[items[i].index] = best_choice[i];
    }

    for (size_t i = 0; i < n; i++) {
      if (i > 0) {
        std::cout << " ";
      }
      std::cout << original_choice[i];
    }
    std::cout << std::endl;
  }

private:
  struct TreeNode {
    int capacity;
    int val;
    size_t start_idx;
    std::vector<bool> choice;
    TreeNode(const int capacity_, const int val_, const size_t start_idx_,
             const std::vector<bool> &choice_)
        : capacity(capacity_), val(val_), start_idx(start_idx_),
          choice(choice_) {}
  };

  // 0 -> 1 relaxation evaluation
  double evaluate(int capacity, const size_t start_idx,
                  const std::vector<Item> &items) const {
    double val = 0.;
    for (size_t i = start_idx; i < items.size(); i++) {
      const auto &item = items[i];
      if (item.weight <= capacity) {
        val += static_cast<double>(item.value);
        capacity -= item.weight;
      } else {
        val += item.value_density() * static_cast<double>(capacity);
        break;
      }
    }
    return val;
  }
};

int main(int argc, char *argv[]) {
  // load file
  std::ifstream input;
  input.open(argv[1]);
  int n, K;
  input >> n >> K;
  std::vector<int> values(n);
  std::vector<int> weights(n);
  for (int i = 0; i < n; ++i) {
    input >> values[i] >> weights[i];
  }
  input.close();

  Solution().solve(weights, values, K);
  return 0;
}
