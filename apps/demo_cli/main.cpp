#include <iostream>
#include "types.hpp"
#include "preprocessing.hpp"
#include "grid.hpp"
#include "digits.hpp"
#include "recognition.hpp"

int main() {
    Cell c;

    c.image = cv::imread("../data/test.jpg");

    if (c.image.empty()) {
        std::cerr << "Error: could not load image\n";
    }
    else {
    std::cout << "Image loaded successfully\n";
    std::cout << "Size: " << c.image.rows << "x" << c.image.cols << "\n";
    cv::imshow("cell image", c.image);
    }

    c.row = 0;
    c.col = 1;
    c.value = 5;

    std::cout << "Row: " << c.row << "\n";
    std::cout << "Col: " << c.col << "\n";
    std::cout << "Value: " << c.value << "\n";

    std::cout << "isEmpty: " << isEmpty(c) << "\n";
    std::cout << "isKnown: " << isKnown(c) << "\n";
    std::cout << "isDigit: " << isDigit(c) << "\n";


    return 0;
}
