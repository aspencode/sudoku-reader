add_executable(cli main.cpp)

target_link_libraries(cli sudoku_core ${OpenCV_LIBS})

target_compile_definitions(cli PRIVATE
    SVM_MODEL_PATH="${CMAKE_SOURCE_DIR}/core/models/svm_model_chi2.xml"
)