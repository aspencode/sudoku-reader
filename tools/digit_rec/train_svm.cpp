#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include "hog_utils.hpp"
#include "split_data.hpp"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    fs::path exeDir = fs::path(argv[0]).parent_path();
    fs::path rootDir = exeDir.parent_path().parent_path().parent_path();
    fs::path dataFolder = rootDir / "data" / "English" / "Fnt";

    const double TEST_RATIO = 0.2;
    const unsigned int SEED = 42;

    auto hog = createHOG();
    std::vector<std::vector<float>> features;
    std::vector<int> labels;

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

    return 0;
}