cmake_minimum_required(VERSION 3.17)
project(aoc_2020)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/common)

add_library(aoc src/common/cli.cpp)

add_executable(day1 src/day1.cpp)
target_link_libraries(day1 aoc)

