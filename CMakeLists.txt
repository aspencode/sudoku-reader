cmake_minimum_required(VERSION 3.16)
project(sudoku_reader)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(core)
add_subdirectory(apps/run_tests)
add_subdirectory(apps/sudoku_reader)
add_subdirectory(apps/grid_benchmark)
add_subdirectory(tools/digit_rec)
