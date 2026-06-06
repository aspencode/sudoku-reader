#pragma once
#include <opencv2/opencv.hpp>
#include "types.hpp"

// Cleans a cell image (noise removal, contrast enhancement)
void cleanupCell(Cell& cell, cv::Mat& outBinary);

// Detects whether the cell is empty or contains a digit
void recognizeEmpty(Cell& cell, const cv::Mat& binary);
