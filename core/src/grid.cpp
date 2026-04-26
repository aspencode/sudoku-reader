#include "grid.hpp"


cv::Mat detectGrid(const cv::Mat& img) {
    // STUB: temporary logic for pipeline testing (returns original)
    return img.clone(); 
}


void splitGrid(const cv::Mat& img, std::array<Cell, 81>& cells) {
    // STUB: temporary logic for pipeline testing

    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {   
            int idx = row * 9 + col;
            cells[idx].row = row;
            cells[idx].col = col;
        }
    }

    return;
}
