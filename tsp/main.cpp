#include <fstream>
#include <iostream>

#include "src/datatype.h"
#include "src/k_opt.h"

std::vector<tsp::Point> load_file(const char *filename) {
  std::ifstream input(filename);

  int n_city;
  input >> n_city;

  std::vector<tsp::Point> cities;
  cities.reserve(n_city);

  for (int i = 0; i < n_city; i++) {
    double x, y;
    input >> x >> y;
    cities.emplace_back(x, y);
  }
  input.close();
  return cities;
}

int main(int argc, char *argv[]) {
  auto cities = load_file(argv[1]);
  auto solver = tsp::KOpt(cities);
  auto sol = solver.solve();
  std::cout << sol << std::endl;
  return 0;
}
