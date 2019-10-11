# PA7: Solar System

An interactive solar system!

## Dependencies

```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
sudo apt-get install libassimp-dev
sudo apt-get install libmagick++-dev
```

## Running the Project

```bash
mkdir build
cd build
cmake ..
make
./Tutorial -v ../shaders/vertexShader.txt -f ../shaders/fragmentShader.txt -o ../obj/Tray.obj
```

#Key | Function
------------ | -------------
<kbd>W</kbd> | Moves forward
<kbd>S</kbd> | Move backward
<kbd>A</kbd> | Move left
<kbd>D</kbd> | Move right

## Group Members

Eryl Kenner, Kyle Brainard, Garrett Moore

## Notes

ImageMagick version 6.9.7-4.
