#!/bin/bash

# Usage: ./preproc.sh [original video] [frame rate]

ffmpeg -i "$1" -vn -acodec libvorbis "$1"_AUDIO.ogg
ffmpeg -i "$1" -an -vcodec mjpeg -qscale 1 -r "$2" "$1"_ORIGINAL_MJPEG.avi
