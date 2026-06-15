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
