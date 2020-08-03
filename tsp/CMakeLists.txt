cmake_minimum_required(VERSION 3.3)
project(TSP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
add_executable(TSP src/TSP_Problem.cpp solver.cpp)