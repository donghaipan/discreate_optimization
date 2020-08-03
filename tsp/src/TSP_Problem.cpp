//
// Created by xiongjie on 7/30/20.
//

#include <cmath>
#include <algorithm>
#include <iostream>
#include "TSP_Problem.h"

void TSP_Problem::add_city(double x, double y) {
    City c;
    c.x = x;
    c.y = y;
    cities[num_cities] = c;
    num_cities += 1;
}

double TSP_Problem::dist(int i, int j) const {
    auto &ci = cities.at(i);
    auto &cj = cities.at(j);
    return std::sqrt((ci.x - cj.x) * (ci.x - cj.x) + (ci.y - cj.y) * (ci.y - cj.y));
}

double TSP_Problem::dist(const TSP_Solution &x) const {
    double d = 0.0;
    for (int i = 0; i < num_cities; ++i) {
        int j = (i + 1) % num_cities;
        d += dist(x.v[i], x.v[j]);
    }
    return d;
}

TSP_Solution TSP_Problem::gen_random_solution() const {
    TSP_Solution sol;
    for (int i = 0; i < num_cities; ++i) {
        sol.v.push_back(i);
    }
    // fix first city
    std::shuffle(sol.v.begin() + 1, sol.v.end(), std::mt19937(std::random_device()()));
    sol.total_dist = dist(sol);

    return sol;
}

TSP_Solution TSP_Problem::gen_greedy_solution() const {
    std::unordered_map<int, bool> visited;
    for (int i = 0; i < num_cities; ++i) {
        visited[i] = false;
    }

    TSP_Solution sol;
    sol.v.push_back(0);
    visited[0] = true;

    while (sol.v.size() < num_cities) {
        int min_idx = -1;
        double min_dist = std::numeric_limits<double>::max();
        for (int i = 0; i < num_cities; ++i) {
            if (!visited[i]) {
                double d = dist(sol.v.back(), i);
                if (min_dist > d) {
                    min_idx = i;
                    min_dist = d;
                }
            }
        }
        sol.v.push_back(min_idx);
        visited[min_idx] = true;
    }
    sol.total_dist = dist(sol);

    return sol;
}

void TSP_Problem::propose_move(int i, int j, TSP_Solution &x) const {
    int choice = 0;
    int i_left = i - 1;
    int i_right = i + 1;
    int j_left = j - 1;
    int j_right = (j + 1) % num_cities;

    // cache some distances
    // old
    double d_il_i = dist(x.v[i_left], x.v[i]);
    double d_j_jr = dist(x.v[j], x.v[j_right]);
    double d_il_j = dist(x.v[i_left], x.v[j]);
    double d_i_jr = dist(x.v[i], x.v[j_right]);
    // inverse
    double min_dist = x.total_dist - (d_il_i + d_j_jr) + (d_il_j + d_i_jr);
    if (j - i > 1) {
        double d_i_ir = dist(x.v[i], x.v[i_right]);
        double d_jl_j = dist(x.v[j_left], x.v[j]);
        double d_j_ir = dist(x.v[j], x.v[i_right]);
        double d_jl_i = dist(x.v[j_left], x.v[i]);
        double d_i_j = dist(x.v[i], x.v[j]);
        double d_il_ir = dist(x.v[i_left], x.v[i_right]);
        double d_jl_jr = dist(x.v[j_left], x.v[j_right]);
        // insert 1
        double x_insert_i_dist = x.total_dist - (d_il_i + d_jl_j + d_j_jr) + (d_il_j + d_i_j + d_jl_jr);
        if (x_insert_i_dist < min_dist) {
            min_dist = x_insert_i_dist;
            choice = 1;
        }
        // insert 2
        double x_insert_j_dist = x.total_dist - (d_il_i + d_i_ir + d_j_jr) + (d_il_ir + d_i_j + d_i_jr);
        if (x_insert_j_dist < min_dist) {
            min_dist = x_insert_j_dist;
            choice = 2;
        }
        // swap
        double x_swap_dist = x.total_dist - (d_il_i + d_i_ir + d_jl_j + d_j_jr) + (d_il_j + d_j_ir + d_jl_i + d_i_jr);
        if (x_swap_dist < min_dist) {
            min_dist = x_swap_dist;
            choice = 3;
        }
    }

    x.total_dist = min_dist;
    if (choice == 0) {
        for (int inc = 0; inc < (j - i + 1) / 2; ++inc) {
            int tmp = x.v[i + inc];
            x.v[i + inc] = x.v[j - inc];
            x.v[j - inc] = tmp;
        }
    } else if (choice == 1) {
        int tmp = x.v[j];
        for (int idx = j; idx > i; --idx) {
            x.v[idx] = x.v[idx - 1];
        }
        x.v[i] = tmp;
    } else if (choice == 2) {
        int tmp = x.v[i];
        for (int idx = i; idx < j; ++idx) {
            x.v[idx] = x.v[idx + 1];
        }
        x.v[j] = tmp;
    } else {
        int tmp = x.v[i];
        x.v[i] = x.v[j];
        x.v[j] = tmp;
    }
}

void TSP_Problem::gen_neighbor(TSP_Solution &x, TSP_Solution &y, std::mt19937 &generator) const {
    // generate 2 random locations, i < j
    auto g = std::uniform_int_distribution<int>(1, num_cities - 1);
    int a = 0, b = 0, i, j;
    while (a == b) {
        a = g(generator);
        b = g(generator);
    }
    i = std::min(a, b);
    j = std::max(a, b);
    y = x;
    propose_move(i, j, y);
}

double TSP_Problem::solve(int num_iter, double temperature, TSP_Solution &x) {
    TSP_Solution y = x;
    double num_new_temperature = 0.0;
    double new_temperature = 0.0;
    std::mt19937 generator1 = std::mt19937(std::random_device()());
    std::mt19937 generator2 = std::mt19937(std::random_device()());
    std::uniform_real_distribution<double> distribution(0, 1);

    int iter = 0;
    while (iter < num_iter) {
        gen_neighbor(x, y, generator1);
        if (y.total_dist <= x.total_dist) {
            x = y;
        } else {
            double r = distribution(generator2), p = std::exp((x.total_dist - y.total_dist) / temperature);
            if (r < p) {
                new_temperature += (x.total_dist - y.total_dist) / std::log(r);
                num_new_temperature += 1;
                x = y;
            }
        }

        if (x.total_dist < best_sol.total_dist) {
            best_sol = x;
        }

        ++iter;
    }

    return new_temperature / std::max(1.0, num_new_temperature);
}

std::vector<double> TSP_Problem::gen_temperatures(int temperature_list_size, TSP_Solution &x) const {
    std::vector<double> temperatures;
    TSP_Solution y;
    std::mt19937 generator = std::mt19937(std::random_device()());

    for (int iter = 0; iter < temperature_list_size; ++iter) {
        gen_neighbor(x, y, generator);
        if (y.total_dist < x.total_dist) {
            x = y;
        } else {
            double t = -std::abs(x.total_dist - y.total_dist) / std::log(1e-2);
            temperatures.push_back(t);
        }
    }
    std::make_heap(temperatures.begin(), temperatures.end());
    return temperatures;
}

void TSP_Problem::optimize(int num_rounds, int num_iter_per_round, int temperature_list_size, bool verbose) {
    auto x = best_sol;
    auto temperatures = gen_temperatures(temperature_list_size, x);

    x = best_sol;
    if (verbose) {
        std::cout << "# num_round # temperature # global_best_solution" << std::endl;
    }
    for (int i = 0; i < num_rounds; ++i) {
        double old_temperature = temperatures.front();
        double new_temperature = solve(num_iter_per_round, old_temperature, x);
        double iteration_best_dist = std::numeric_limits<double>::max();
        if (new_temperature > 1e-3) {
            std::pop_heap(temperatures.begin(), temperatures.end());
            temperatures.pop_back();
            temperatures.push_back(new_temperature);
            std::push_heap(temperatures.begin(), temperatures.end());
        }

        if (i % 100 == 0) {
            if (verbose) {
                std::cout.precision(3);
                std::cout << std::fixed << i << " " << old_temperature << " " << best_sol.total_dist << std::endl;
            }
        }
    }
}
