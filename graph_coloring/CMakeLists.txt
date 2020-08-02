cmake_minimum_required(VERSION 3.15)
project(coloring)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES src/iter_greedy.cpp
  src/color_solver.cpp
  src/tabu.cpp)

add_executable(coloring main.cpp ${SOURCES})
