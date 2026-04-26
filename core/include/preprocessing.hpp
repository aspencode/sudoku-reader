#pragma once
#include <opencv2/opencv.hpp>

// initial preprocessing (grayscale+noise removal)
void preprocessing(const cv::Mat& src, cv::Mat& dst);
