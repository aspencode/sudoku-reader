#pragma once
#include <opencv2/opencv.hpp>

// initial preprocessing (grayscale+noise removal)
cv::Mat preprocessing(cv::Mat img);
