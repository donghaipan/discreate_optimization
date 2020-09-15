cmake_minimum_required(VERSION 3.15)
project(tsp_sa)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cc")

add_executable(tsp_sa main.cc ${SOURCES})
