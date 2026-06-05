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
    
    if (grid.empty()) {
        std::cerr << "Error: Could not detect Sudoku grid in the image.\n";
        return recognizedSudoku; 
    }
    
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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_sudoku_image>\n";
        return 1;
    }

    initRecognition(SVM_MODEL_PATH);
    std::string imagePath = argv[1];
    cv::Mat image = cv::imread(imagePath);

    if (image.empty()) {
        std::cerr << "Error: Failed to load image from path: " << imagePath << "\n";
        return 1;
    }

    std::cout << "Processing full pipeline for: " << imagePath << "\n";
    

        Sudoku result = pipeline(image);
    std::cout << "\n--- RECOGNIZED SUDOKU MATRIX ---\n";
    for (int r = 0; r < 9; r++) {
        if (r % 3 == 0 && r != 0)
            std::cout << "------+-------+------\n";
        for (int c = 0; c < 9; c++) {
            if (c % 3 == 0 && c != 0)
                std::cout << " |";
            std::cout << " " << result.values[r][c];
        }
        std::cout << "\n";
    }
    std::cout << "--------------------------------\n";
    return 0;
}