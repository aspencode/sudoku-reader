add_executable(cli main.cpp)

target_link_libraries(cli sudoku_core ${OpenCV_LIBS})
