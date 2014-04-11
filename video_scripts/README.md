Video Processing Scripts
========================

These scripts can be used for unwrapping videos which are either not of MJPEG encoding, have an audio track or both.

Note that this is a temporary solution until I have time to implement this functionality into BubbleScopeCL properly.

Usage
-----

```./preproc.sh [original video] [frame rate]```   
Generates an MJPEG video to be unwrapped with BubbleScopeCL and an audio track for ```postproc.sh```.

```./postproc.sh [unwrapped video from BubbleScopeCL] [audio track from preproc.sh]```   
Combines the video track from the output video of BubbleScopeCL with the audio track from ```preproc.sh```.
