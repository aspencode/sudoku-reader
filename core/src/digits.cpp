#include "digits.hpp"

void cleanupCell(Cell& cell) {
    int margin = 4;
    int h = cell.image.rows;
    int w = cell.image.cols;

    // 1. Binaryzacja na czystym obrazie — bez bia³ych marginesów
    cv::Mat binary;
    cv::adaptiveThreshold(
        cell.image, binary, 255,
        cv::ADAPTIVE_THRESH_GAUSSIAN_C,
        cv::THRESH_BINARY_INV,
        11, 2
    );

    // 2. Marginesy PO binaryzacji — czarne (0 = t³o po BINARY_INV)
    binary(cv::Rect(0, 0, w, margin)).setTo(0);
    binary(cv::Rect(0, h - margin, w, margin)).setTo(0);
    binary(cv::Rect(0, 0, margin, h)).setTo(0);
    binary(cv::Rect(w - margin, 0, margin, h)).setTo(0);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::morphologyEx(binary, binary, cv::MORPH_OPEN, kernel);
    cell.image = binary;
}

void recognizeEmpty(Cell& cell) {
    cv::Mat labels, stats, centroids;
    int numLabels = cv::connectedComponentsWithStats(
        cell.image, labels, stats, centroids
    );

    int maxArea = 0;
    for (int i = 1; i < numLabels; i++) {
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area > maxArea) maxArea = area;
    }

    int totalPixels = cell.image.rows * cell.image.cols;
    double maxAreaRatio = (double)maxArea / totalPixels;

    std::cout << "row=" << cell.row << " col=" << cell.col
        << " maxAreaRatio=" << maxAreaRatio << "\n";

    if (maxAreaRatio < 0.05) {
        cell.value = EMPTY;
    }
}