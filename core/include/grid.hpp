#pragma once
#include <opencv2/opencv.hpp>
#include <array>
#include "types.hpp"

// 
// returns image with a cropped out sudoku grid
cv::Mat detectGrid(cv::Mat img);

// returns arr of 81 cell elements with images of each cell
std::array<Cell, 81> splitGrid(cv::Mat img);
