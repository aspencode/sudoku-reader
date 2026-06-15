#include "pipeline.hpp"
#include <iostream>
#include "preprocessing.hpp"
#include "grid.hpp"
#include "digits.hpp"
#include "recognition.hpp"

bool pipeline(const cv::Mat& inputImg, std::array<Cell, 81>& cellsArr, Sudoku& recognizedSudoku) {
    recognizedSudoku = {};
    cv::Mat processed;
    preprocessing(inputImg, processed);
    
    cv::Mat grid = detectGrid(processed);
    if (grid.empty()) {
        std::cerr << "Error: Could not detect Sudoku grid in the image.\n";
        return false;
    }
    
    splitGrid(grid, cellsArr);
    
    for (int i = 0; i < 81; i++) {
        cv::Mat binary;
        cleanupCell(cellsArr[i], binary);
        recognizeEmpty(cellsArr[i], binary);
        
        if (!isKnown(cellsArr[i])) {
            recognizeNumber(cellsArr[i]);
        }
        
        recognizedSudoku.values[i / 9][i % 9] = cellsArr[i].value;
    }
    
    return true;
}
