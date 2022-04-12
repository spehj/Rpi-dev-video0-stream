# Rpi-dev-video0-stream
Copy images from /dev/video0 Rpi camera file to display them on screen (/dev/fb0 file).


First set right image format:
v4l2-ctl --set-fmt-video=width=640,height=480,pixelformat=2
