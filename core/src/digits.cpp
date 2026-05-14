#include "digits.hpp"

void cleanupCell(Cell& cell) {
    int margin = 4;
    int h = cell.image.rows;
    int w = cell.image.cols;
    cv::Rect inner(margin, margin, w - 2 * margin, h - 2 * margin);
    cv::Mat cropped = cell.image(inner);

    cv::Mat binary;
    cv::adaptiveThreshold(
        cropped, binary, 255,
        cv::ADAPTIVE_THRESH_GAUSSIAN_C,
        cv::THRESH_BINARY_INV,
        11, 2
    );

    // Usuń drobny szum morfologicznym openingiem
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::morphologyEx(binary, binary, cv::MORPH_OPEN, kernel);

    cv::resize(binary, cell.image, cv::Size(32, 32));
}

void recognizeEmpty(Cell& cell) {
    // Znajdź wszystkie spójne obszary (blobs)
    cv::Mat labels, stats, centroids;
    int numLabels = cv::connectedComponentsWithStats(
        cell.image, labels, stats, centroids
    );

    // Znajdź największy obszar (pomijamy tło = label 0)
    int maxArea = 0;
    for (int i = 1; i < numLabels; i++) {
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area > maxArea) maxArea = area;
    }

    // Cyfra zajmuje co najmniej 50 pikseli z 1024 (32x32)
    // Szum ma małe rozproszone blobs
    std::cout << "row=" << cell.row << " col=" << cell.col << " maxArea=" << maxArea << "\n";

    if (maxArea < 50) {
        cell.value = EMPTY;
    }
}
