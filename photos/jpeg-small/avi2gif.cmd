REM ffmpeg -i looped-anim-bright.mkv -r 15 -filter_complex "[0:v]scale=iw/2:ih/2:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse=dither=sierra2_4a" looped-anim-bright.gif

ffmpeg -i looped-anim-bright.mkv -r 15 -filter_complex "[0:v]scale=iw/2:ih/2:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse=dither=sierra2_4a" looped-anim-bright.gif