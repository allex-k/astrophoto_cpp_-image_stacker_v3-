@echo off
setlocal

set INPUT=anim.mkv
set REVERSED=reversed.mkv
set FINAL=final.mkv

:: Реверс с теми же параметрами что и оригинал
ffmpeg -i %INPUT% -vf reverse -c:v ffv1 -level 3 -g 1 -pix_fmt yuv422p -r 25 %REVERSED%

:: Список для склейки
(
echo file '%INPUT%'
echo file '%REVERSED%'
) > concat.txt

:: Склейка
ffmpeg -f concat -safe 0 -i concat.txt -c copy %FINAL%

echo.
echo Готово: %FINAL%
pause