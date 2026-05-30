@echo off
setlocal

set OUTPUT=output.mp4
set FRAMERATE=24

(for %%f in (IMG*.jpg) do echo file '%%f') > list.txt

:: ffmpeg -f concat -safe 0 -i list.txt -vf "fps=%FRAMERATE%" -c:v libx264 -crf 0 -preset veryslow %OUTPUT%

:: lossless
ffmpeg -f concat -safe 0 -i list.txt -c:v ffv1 -framerate %FRAMERATE% output.mkv

echo.
echo Готово: %OUTPUT%
pause