#pragma once
#include <opencv2/opencv.hpp>

constexpr int UNKNOWN = -1;
constexpr int EMPTY = 0;

struct Sudoku
{
int values[9][9]; // -1 = unknown, 0 = empty, 1-9 = digits
};
 
struct Cell{
cv::Mat image; // 32x32 px
int row=UNKNOWN; // 0-8 row number
int col=UNKNOWN; // 0-8 column number
int value=UNKNOWN; // -1 unknown, 0 empty, 1-9 digit
};

inline bool isEmpty(const Cell& c) {
    return c.value == 0;
}

inline bool isKnown(const Cell& c) {
    return c.value != -1;
}

inline bool isDigit(const Cell& c) {
    return c.value >= 1 && c.value <= 9;
}
