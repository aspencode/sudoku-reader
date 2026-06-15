# Sudoku-reader
Sudoku Grid Detection and Recognition System.

This project focuses on automated detection, extraction, and recognition of Sudoku puzzles.

## Dependencies
- C++17
- OpenCV 4.6.0
- CMake 3.28.3
### Dependency Installation
#### Windows
Install a C++17-compatible compiler.

Install CMake 3.28.3 or newer and add it to the system `PATH`.

Install OpenCV 4.6.0:
-   Download and extract OpenCV 4.6.0.
-   Set the `OpenCV_DIR` environment variable to the OpenCV build directory (e.g. `C:\opencv\build`).

Verify the installation: `cmake --version`
#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install -y build-essential cmake libopencv-dev
```
#### Arch Linux
```
sudo pacman -S gcc cmake opencv
```
#### Fedora
```
sudo dnf install gcc-c++ cmake opencv-devel
```
#### macOS

Install Homebrew if it is not already installed, then run:

```
brew install cmake opencv
```

Apple Clang included with Xcode Command Line Tools provides C++17 support. If the tools are not installed, run:

```
xcode-select --install
```
## Building
```
cd build
cmake ..
make -j
```
### Example dataset

To download a sample dataset:
```
mkdir -p data
cd data
wget https://raw.githubusercontent.com/wichtounet/sudoku_dataset/master/datasets/v2_test.tar.bz2
tar -xjf v2_test.tar.bz2
```

## The CLI demo
This script runs a specified image or batch of images through the entire pipeline and tests the accuracy of the algorithm.
``` 
# single image
./build/apps/demo_cli/cli <image.jpg>       
# batch with .dat comparison  
./build/apps/demo_cli/cli --batch <directory>  
```

## The grid detection benchmark
This script tries to find and extract a sudoku grid from all images in a given directory,  and tests the accuracy of the algorithm.
```
# Usage: ./build/apps/grid_benchmark/grid_benchmark [input_dir] [output_dir]
./build/apps/grid_benchmark/grid_benchmark ./data/v2_test ./data_results_v2_test
```
