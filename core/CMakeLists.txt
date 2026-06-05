add_library(sudoku_core
    src/preprocessing.cpp
    src/grid.cpp
    src/digits.cpp
    src/recognition.cpp
)

target_include_directories(sudoku_core PUBLIC include)

find_package(OpenCV REQUIRED)
target_link_libraries(sudoku_core ${OpenCV_LIBS})

target_compile_definitions(sudoku_core PRIVATE
    SVM_MODEL_PATH="${CMAKE_SOURCE_DIR}/core/models/svm_model_chi2.xml"
)