## Requirements
- C++17
- OpenCV 4.6.0
- CMake 3.28.3

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

## Running the CLI demo

The application reads Sudoku images from the data directory in the project root.
```
cd build
./apps/demo_cli/cli
```

