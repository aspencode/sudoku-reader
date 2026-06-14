#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "hog_utils.hpp"
#include "split_data.hpp"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    fs::path exeDir = fs::path(argv[0]).parent_path();
    fs::path rootDir = exeDir.parent_path().parent_path().parent_path();
    fs::path dataFolder = rootDir / "data" / "English" / "Fnt";

    fs::path modelFolder = rootDir / "tools" / "digit_rec";
    fs::path modelPath = modelFolder / "svm_model_chi2.xml";

    const double TEST_RATIO = 0.2;
    const unsigned int SEED = 42;

    auto hog = createHOG();
    auto svm = cv::ml::SVM::load(modelPath.string());

    int predictCorrect = 0;
    int predictTotal = 0;

    fs::path csvPath = modelFolder / "chi2_eval_results.csv";
    std::ofstream csvFile(csvPath.string());
    csvFile << "Digit,Correct,Total,Accuracy (%)\n";

    for (int digit = 1; digit <= 9; digit++)
    {
        std::ostringstream folderNum;
        folderNum << std::setw(3) << std::setfill('0') << digit + 1;

        fs::path folder = dataFolder / ("Sample" + folderNum.str());

        auto split = splitFolder(folder, TEST_RATIO, SEED);

        int classCorrect = 0;
        int classTotal = 0;

        for (auto &path : split.test)
        {
            cv::Mat img = cv::imread(path.string());
            if (img.empty())
                continue;

            auto desc = extractHOG(img, hog);
            cv::Mat sample(1, desc.size(), CV_32F, desc.data());

            float predicted = svm->predict(sample);

            if ((int)predicted == digit)
                classCorrect++;

            classTotal++;
        }

        if (classTotal > 0)
        {
            double accuracy = (double)classCorrect / classTotal * 100.0;

            std::cout << "Digit " << digit << " (Sample" << folderNum.str() << "): "
                      << std::fixed << std::setprecision(2) << accuracy << "% "
                      << "(" << classCorrect << "/" << classTotal << ")" << std::endl;

            csvFile << digit << ","
                    << classCorrect << ","
                    << classTotal << ","
                    << std::fixed << std::setprecision(2) << accuracy << "\n";

            predictCorrect += classCorrect;
            predictTotal += classTotal;
        }
    }

    if (predictTotal > 0)
    {
        double totalAccuracy = (double)predictCorrect / predictTotal * 100.0;

        std::cout << std::endl
                  << "Total accuracy on test split: "
                  << std::fixed << std::setprecision(2) << totalAccuracy << "% "
                  << "(" << predictCorrect << "/" << predictTotal << ")" << std::endl;

        csvFile << "TOTAL,"
                << predictCorrect << ","
                << predictTotal << ","
                << std::fixed << std::setprecision(2) << totalAccuracy << "\n";
    }

    csvFile.close();
    std::cout << "\nWyniki zapisane do: " << csvPath.string() << std::endl;

    return 0;
}