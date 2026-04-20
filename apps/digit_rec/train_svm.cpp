#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include "hog_utils.hpp"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    fs::path exeDir = fs::path(argv[0]).parent_path();
    fs::path rootDir = exeDir.parent_path().parent_path().parent_path();
    fs::path dataFolder = rootDir / "data" / "English" / "Fnt";

    auto hog = createHOG();

    return 0;
}