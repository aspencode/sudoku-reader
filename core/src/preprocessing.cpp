#include "preprocessing.hpp"

cv::Mat preprocessing(cv::Mat img){

    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(img, img, cv::Size(3, 3), 0);

    return img;
}
