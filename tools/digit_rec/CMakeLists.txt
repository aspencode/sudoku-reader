find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(train_svm train_svm.cpp)
target_link_libraries(train_svm ${OpenCV_LIBS})

add_executable(test_digit test_digit.cpp)
target_link_libraries(test_digit ${OpenCV_LIBS})

add_executable(evaluate_svm evaluate_svm.cpp)
target_link_libraries(evaluate_svm  ${OpenCV_LIBS})