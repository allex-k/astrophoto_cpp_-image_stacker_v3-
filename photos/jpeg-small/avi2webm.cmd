@echo off

set INPUT=looped-anim-bright.mkv
set OUTPUT=looped-anim-bright.webm

ffmpeg -i "%INPUT%" ^
  -c:v libvpx-vp9 ^
  -crf 32 ^
  -b:v 0 ^
  -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" ^
  "%OUTPUT%"

pause