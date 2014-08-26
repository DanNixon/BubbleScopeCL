BubbleScopeCL
=============

A BubbleScope 360 degree image capture and processing tool

[![Build Status](https://travis-ci.org/DanNixon/BubbleScopeCL.svg?branch=master)](https://travis-ci.org/DanNixon/BubbleScopeCL)
<a href="https://scan.coverity.com/projects/2865">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/2865/badge.svg"/>
</a>

Allows capture of 360 degree unwrapped and raw images and videos from a video capture device fitted with a [BubbleScope](http://store.bubblepix.com/).

Note that there are very few cameras that this works well with, [this](http://www.maplin.co.uk/p/zyno-1080p-hd-webcam-a98la) camera seems to be the best I can find, the best ones are those with a very short distance between the focusing lens and the image sensor (similar to the Raspberry Pi camera).

See [usage](https://github.com/DanNixon/BubbleScopeCL/blob/master/USAGE.md) for complete instructions and parameter listing.

Requirements
------------

  - [CMake](http://www.cmake.org)
  - [OpenCV](http://opencv.org)
  - [boost](http://www.boost.org)
  - libv4l2

Requirements can be installed on Debian using:   
```sudo add-apt-repository -y ppa:kubuntu-ppa/backports```   
```sudo apt-get update```   
```sudo apt-get install libv4l-0 libv4l-dev libcv-dev libopencv-dev libboost-all-dev```
