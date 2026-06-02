#include "grid.hpp"



std::array<cv::Point2f, 4> orderPoints(const std::vector<cv::Point>& pts) {
    std::array<cv::Point2f, 4> rect;

    std::vector<cv::Point2f> ptsf;
    for (const auto& p : pts)
        ptsf.emplace_back(p.x, p.y);

    std::vector<float> sum(4), diff(4);
    for (int i = 0; i < 4; i++) {
        sum[i] = ptsf[i].x + ptsf[i].y;
        diff[i] = ptsf[i].y - ptsf[i].x;
    }

    rect[0] = ptsf[std::min_element(sum.begin(), sum.end()) - sum.begin()]; // top left
    rect[2] = ptsf[std::max_element(sum.begin(), sum.end()) - sum.begin()]; // bottom right
    rect[1] = ptsf[std::min_element(diff.begin(), diff.end()) - diff.begin()]; // top right
    rect[3] = ptsf[std::max_element(diff.begin(), diff.end()) - diff.begin()]; // bottom left

    return rect;
}

cv::Mat detectGrid(const cv::Mat& img) {

    // input picture parameters 
    cv::Scalar mean, stddev;
    cv::meanStdDev(img, mean, stddev);
    double contrast = stddev[0];
    double brightness = mean[0];
    int shortSide = std::min(img.cols, img.rows);

    // dynamic parameters for binarization
    int blockSize = static_cast<int>(shortSide * 0.05);
    if (blockSize < 5)
        blockSize = 5;
    if (blockSize % 2 == 0) blockSize++;

    double C;

    if (brightness < 80)
        C = 2;
    else if (brightness < 150)
        C = 4;
    else
        C = 5;


    // binarization

    cv::Mat thresh;
    cv::adaptiveThreshold(img, thresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, blockSize, C);


    // find contours

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(
        thresh,
        contours,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE
    );

    // sort by area (descending)

    std::sort(contours.begin(), contours.end(),
        [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
            return cv::contourArea(a) > cv::contourArea(b);
        });

    // look for quadrilateral


    std::vector<cv::Point> boardContour;
    double maxArea = img.cols * img.rows;

    for (int i = 0; i < std::min(15, (int)contours.size()); i++) {
        double area = cv::contourArea(contours[i]);
        
        if (area < (maxArea * 0.08)) continue; 

        double peri = cv::arcLength(contours[i], true);
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contours[i], approx, 0.02 * peri, true);

        if (approx.size() == 4) {
            double d1 = cv::norm(approx[0] - approx[1]);
            double d2 = cv::norm(approx[1] - approx[2]);
            double d3 = cv::norm(approx[2] - approx[3]);
            double d4 = cv::norm(approx[3] - approx[0]);

            double avgWidth = (d1 + d3) / 2.0;
            double avgHeight = (d2 + d4) / 2.0;

            if (avgWidth > 0 && avgHeight > 0) {
                double aspectRatio = avgWidth / avgHeight;
                
                // Bardzo bezpieczny, szeroki zakres proporcji
                if (aspectRatio > 0.5 && aspectRatio < 2.0) {
                    boardContour = approx;
                    break;
                }
            }
        }
    }

    if (boardContour.empty() && !contours.empty()) {
        for (int i = 0; i < std::min(5, (int)contours.size()); i++) {
            double area = cv::contourArea(contours[i]);
            if (area < (maxArea * 0.08)) continue;

            std::vector<cv::Point> hull;
            cv::convexHull(contours[i], hull);

            std::vector<cv::Point> approxHull;
            cv::approxPolyDP(hull, approxHull, 0.03 * cv::arcLength(hull, true), true);

            if (approxHull.size() == 4) {
                boardContour = approxHull;
                break; 
            }
        }
    }

    if (boardContour.size() != 4) {
        std::cerr << "Grid not found after fallback!\n";
        return cv::Mat();
    }

    // order points

    auto rect = orderPoints(boardContour);

    std::vector<cv::Point2f> src(rect.begin(), rect.end());

    int side = 288;
    std::vector<cv::Point2f> dst = {
        {0, 0},
        {float(side - 1), 0},
        {float(side - 1), float(side - 1)},
        {0, float(side - 1)}
    };

    // perspective transform

    cv::Mat M = cv::getPerspectiveTransform(src, dst);

    cv::Mat warped;
    cv::warpPerspective(img, warped, M, cv::Size(side, side));

    return warped;
}


void splitGrid(const cv::Mat& img, std::array<Cell, 81>& cells) {
    if (img.empty()) {
        std::cerr << "splitGrid: detectGrid nie znalazl planszy!\n";
        return;
    }

    int cellSize = img.rows / 9;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int idx = row * 9 + col;
            cv::Rect region(col * cellSize, row * cellSize, cellSize, cellSize);
            cells[idx].image = img(region).clone();
            cells[idx].row = row;
            cells[idx].col = col;
            cells[idx].value = UNKNOWN;
        }
    }
}
