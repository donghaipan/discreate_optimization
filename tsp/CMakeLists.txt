cmake_minimum_required(VERSION 3.15)
project(tsp)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES "src/*.cpp")

add_executable(tsp main.cpp ${SOURCES})
