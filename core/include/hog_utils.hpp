#pragma once
#include <opencv2/opencv.hpp>

cv::HOGDescriptor createHOG()
{
    cv::Size winSize(32, 32);
    cv::Size blockSize(8, 8);
    cv::Size blockStride(4, 4);
    cv::Size cellSize(8, 8);
    int nbins = 9;
    return cv::HOGDescriptor(winSize, blockSize, blockStride, cellSize, nbins);
}

std::vector<float> extractHOG(const cv::Mat &img, cv::HOGDescriptor &hog)
{
    cv::Mat gray, resized;

    if (img.channels() == 1)
        gray = img;
    else
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::resize(gray, resized, cv::Size(32, 32));

    std::vector<float> desc;
    hog.compute(resized, desc);
    return desc;
}