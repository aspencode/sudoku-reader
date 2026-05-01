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
    std::string input_dir = "../data";
    std::string output_dir = "../data_results";

    fs::create_directories(output_dir);

    int success = 0;
    int fail = 0;

    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (!entry.is_regular_file() || !isImageFile(entry.path()))
            continue;

        std::string path = entry.path().string();
        std::string filename = entry.path().stem().string();

        std::cout << "Processing: " << path << "\n";

        cv::Mat image = cv::imread(path);
        if (image.empty()) {
            std::cerr << "Failed to load image\n";
            fail++;
            continue;
        }

        cv::Mat processed;
        preprocessing(image, processed);

        cv::Mat grid = detectGrid(processed);

        if (grid.empty()) {
            std::cerr << "Grid detection failed\n";
            fail++;
            continue;
        }

        std::string out_path = output_dir + "/" + filename + "_grid.jpg";
        cv::imwrite(out_path, grid);

        success++;
    }

    std::cout << "\n=== SUMMARY ===\n";
    std::cout << "Success: " << success << "\n";
    std::cout << "Fail:    " << fail << "\n";

    return 0;
}
