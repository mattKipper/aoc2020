cmake_minimum_required(VERSION 3.17)
project(aoc_2020)

set(CMAKE_CXX_STANDARD 17)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP8>)

include_directories(src/common)

add_library(aoc src/common/cli.cpp)

foreach(n RANGE 1 12)
    add_executable("day${n}" "src/day${n}.cpp")
    target_link_libraries("day${n}" aoc)
endforeach()

