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
    std::vector<cv::String> imagePaths;
    cv::glob("../data/*.jpg", imagePaths);
    cv::glob("../data/*.png", imagePaths);

    if (imagePaths.empty()) {
        std::cerr << "Brak zdjec w folderze data\n";
        return -1;
    }

    for (const auto& path : imagePaths) {
        std::cout << "\n=== " << path << " ===\n";
        cv::Mat image = cv::imread(path);
        if (image.empty()) {
            std::cerr << "Nie mozna wczytac: " << path << "\n";
            continue;
        }
        std::cout << "Size: " << image.rows << "x" << image.cols << "\n";

        Sudoku s = pipeline(image);

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                std::cout << s.values[i][j] << "\t";
            }
            std::cout << "\n";
        }
    }

    return 0;
}
