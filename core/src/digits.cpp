#include "digits.hpp"

void cleanupCell(Cell& cell, cv::Mat& outBinary) {
    int h = cell.image.rows;
    int w = cell.image.cols;

    cv::Mat original = cell.image.clone();
    
    int margin = std::max(1, static_cast<int>(std::min(h, w) * 0.15));

    int blockSize = static_cast<int>(std::min(h, w) * 0.25);
    if (blockSize < 5)  blockSize = 5;
    if (blockSize % 2 == 0) blockSize++;

    cv::adaptiveThreshold(
        cell.image, outBinary, 255,
        cv::ADAPTIVE_THRESH_GAUSSIAN_C,
        cv::THRESH_BINARY_INV,
        blockSize, 2
    );

    outBinary(cv::Rect(0, 0, w, margin)).setTo(0);
    outBinary(cv::Rect(0, h - margin, w, margin)).setTo(0);
    outBinary(cv::Rect(0, 0, margin, h)).setTo(0);
    outBinary(cv::Rect(w - margin, 0, margin, h)).setTo(0);

    int kernelSize = std::max(2, static_cast<int>(std::min(h, w) * 0.06));
    cv::Mat openKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(outBinary, outBinary, cv::MORPH_OPEN, openKernel);

    cv::Mat binaryForDigit = outBinary.clone();

    int dilateSize = std::max(1, static_cast<int>(std::min(h, w) * 0.06));
    cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilateSize, dilateSize));
    cv::dilate(outBinary, outBinary, dilateKernel);

    cv::Mat labels, stats, centroids;
    int numLabels = cv::connectedComponentsWithStats(
        outBinary, labels, stats, centroids
    );

    int maxArea = 0;
    int maxLabel = -1;
    for (int i = 1; i < numLabels; i++) {
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area > maxArea) { maxArea = area; maxLabel = i; }
    }

    double maxAreaRatio = (double)maxArea / (h * w);
    cv::Mat result(32, 32, CV_8UC1, cv::Scalar(255));

    if (maxLabel != -1 && maxAreaRatio >= 0.05) {

        int minFragArea = std::max(4, maxArea / 12);
        int unionL = w, unionT = h, unionR = 0, unionB = 0;

        for (int i = 1; i < numLabels; i++) {
            int area = stats.at<int>(i, cv::CC_STAT_AREA);
            if (area < minFragArea) continue;
            int cx = stats.at<int>(i, cv::CC_STAT_LEFT);
            int cy = stats.at<int>(i, cv::CC_STAT_TOP);
            int cw = stats.at<int>(i, cv::CC_STAT_WIDTH);
            int ch = stats.at<int>(i, cv::CC_STAT_HEIGHT);
            unionL = std::min(unionL, cx);
            unionT = std::min(unionT, cy);
            unionR = std::max(unionR, cx + cw);
            unionB = std::max(unionB, cy + ch);
        }

        int pad = std::max(1, static_cast<int>(std::min(h, w) * 0.03));
        int bx = std::max(0, unionL - pad);
        int by = std::max(0, unionT - pad);
        int bw = std::min(w - bx, (unionR - unionL) + 2 * pad);
        int bh = std::min(h - by, (unionB - unionT) + 2 * pad);

        cv::Mat croppedBinary = binaryForDigit(cv::Rect(bx, by, bw, bh)).clone();
        cv::Mat digitInverted;
        cv::bitwise_not(croppedBinary, digitInverted);

        const int targetSize = 28;
        double scale = std::min((double)targetSize / bw, (double)targetSize / bh);
        int newW = static_cast<int>(bw * scale);
        int newH = static_cast<int>(bh * scale);

        cv::Mat resized;
        cv::InterpolationFlags interp = (scale < 1.0) ? cv::INTER_AREA : cv::INTER_LINEAR;
        cv::resize(digitInverted, resized, cv::Size(newW, newH), 0, 0, interp);

        int offsetX = (32 - newW) / 2;
        int offsetY = (32 - newH) / 2;
        resized.copyTo(result(cv::Rect(offsetX, offsetY, newW, newH)));
    }

    cell.image = result;
    /*
    std::string base = "C:\\temp\\cell_"
        + std::to_string(cell.row) + "_" + std::to_string(cell.col);
    cv::imwrite(base + "_original.png", original);
    cv::imwrite(base + "_binary.png", outBinary);
    cv::imwrite(base + "_gray.png", cell.image);
    */
}

void recognizeEmpty(Cell& cell, const cv::Mat& binary) {
    cv::Mat labels, stats, centroids;
    int numLabels = cv::connectedComponentsWithStats(
        binary, labels, stats, centroids
    );
    int maxArea = 0;
    for (int i = 1; i < numLabels; i++) {
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area > maxArea) maxArea = area;
    }
    double maxAreaRatio = (double)maxArea / (binary.rows * binary.cols);
    if (maxAreaRatio < 0.05) {
        cell.value = EMPTY;
    }
}