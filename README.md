BubbleScopeCL
=============

A BubbleScope 360 degree image capture tool for V4L2 sources

Allows capture of 360 degree unwrapped and raw images and videos from a video capture device fitted with a [BubbleScope](http://store.bubblepix.com/).

Note that there are very few cameras that this works well with, [this](http://www.maplin.co.uk/p/zyno-1080p-hd-webcam-a98la) camera seems to be the best I can find, the best ones are those with a very short distance between the focusing lens and the image sensor (similar to the Raspberry Pi camera).

You may encounter issues setting original image capture properties, this is either an issue with OpenCV (more likely to happen with image size) or the camera/V4L2 not supporting the property (more likely with FPS), the ```-p``` argument can be used to check the actual capture size and framerate.   
This can be used to correct the framerate for the video output, if the camera does not support user defined framerate then passing the measured FPS using ```-f``` will ensure the recorded video is of the correct framerate.

Requires OpenCV (tested with version 2.4) and uses the [High Resolution Timer](http://www.songho.ca/misc/timer/timer.html) C++ library.
