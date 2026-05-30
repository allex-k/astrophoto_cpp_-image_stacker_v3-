@echo off
setlocal

if not exist build mkdir build

:: start time
for /f %%i in ('powershell -NoProfile -Command "[System.Diagnostics.Stopwatch]::GetTimestamp()"') do set start=%%i

echo Building...
echo.

clang++ -O2 -Wall -std=c++17 ./src/stack_jpeg.cpp -o ./build/stack_jpeg.exe

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

:: end time
for /f %%i in ('powershell -NoProfile -Command "$swFreq=[System.Diagnostics.Stopwatch]::Frequency; $end=[System.Diagnostics.Stopwatch]::GetTimestamp(); [int](($end-%start%)*1000/$swFreq)"') do set build_ms=%%i

echo Build successful. Compilation time: %build_ms% ms
echo Running program...
echo.

.\\build\\stack_jpeg.exe

set run_error=%errorlevel%

if %run_error% neq 0 (
    echo Program exited with error code %run_error%
    exit /b %run_error%
)

echo.
echo Done.
endlocal