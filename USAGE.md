Usage
=====

TODO: Further expansion needed here

Examples
--------

Note that calibration parameters (```--min-radius```, ```--max-radius```, ```--u-centre```, ```--v-centre```) will almost always be needed in addition to any other parameters provided.

Capture a reasonably sized unwrapped video from ```/dev/video1```: ```$ bubblescope --v4l2-capture /dev/video1 --ouput-video cap.avi --capture-width 1200 --capture-height 1200 --unwrap-width 1000```

Convert an existing video: ```$ bubblescope --convert-video original_video.avi -ow 800 --output-video unwrapped_video.mkv```

Convert an existing still image: ```$ bubblescope --still-convert original_photo.jpg --unwrap-width 1000  --output-stills unwrapped_photo.jpg```

Unwrap an original timelpase into a set of frames: ```& bubblescope -t 1 unwrapped_frame_%d.jpg -stl original_frame_%d.jpg```

Unwrap an original timelapse into a video showing 15 images per second: ```& bubblescope --timelapse-convert original_frame_%d.jpg --force-fps 15.0 --output-video timelapse_video.mkv```
