# PA9: Lighting

The purpose of this assignment is to create multiple shaders to do per vertex lighting and per fragment lighting in OpenGL.

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
./Bullet
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
<kbd>R</kbd> | Diffuse Lighting Lower
<kbd>T</kbd> | Diffuse Lighting Higher
<kbd>B</kbd> | Ambient Lighting Lower
<kbd>N</kbd> | Ambient Lighting Higher
<kbd>F</kbd> | Specular Lighting Lower
<kbd>G</kbd> | Specular Lighting Higher
<kbd>M</kbd> | Swap Shaders(Per Vertex as Default)

## Group Members

Eryl Kenner, Kyle Brainard, Garrett Moore

## Extra Credit

N/A

## Notes

ImageMagick version 6.9.7-4.
