#include "recognition.hpp"
#include "hog_utils.hpp"
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

static cv::Ptr<cv::ml::SVM> g_svm;
static cv::HOGDescriptor g_hog;

void initRecognition(const std::string &modelPath)
{
    g_hog = createHOG();
    g_svm = cv::ml::SVM::load(modelPath);
    if (!g_svm)
        throw std::runtime_error("Failed to load SVM model: " + modelPath);
}

void recognizeNumber(Cell &cell)
{
    if (!g_svm)
        throw std::runtime_error("Call initRecognition() before recognizeNumber()");

    auto desc = extractHOG(cell.image, g_hog);
    cv::Mat sample(1, (int)desc.size(), CV_32F, desc.data());
    cell.value = static_cast<int>(g_svm->predict(sample));
}
