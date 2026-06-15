#include <iostream>
#include <fstream>
#include <filesystem>
#include "types.hpp"
#include "preprocessing.hpp"
#include "grid.hpp"
#include "digits.hpp"
#include "recognition.hpp"
#include "pipeline.hpp"

namespace fs = std::filesystem;
const std::string WRONG_CELLS_DIR = "./tmp_wrong_cells";

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

bool loadDat(const std::string& path, Sudoku& out) {
    std::ifstream f(path);
    if (!f.is_open()) return false;

    std::string line;
    int validRowsCount = 0;

    while (std::getline(f, line) && validRowsCount < 9) {
        std::vector<int> rowDigits;
        bool hasInvalidChar = false;

        for (char c : line) {
            if (c == '\r') {
                continue;
            }

            if (std::isdigit(static_cast<unsigned char>(c))) {
                rowDigits.push_back(c - '0');
            } 
            else if (c != ' ' && c != ',') {
                hasInvalidChar = true;
                break;
            }
        }

        if (!hasInvalidChar && rowDigits.size() == 9) {
            for (int c = 0; c < 9; c++) {
                out.values[validRowsCount][c] = rowDigits[c];
            }
            validRowsCount++;
        }
    }

    return validRowsCount == 9;
}

struct CompareStats {
    int correctEmpty = 0;
    int wrongEmpty = 0;
    int missedEmpty = 0;
    int correctDigit = 0;
    int wrongDigit = 0;
};

CompareStats compareResults(const Sudoku& result, const Sudoku& expected) {
    CompareStats s;
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            int res = result.values[r][c];
            int exp = expected.values[r][c];
            if (exp == 0) {
                if (res == 0) s.correctEmpty++;
                else          s.wrongEmpty++;
            }
            else {
                if (res == 0)        s.missedEmpty++;
                else if (res == exp) s.correctDigit++;
                else                 s.wrongDigit++;
            }
        }
    }
    return s;
}

void printStats(const CompareStats& s) {
    int totalEmpty = s.correctEmpty + s.wrongEmpty;
    int totalDigits = s.correctDigit + s.wrongDigit + s.missedEmpty;
    int correct = s.correctEmpty + s.correctDigit;
    std::cout << "Empty cells:  " << s.correctEmpty << "/" << totalEmpty
        << " correct, " << s.wrongEmpty << " false positives"
        << ", " << s.missedEmpty << " missed\n";
    std::cout << "Digits:       " << s.correctDigit << "/" << totalDigits
        << " correct, " << s.wrongDigit << " wrong\n";
    std::cout << "Total:        " << correct << "/81"
        << " (" << (correct * 100 / 81) << "%)\n";
}

void saveWrongCells(const std::array<Cell, 81>& cells,
    const Sudoku& result,
    const Sudoku& expected,
    const std::string& imgName) {
    fs::create_directories(WRONG_CELLS_DIR);
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            int res = result.values[r][c];
            int exp = expected.values[r][c];
            if (res == exp) continue;
            int idx = r * 9 + c;
            if (cells[idx].image.empty()) continue;
            std::string base = WRONG_CELLS_DIR + "/" + imgName
                + "_r" + std::to_string(r)
                + "_c" + std::to_string(c)
                + "_exp" + std::to_string(exp)
                + "_got" + std::to_string(res);
            cv::imwrite(base + "_gray.png", cells[idx].image);
        }
    }
}

void runBatch(const std::string& dir) {
    fs::remove_all(WRONG_CELLS_DIR);
    int totalCells = 0, correctCells = 0;
    int totalImages = 0, perfectImages = 0;
    int totalCorrectEmpty = 0, totalWrongEmpty = 0, totalMissedEmpty = 0;
    int totalCorrectDigit = 0, totalWrongDigit = 0;

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext != ".jpg" && ext != ".jpeg" && ext != ".png") continue;

        std::string imgPath = entry.path().string();
        std::string datPath = entry.path().parent_path().string()
            + "/" + entry.path().stem().string() + ".dat";

        Sudoku expected = {};
        bool hasDat = loadDat(datPath, expected);

        cv::Mat image = cv::imread(imgPath);
        if (image.empty()) {
            std::cerr << "Failed to load: " << imgPath << "\n";
            continue;
        }

        std::cout << "\n=== " << entry.path().filename().string() << " ===\n";
        std::array<Cell, 81> cellsArr;
        Sudoku result = {};
        bool success = pipeline(image, cellsArr, result);
        printSudoku(result);

        if (hasDat) {
            CompareStats s = compareResults(result, expected);
            printStats(s);
            
            if (!success) {
                totalCells += 81;
                totalImages++;
                totalWrongEmpty += s.wrongEmpty;
                totalMissedEmpty += s.missedEmpty;
                totalWrongDigit += s.wrongDigit;
                continue; 
            }

            saveWrongCells(cellsArr, result, expected, entry.path().stem().string());
            int correct = s.correctEmpty + s.correctDigit;
            totalCells += 81;
            correctCells += correct;
            totalImages++;
            if (correct == 81) perfectImages++;
            totalCorrectEmpty += s.correctEmpty;
            totalWrongEmpty += s.wrongEmpty;
            totalMissedEmpty += s.missedEmpty;
            totalCorrectDigit += s.correctDigit;
            totalWrongDigit += s.wrongDigit;
        }
        else {
            std::cout << "(Missing .dat file - skipping comparison)\n";
        }
    }

    if (totalImages > 0) {
        int totalEmpty = totalCorrectEmpty + totalWrongEmpty;
        int totalDigits = totalCorrectDigit + totalWrongDigit + totalMissedEmpty;
        std::cout << "\n====== BATCH SUMMARY ======\n";
        std::cout << "Images:        " << totalImages << "\n";
        std::cout << "Perfect:       " << perfectImages << "/" << totalImages << "\n";
        std::cout << "Empty cells:   " << totalCorrectEmpty << "/" << totalEmpty
            << " correct, " << totalWrongEmpty << " false positives"
            << ", " << totalMissedEmpty << " missed\n";
        std::cout << "Digits:        " << totalCorrectDigit << "/" << totalDigits
            << " correct, " << totalWrongDigit << " wrong\n";
        std::cout << "Cells correct: " << correctCells << "/" << totalCells
            << " (" << (correctCells * 100 / totalCells) << "%)\n";
        std::cout << "===========================\n";
    }
    if (fs::exists(WRONG_CELLS_DIR) && fs::is_empty(WRONG_CELLS_DIR)) {
        fs::remove(WRONG_CELLS_DIR);
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  " << argv[0] << " <image.jpg>          -- single image\n";
        std::cout << "  " << argv[0] << " --batch <directory>  -- batch with .dat comparison\n";
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

    cv::Mat image = cv::imread(argv[1]);
    if (image.empty()) {
        std::cerr << "Error: Failed to load image: " << argv[1] << "\n";
        return 1;
    }

    std::cout << "Processing: " << argv[1] << "\n";
    std::array<Cell, 81> cellsArr;
    Sudoku result = {};
    bool success = pipeline(image,cellsArr,result);

    std::cout << "\n--- RECOGNIZED SUDOKU MATRIX ---\n";
    printSudoku(result);
    std::cout << "--------------------------------\n";

    fs::path imgPath(argv[1]);
    std::string datPath = imgPath.parent_path().string()
        + "/" + imgPath.stem().string() + ".dat";
    Sudoku expected = {};
    if (loadDat(datPath, expected))
    {
        fs::remove_all(WRONG_CELLS_DIR);
        CompareStats s = compareResults(result, expected);
        std::cout << "\n--- EXPECTED ---\n";
        printSudoku(expected);
        std::cout << "----------------\n";
        printStats(s);

        if (!success)
            std::cerr << "Error: Pipeline failed. Exiting.\n"; 
        else if (success)
        {
            saveWrongCells(cellsArr, result, expected, imgPath.stem().string());
            if (fs::exists(WRONG_CELLS_DIR) && fs::is_empty(WRONG_CELLS_DIR))
                fs::remove(WRONG_CELLS_DIR);
        }
    }
    else 
        std::cout << "(Missing .dat file - skipping comparison for single image)\n";

    return success ? 0 : 1;

}
