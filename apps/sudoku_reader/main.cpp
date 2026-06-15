#include <iostream>
#include <fstream>
#include <filesystem>
#include "types.hpp"
#include "recognition.hpp"
#include "pipeline.hpp"

namespace fs = std::filesystem;

void printSudoku(const Sudoku& s) {
    for (int r = 0; r < 9; r++) {
        if (r % 3 == 0 && r != 0)
            std::cout << "-------+-------+------\n";
        for (int c = 0; c < 9; c++) {
            if (c % 3 == 0 && c != 0)
                std::cout << " |";
            std::cout << " " << s.values[r][c];
        }
        std::cout << "\n";
    }
}

bool saveDat(const std::string& path, const Sudoku& s) {
    std::ofstream f(path);
    if (!f.is_open()) return false;
    
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            f << s.values[r][c] << (c == 8 ? "" : " ");
        }
        f << "\n";
    }
    return true;
}

bool processSingleImage(const std::string& imgPath) {
    cv::Mat image = cv::imread(imgPath);
    if (image.empty()) {
        std::cerr << "Error: Failed to load image: " << imgPath << "\n";
        return false;
    }

    std::cout << "Processing: " << imgPath << "\n";
    std::array<Cell, 81> cellsArr;
    Sudoku result = {};
    
    bool success = pipeline(image, cellsArr, result);
    if (!success) {
        std::cerr << "Error: Pipeline failed for " << imgPath << "\n";
        return false;
    }

    std::cout << "\n--- RECOGNIZED SUDOKU MATRIX ---\n";
    printSudoku(result);
    std::cout << "--------------------------------\n";

    fs::path p(imgPath);
    std::string outPath = p.parent_path().string() + "/" + p.stem().string() + ".dat";
    
    if (saveDat(outPath, result)) {
        std::cout << "Saved results to: " << outPath << "\n";
    } else {
        std::cerr << "Error: Could not save file " << outPath << "\n";
        return false;
    }

    return true;
}

void runBatch(const std::string& dir) {
    int totalImages = 0;
    int successfulImages = 0;

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext != ".jpg" && ext != ".jpeg" && ext != ".png") continue;

        totalImages++;
        std::cout << "\n=== " << entry.path().filename().string() << " ===\n";
        
        if (processSingleImage(entry.path().string())) {
            successfulImages++;
        }
    }

    std::cout << "\n====== BATCH SUMMARY ======\n";
    std::cout << "Processed images: " << successfulImages << "/" << totalImages << "\n";
    std::cout << "===========================\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  " << argv[0] << " <image.jpg>          -- single image\n";
        std::cout << "  " << argv[0] << " --batch <directory>  -- batch generation\n";
        return 1;
    }

    initRecognition(SVM_MODEL_PATH);

    if (std::string(argv[1]) == "--batch") {
        if (argc < 3) {
            std::cerr << "Error: No directory specified\n";
            return 1;
        }
        runBatch(argv[2]);
        return 0;
    }

    bool success = processSingleImage(argv[1]);
    return success ? 0 : 1;
}
