BubbleScopeCL
=============

A BubbleScope 360 degree image capture and processing tool.

[![Build Status](https://travis-ci.org/DanNixon/BubbleScopeCL.svg?branch=master)](https://travis-ci.org/DanNixon/BubbleScopeCL)
<a href="https://scan.coverity.com/projects/2865">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/2865/badge.svg"/>
</a>

Allows capture of 360 degree unwrapped and raw images and videos from a video
capture device fitted with a [BubbleScope](http://store.bubblepix.com/).

Note that there are very few cameras that this works well with,
[this](http://www.maplin.co.uk/p/zyno-1080p-hd-webcam-a98la) camera seems to be
the best I can find, the best ones are those with a very short distance between
the focusing lens and the image sensor (similar to the Raspberry Pi camera).

Usage Examples
--------------

Note that calibration parameters (`--min-radius`, `--max-radius`, `--u-centre`,
`--v-centre`) will almost always be needed in addition to any other parameters
provided.

Capture a reasonably sized unwrapped video from `/dev/video1`: `$ bubblescope
--v4l2-capture /dev/video1 --ouput-video cap.avi --capture-width 1200
--capture-height 1200 --unwrap-width 1000`

Convert an existing video: `$ bubblescope --video-convert original_video.avi -ow
800 --output-video unwrapped_video.mkv`

Convert an existing still image: `$ bubblescope --still-convert
original_photo.jpg --unwrap-width 1000  --output-stills unwrapped_photo.jpg`

Unwrap an original timelpase into a set of frames: `$ bubblescope -t 1
unwrapped_frame_%d.jpg -stl original_frame_%d.jpg`

Unwrap an original timelapse into a video showing 15 images per second: `$
bubblescope --timelapse-convert original_frame_%d.jpg --force-fps 15.0
--output-video timelapse_video.mkv`

Requirements
------------

  - [CMake](http://www.cmake.org)
  - [OpenCV](http://opencv.org)
  - [boost](http://www.boost.org)
  - libv4l2

Requirements can be installed on Debian using:

```
sudo add-apt-repository -y ppa:kubuntu-ppa/backports
sudo apt-get update
sudo apt-get install libv4l-0 libv4l-dev libcv-dev libopencv-dev libboost-all-dev
```
