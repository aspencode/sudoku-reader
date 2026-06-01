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
./apps/demo_cli/cli <path_to_sudoku_image>
```

## Grid detection benchmark
```
# Usage: ./build/apps/grid_benchmark/grid_benchmark [input_dir] [output_dir]
./build/apps/grid_benchmark/grid_benchmark ./data ./data_results
```
