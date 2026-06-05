#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include "hog_utils.hpp"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    fs::path exeDir = fs::path(argv[0]).parent_path();
    fs::path dataFolder = exeDir / ".." / ".." / ".." / "data" / "English" / "Fnt";
    fs::path imagePath = dataFolder / "Sample004" / "img004-00125.png";

    auto hog = createHOG();
    cv::Mat img = cv::imread(imagePath.string());

    auto desc = extractHOG(img, hog);
    std::cout << "Rozmiar wektora cech HOG: " << desc.size() << std::endl;

    int N = 20;
    int cnt;
    std::cout << "Pierwsze " << N << " wartosci wieksze od 0:" << std::endl;
    for (int i = 0; i < (int)desc.size(); ++i)
    {
        if (desc[i] > 0)
        {
            std::cout << "  desc[" << i << "] = " << desc[i] << std::endl;
            cnt++;
        }
        if (cnt == 20)
            break;
    }

    return 0;
}