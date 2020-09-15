#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "src/cooling_scheduler.h"
#include "src/datatype.h"
#include "src/solver.h"
#include "src/tsp_move.h"

std::vector<tsp_sa::City> load_file(const char *filename) {
  std::ifstream input(filename);

  int n_city;
  input >> n_city;

  std::vector<tsp_sa::City> cities;
  cities.reserve(n_city);

  for (int i = 0; i < n_city; i++) {
    double x, y;
    input >> x >> y;
    cities.emplace_back(x, y);
  }
  input.close();
  return cities;
}

std::vector<size_t> load_sol(const char *filename) {
  std::ifstream input(filename);
  double best_val;
  int is_optimal;
  input >> best_val >> is_optimal;
  std::vector<size_t> idx;
  int n;
  while (input >> n) {
    assert(n >= 0);
    idx.push_back(n);
  }
  input.close();
  return idx;
}

auto load_parameters(const char *filename) {
  int max_outer_iter;
  int max_inner_iter;
  double init_temp;
  double alpha;
  std::ifstream input(filename);
  input >> max_inner_iter >> max_outer_iter >> init_temp >> alpha;
  input.close();
  return std::make_tuple(max_inner_iter, max_outer_iter, init_temp, alpha);
}

int main(int argc, char *argv[]) {

  auto cities = load_file(argv[1]);
  auto [max_inner_iter, max_outer_iter, init_temp, alpha] =
      load_parameters(argv[2]);

  std::cerr << "max_inner_iter: " << max_inner_iter << std::endl;
  std::cerr << "max_outer_iter: " << max_outer_iter << std::endl;
  std::cerr << "init_temp: " << init_temp << std::endl;
  std::cerr << "alpha: " << alpha << std::endl;

  auto city_map = tsp_sa::CityMap(cities);

  auto config = tsp_sa::SolverConfig(max_inner_iter, max_outer_iter);

  auto solver = tsp_sa::Solver(config);
  auto cooler = tsp_sa::GeometricCooling(alpha, init_temp);

  auto two_opt = tsp_sa::TwoOPTMove();
  solver.register_tsp_move(&two_opt);

  solver.register_cooling_schedular(&cooler);

  auto sol = solver.solve(city_map);

  auto best_val = city_map.evaluate(sol);

  std::cout << best_val << " " << 0 << std::endl;
  for (size_t i = 0; i < sol.size(); i++) {
    if (i) {
      std::cout << " ";
    }
    std::cout << sol[i];
  }
  std::cout << std::endl;
  return 0;
}
