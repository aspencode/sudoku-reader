#include "preprocessing.hpp"

void preprocessing(const cv::Mat& src, cv::Mat& dst) {

    cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(dst, dst, cv::Size(3, 3), 0);

    return;
}
