#pragma once
#include <opencv2/core.hpp>
#include <array>
#include "types.hpp"

bool pipeline(const cv::Mat& inputImg, std::array<Cell, 81>& cellsArr, Sudoku& recognizedSudoku);
