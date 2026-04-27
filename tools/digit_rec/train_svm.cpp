#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include "hog_utils.hpp"
#include "split_data.hpp"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    fs::path exeDir = fs::path(argv[0]).parent_path();
    fs::path rootDir = exeDir.parent_path().parent_path().parent_path();
    fs::path dataFolder = rootDir / "data" / "English" / "Fnt";

    fs::path modelFolder = rootDir / "tools" / "digit_rec";
    fs::path modelPath = modelFolder / "svm_model.xml";

    const double TEST_RATIO = 0.2;
    const unsigned int SEED = 42;

    auto hog = createHOG();
    std::vector<std::vector<float>> features;
    std::vector<int> labels;

    std::cout << "Wczytywanie danych treningowych..." << std::endl;

    for (int digit = 1; digit <= 9; digit++)
    {
        std::ostringstream folderNum;
        folderNum << std::setw(3) << std::setfill('0') << digit + 1;

        fs::path folder = dataFolder / ("Sample" + folderNum.str());

        auto split = splitFolder(folder, TEST_RATIO, SEED);

        for (auto &path : split.train)
        {
            cv::Mat img = cv::imread(path.string());
            if (img.empty())
                continue;

            auto desc = extractHOG(img, hog);
            features.push_back(desc);
            labels.push_back(digit);
        }
    }

    int nSamples = features.size();

    std::cout << "Wczytano: " << nSamples << " zdjec." << std::endl;

    int nFeatures = features[0].size();
    cv::Mat trainData(nSamples, nFeatures, CV_32F);
    cv::Mat labelsMat(nSamples, 1, CV_32S);

    for (int i = 0; i < nSamples; i++)
    {
        for (int j = 0; j < nFeatures; j++)
            trainData.at<float>(i, j) = features[i][j];
        labelsMat.at<int>(i, 0) = labels[i];
    }

    auto svm = cv::ml::SVM::create();
    svm->setType(cv::ml::SVM::C_SVC);
    svm->setKernel(cv::ml::SVM::RBF);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 1000, 1e-6));

    std::cout << "Rozpoczynanie treningu..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    svm->trainAuto(trainData, cv::ml::ROW_SAMPLE, labelsMat);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Trening trwal: " << std::fixed << std::setprecision(2)
              << elapsed.count() << " sekund." << std::endl;

    if (!fs::exists(modelFolder))
        fs::create_directories(modelFolder);

    svm->save(modelPath.string());
    std::cout << "Model zapisany w: " << modelPath << std::endl;

    return 0;
}