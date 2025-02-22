# PA8: Bullet

The purpose of this assignment is to become familiar with Bullet Physics, a 3D physics simulation engine. At the end of the assignment, you should be able to integrate 3D physics using Bullet into an OpenGL program with user interaction.

## Dependencies

```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
sudo apt-get install libassimp-dev
sudo apt-get install libmagick++-dev
sudo apt-get install libbullet-dev
```

## Running the Project

```bash
mkdir build
cd build
cmake ..
make
./Bullet -v ../shaders/vertexShader.txt -f ../shaders/fragmentShader.txt
```

Key | Function
------------ | -------------
<kbd>W</kbd> | Move forward camera
<kbd>S</kbd> | Move backward camera
<kbd>A</kbd> | Move left camera
<kbd>D</kbd> | Move right camera
<kbd>Shift</kbd> | Speed up camera
<kbd>Up</kbd> | Move cube up
<kbd>Down</kbd> | Move cube down
<kbd>Left</kbd> | Move cube left
<kbd>Right</kbd> | Move cube right

## Group Members

Eryl Kenner, Kyle Brainard, Garrett Moore

## Extra Credit

N/A

## Notes

ImageMagick version 6.9.7-4.
