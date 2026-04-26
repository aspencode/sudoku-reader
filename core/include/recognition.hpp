#pragma once
#include <opencv2/opencv.hpp>
#include "types.hpp"

// recognize what number a cell contains (value={0-9}, 0 for empty)
void recognizeNumber(Cell& cell);
