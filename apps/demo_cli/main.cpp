#include <iostream>
#include "types.hpp"
#include "preprocessing.hpp"
#include "grid.hpp"
#include "digits.hpp"
#include "recognition.hpp"

Sudoku pipeline(const cv::Mat& inputImg) {
    Sudoku recognizedSudoku;
    std::array<Cell, 81> cellsArr; 
    cv::Mat processed;

    preprocessing(inputImg, processed);
    cv::Mat grid = detectGrid(processed);
    
    splitGrid(grid, cellsArr);

    for (int i = 0; i < 81; i++) {
        cleanupCell(cellsArr[i]);
        recognizeEmpty(cellsArr[i]);
        
        if (!isKnown(cellsArr[i])) {
            recognizeNumber(cellsArr[i]);
        }
        
        recognizedSudoku.values[i / 9][i % 9] = cellsArr[i].value;
    }

    return recognizedSudoku;
}

int main() {
    initRecognition(SVM_MODEL_PATH);
    cv::Mat image = cv::imread("../data/test.jpg");

    if (image.empty()) {
        std::cerr << "Error: could not load image. Add an image in ../data/test.jpg \n";
        return -1;
    }
    else {
        std::cout << "Image loaded successfully\n";
        std::cout << "Size: " << image.rows << "x" << image.cols << "\n";
        cv::imshow("cell image", image);
        Sudoku s;
        s=pipeline(image);
            for (int i=0;i<9;i++){
            for (int j=0;j<9;j++){
                std::cout<< s.values[i][j]<<"\t";
            }    
                std::cout<<"\n";
            }
    }



    return 0;
}
