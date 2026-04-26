#pragma once
#include <filesystem>
#include <vector>
#include <algorithm>
#include <random>

namespace fs = std::filesystem;

struct SplitPaths
{
    std::vector<fs::path> train;
    std::vector<fs::path> test;
};

SplitPaths splitFolder(const fs::path &folder, double testRatio = 0.2, int seed = 42)
{
    std::vector<fs::path> allFiles;

    for (const auto &entry : fs::directory_iterator(folder))
    {
        if (entry.is_regular_file())
            allFiles.push_back(entry.path());
    }

    std::mt19937 g(seed);
    std::shuffle(allFiles.begin(), allFiles.end(), g);

    size_t testCount = static_cast<size_t>(allFiles.size() * testRatio);

    SplitPaths result;
    result.test.assign(allFiles.begin(), allFiles.begin() + testCount);
    result.train.assign(allFiles.begin() + testCount, allFiles.end());

    return result;
}