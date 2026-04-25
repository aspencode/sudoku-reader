#include <iostream>
#include "types.hpp"
#include "preprocessing.hpp"
#include "grid.hpp"
#include "digits.hpp"
#include "recognition.hpp"

Sudoku pipeline(cv::Mat img){

Sudoku recognizedSudoku;
std::array<Cell, 81> cellsArr;

img = preprocessing(img);
img = detectGrid(img);
cellsArr = splitGrid(img);
for (int i=0;i<81;i++){  
        cellsArr[i]=cleanupCell(cellsArr[i]);
        cellsArr[i]=recognizeEmpty(cellsArr[i]);
        if(!isEmpty(cellsArr[i]))
            cellsArr[i]=recognizeNumber(cellsArr[i]);        
}
for (int i=0;i<9;i++){
    for (int j=0;j<9;j++)
    { 
    recognizedSudoku.values[i][j] = cellsArr[i * 9 + j].value;
    }
}


return recognizedSudoku;
}

int main() {

    cv::Mat image = cv::imread("../data/test.jpg");

    if (image.empty()) {
        std::cerr << "Error: could not load image\n";
        return -1;
    }
    else {
        std::cout << "Image loaded successfully\n";
        std::cout << "Size: " << image.rows << "x" << image.cols << "\n";
        cv::imshow("cell image", image);
        Sudoku s;
        s=pipeline(image);
            for (int i=0;i<9;i++){
            for (int j=0;j<9;j++){
                std::cout<< s.values[i][j]<<"\t";
            }    
                std::cout<<"\n";
            }
    }



    return 0;
}
