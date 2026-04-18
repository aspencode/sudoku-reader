add_library(sudoku_core
    src/preprocessing.cpp
    src/grid.cpp
    src/digits.cpp
    src/recognition.cpp
)

target_include_directories(sudoku_core PUBLIC include)

find_package(OpenCV REQUIRED)
target_link_libraries(sudoku_core ${OpenCV_LIBS})
