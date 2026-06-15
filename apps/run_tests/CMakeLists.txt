add_executable(run_tests main.cpp)

target_link_libraries(run_tests sudoku_core ${OpenCV_LIBS})

target_compile_definitions(run_tests PRIVATE
    SVM_MODEL_PATH="${CMAKE_SOURCE_DIR}/core/models/svm_model_chi2.xml"
)
