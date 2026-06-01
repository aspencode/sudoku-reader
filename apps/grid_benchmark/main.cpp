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


int main(int argc, char* argv[]) {

    std::string input_dir = "./data"; 
    std::string output_dir = "./data_results";


    if (argc > 1) input_dir = argv[1];
    if (argc > 2) output_dir = argv[2];

    std::cout << "Starting evaluation...\n";
    std::cout << "Input directory: " << input_dir << "\n";
    std::cout << "Output directory: " << output_dir << "\n\n";


    if (!fs::exists(input_dir)) {
        std::cerr << "Error: Input directory '" << input_dir << "' does not exist!\n";
        return 1;
    }

    if (!fs::is_directory(input_dir)) {
        std::cerr << "Error: '" << input_dir << "' is not a directory!\n";
        return 1;
    }

    fs::create_directories(output_dir);

    int success = 0;
    int fail = 0;

    try {
        for (const auto& entry : fs::directory_iterator(input_dir)) { 
            if (!entry.is_regular_file() || !isImageFile(entry.path()))
                continue;

            std::string path = entry.path().string();
            std::string filename = entry.path().stem().string();

            cv::Mat image = cv::imread(path);
            if (image.empty()) {
                std::cerr << "Failed to load image: " << filename << "\n";
                fail++;
                continue;
            }

            cv::Mat processed;
            preprocessing(image, processed);
            cv::Mat grid = detectGrid(processed);

            if (grid.empty()) {
                fail++;
                continue;
            }

            std::string out_path = output_dir + "/" + filename + "_grid.jpg";
            cv::imwrite(out_path, grid);
            success++;
        }
    } 
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error occurred: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\n=== EVALUATION SUMMARY ===\n";
    std::cout << "Successfully processed: " << success << "\n";
    std::cout << "Failed to detect:       " << fail << "\n";
    std::cout << "Accuracy:               " << (success + fail > 0 ? (success * 100.0 / (success + fail)) : 0.0) << "%\n";

    return 0;
}
