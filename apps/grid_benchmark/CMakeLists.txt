add_executable(grid_benchmark main.cpp)

target_link_libraries(grid_benchmark sudoku_core ${OpenCV_LIBS})
