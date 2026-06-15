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

## Changing SVM model
To change the SVM model used in the cli apps (sudoku_reader, run_tests), you need to change the model path in corresponding CMakeLists.txt in the folder of the app. The models differ in their kernel usage and are available in core/models.

## Reading the grid from an image
This application runs the core vision and recognition pipeline to process Sudoku puzzles. It automatically detects the grid, isolates individual cells, recognizes the digits, and exports the final matrix into a production-ready .dat file containing a clean 9×9 grid of numbers.

``` 
# single image
./build/apps/sudoku_reader/sudoku_reader <image.jpg>       
# batch   
./build/apps/sudoku_reader/sudoku_reader --batch <directory>  
```


## Running tests
This script runs a specified image or batch of images through the entire pipeline and tests the accuracy of the algorithm. 

Each extracted grid will be compared to the .dat file corresponding to the image.

``` 
# single image
./build/apps/run_tests/run_tests <image.jpg>       
# batch   
./build/apps/run_tests/run_tests --batch <directory>  
```

### Supported .dat file format:
- It must contain exactly 9 valid rows representing the Sudoku grid.
- Each valid row must consist of exactly 9 single-digit numbers (0−9).
- Numbers within a row can be separated by spaces ( ), commas (,), or completely joined together (e.g., 091006000).
- Automatic filtering: Any line containing characters other than digits, spaces, commas, or carriage returns (\r) is automatically ignored



## The grid detection benchmark
This standalone benchmark focuses strictly on the initial computer vision stages. It attempts to locate, deskew, and extract the main 9×9 Sudoku grid from all images in a given directory. Accuracy and amount of positive outcomes and failures is printed out to the console.
```
./build/apps/grid_benchmark/grid_benchmark <input_dir> <output_dir>
```
