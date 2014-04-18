Usage
=====

Parameters
----------

Short parameter | Long paremetr         | Description
----------------|-----------------------|-----------------------------------------------------------------------
```-h```        |```--help```           |Shows in app help text
```-d```        |```--device```         |Sets the capture source to V4L2 and specifies the path to the device
```-ss```       |```--sourcestill```    |Sets the capture source to a still image file and specified its source
```-sv```       |```--sourcevideo```    |Sets the capture source to a video file (currently only tested with MJPEG wrapped in AVI) and specifies its source
```-stl```      |```--sourcetimelapse```|Sets the capture source to a set of pre captured timelapse frames
```-iw```       |```--inwidth```        |Set the desired capture width (V4L2 only)
```-ih```       |```--inheight```       |Set the desired capture height (V4L2 only)
```-ow```       |```--outwidth```       |Set the width of the unwrapped image frames (height is calculated from this value)
```-rmin```     |```--minradius```      |Set the lower unwrap radius (0 - 0.5)
```-rmax```     |```--maxradius```      |Set the upper unwrap radius (0 - 0.5)
```-uc```       |```--ucentre```        |Set U original image centre (0 - 1)
```-vc```       |```--vcentre```        |Set V original image centre (0 - 1)
```-a```        |```--offset```         |Set the unwrap image offset angle (equivalent of rotating camera on scope)
```-o```        |```--original```       |Shows the raw frame captured (V4L2 only, useful for camera positioning/calibration)
```-u```        |```--unwrap```         |Show unwrapped video (V4L2 only)
```-s```        |```--stills```         |Capture stills on spacebar press or SIGUSR1 (V4L2) or specify output location of converted image
```-v```        |```--video```          |Capture AVI video (V4L2) or specify output location of converted video
```-m```        |```--mjpg```           |Output frames for MJPG streamer (V4L2 only)
```-t```        |```--timelapse```      |Output captured frame as a timelapse at a given interval
```-sin```      |```--single```         |Capture a single still image and exit
```-sfr```      |```--samplefps```      |Specifies how many samples to use in measuring capture frame rate (V4L2 only)
```-fps```      |```--forcefps```       |Force the video output frame rate to a specified value
```-nuw```      |```--nounwrap```       |Keeps captures in original "wrapped" format

Examples
--------

Note that calibration parameters (```-rmin```, ```-rmax```, ```-uc```, ```-vc```) will almost always be needed in addition to any other parameters provided.

Capture a reasonably sized unwrapped video from ```/dev/video1```: ```$ bubblescope -d /dev/video1 -v cap.avi -iw 1200 -ih 1200 -ow 1000```

Convert an existing video: ```$ bubblescope -sv original_video.avi -ow 800 -v unwrapped_video.mkv```

Convert an existing still image: ```$ bubblescope -ss original_photo.jpg -s -ow 1000 unwrapped_photo.jpg```

Capture a timelapse frame every 2.5 seconds: ```$ bubblescope -t 2500 timelapse_frame_%d.jpg```

Unwrap an original timelpase into a set of frames: ```& bubblescope -t 1 unwrapped_frame_%d.jpg -stl original_frame_%d.jpg```

Unwrap an original timelapse into a video showing 15 images per second: ```& bubblescope -v timelapse_video.mkv -stl original_frame_%d.jpg -fps 15.0```
