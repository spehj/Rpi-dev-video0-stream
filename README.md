# Rpi-dev-video0-stream
Copy images from /dev/video0 Rpi camera file to display them on screen (/dev/fb0 file).


First set right image format:
```console
v4l2-ctl --set-fmt-video=width=640,height=480,pixelformat=2
```

Check for your screen size and bit depth using:
http://raspberrycompote.blogspot.si/2012/12/low-level-graphics-on-raspberry-pi-part_9509.html

Then edit pr2.c and pr3.c defines.

Compile each .c program with:
```console
gcc prX.c -Wall -o prX
```

Then run each program in a separate bash shell.
