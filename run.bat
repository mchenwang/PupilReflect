@echo off

if not exist build\ mkdir build

cd build
cmake .. -B .
cmake --build .
cd ..