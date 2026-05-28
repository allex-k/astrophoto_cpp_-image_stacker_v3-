@echo off
setlocal

if not exist build mkdir build

echo Building...
:: -Wextra -Wpedantic -Wshadow -Wconversion
clang++ -O2 -Wall -std=c++17 ./src/stack_jpeg.cpp -o ./build/stack_jpeg.exe

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo Build successful.

echo Running program...

.\build\stack_jpeg.exe

set run_error=%errorlevel%

if %run_error% neq 0 (
    echo Program exited with error code %run_error%
    exit /b %run_error%
)

echo Done.
endlocal