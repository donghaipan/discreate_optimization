//
// Created by xiongjie on 7/30/20.
//

#ifndef TSP_TSP_PROBLEM_H
#define TSP_TSP_PROBLEM_H

#include <unordered_map>
#include <vector>
#include <random>

struct City {
    double x = 0;
    double y = 0;
};

struct TSP_Solution {
    double total_dist = 0.0;
    std::vector<int> v;

    bool operator<(const TSP_Solution &other) const {
        return total_dist < other.total_dist;
    }
};

class TSP_Problem {
public:
    TSP_Solution best_sol;

    void add_city(double x, double y);

    double dist(int i, int j) const;

    double dist(const TSP_Solution &x) const;

    TSP_Solution gen_random_solution() const;

    TSP_Solution gen_greedy_solution() const;

    void propose_move(int i, int j, TSP_Solution &x) const;

    void gen_neighbor(TSP_Solution &x, TSP_Solution &y, std::mt19937 &generator) const;

    std::vector<double> gen_temperatures(int temperature_list_size, TSP_Solution &x) const;

    double solve(int num_iter, double temperature, TSP_Solution &x);

    void optimize(int num_rounds, int num_iter_per_round, int temperature_list_size, bool verbose);

private:
    int num_cities = 0;
    std::unordered_map<int, City> cities;
};

#endif //TSP_TSP_PROBLEM_H
