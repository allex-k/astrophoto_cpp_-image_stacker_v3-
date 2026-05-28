set -e
mkdir -p build
g++ -O2 -Wall -std=c++17 ./src/stack_jpeg.cpp -o ./build/stack_jpeg
./build/stack_jpeg
