#pragma once
#include <opencv2/opencv.hpp>
#include "types.hpp"

// Cleans a cell image (noise removal, contrast enhancement)
Cell cleanupCell(Cell cell);

// Detects whether the cell is empty or contains a digit
Cell recognizeEmpty(Cell cell);
