#include <fstream>
#include <iostream>

#include "src/iter_greedy.h"
#include "src/tabu.h"

void save_results(const char *filename, const Solution &sol) {
  // try load existing results
  std::ifstream input(filename);
  bool overwrite = false;
  if (input.good()) {
    try {
      int n_color;
      input >> n_color;
      if (sol.num_color() < n_color) {
        overwrite = true;
      } else {
        std::cout << "solution is no better than the cached one." << std::endl;
      }
    } catch (std::exception &e) {
      overwrite = true;
    }
    input.close();
  } else {
    overwrite = true;
  }

  if (overwrite) {
    std::cout << "Overwrite existing solution..." << std::endl;
    std::ofstream output(filename);
    output << sol << std::endl;
    output.close();
  }
}

int main(int argc, char *argv[]) {
  auto graph = Graph(argv[1]);
  TabuSearch solver(graph);
  auto sol = solver.solve();
  graph.check_coloring(sol.get_colorings());
  std::cout << sol << std::endl;
  save_results(argv[2], sol);
  return 0;
}
