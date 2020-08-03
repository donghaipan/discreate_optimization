#include <iostream>
#include "src/TSP_Problem.h"

int main(int argc, char **argv) {
    TSP_Problem problem;

    int N = 0;
    std::cin >> N;

    for (int i = 0; i < N; ++i) {
        double x, y;
        std::cin >> x;
        std::cin >> y;

        problem.add_city(x, y);
    }

    problem.best_sol = problem.gen_random_solution();
    problem.optimize(10000, 100 * N, 2000, true);

    // stdout
    std::cout.precision(3);
    std::cout << std::fixed << problem.best_sol.total_dist << " " << 0 << std::endl;
    for (auto &c : problem.best_sol.v) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}
