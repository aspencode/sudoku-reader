#include "grid.hpp"


cv::Mat detectGrid(cv::Mat img)
{
    return img;
}


std::array<Cell, 81> splitGrid(cv::Mat img)
{
    // STUB: temporary logic for pipeline testing
    std::array<Cell, 81> cellsArr;


    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {   
            Cell c;
            c.row = row;
            c.col = col;
            c.value = -1;
            cellsArr[row * 9 + col] = c;
        }
    }

    return cellsArr;
}
