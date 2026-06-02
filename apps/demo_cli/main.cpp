#include <iostream>
#include <filesystem>
#include "types.hpp"
#include "preprocessing.hpp"
#include "grid.hpp"
#include "digits.hpp"
#include "recognition.hpp"

namespace fs = std::filesystem;

bool isImageFile(const fs::path& path) {
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext == ".jpg" || ext == ".jpeg" || ext == ".png");
}

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
    cv::glob("C:/Users/micha/OneDrive/Pulpit/doku/sudoku-reader/build/data/*.jpg", imagePaths);
    

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
            if (i % 3 == 0 && i != 0)
                std::cout << "------+-------+------\n";
            for (int j = 0; j < 9; j++) {
                if (j % 3 == 0 && j != 0)
                    std::cout << " |";
                std::cout << " " << s.values[i][j];
            }
            std::cout << "\n";
        }
    }
    return 0;
}