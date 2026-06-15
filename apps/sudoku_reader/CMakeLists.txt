add_executable(sudoku_reader main.cpp)

target_link_libraries(sudoku_reader sudoku_core ${OpenCV_LIBS})

target_compile_definitions(sudoku_reader PRIVATE
    SVM_MODEL_PATH="${CMAKE_SOURCE_DIR}/core/models/svm_model_chi2.xml"
)
