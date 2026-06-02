#pragma once
#include <opencv2/opencv.hpp>
#include <array>
#include "types.hpp"

// helper function used in detectGrid
std::array<cv::Point2f, 4> orderPoints(const std::vector<cv::Point>& pts);

// returns image with a cropped out sudoku grid
cv::Mat detectGrid(const cv::Mat& img);

// returns arr of 81 cell elements with images of each cell
void splitGrid(const cv::Mat& img, std::array<Cell, 81>& cells);
