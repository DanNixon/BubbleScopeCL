#!/bin/bash

# Usage: ./postproc.sh [unwrapped video] [original audio]

ffmpeg -i "$1" -i "$2" -vcodec copy -acodec aac -strict experimental "$1"_wAUDIO.mkv
