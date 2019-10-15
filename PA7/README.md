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
./SolarSystem -v ../shaders/vertexShader.txt -f ../shaders/fragmentShader.txt
```

Key | Function
------------ | -------------
<kbd>W</kbd> | Move forward
<kbd>S</kbd> | Move backward
<kbd>A</kbd> | Move left
<kbd>D</kbd> | Move right
<kbd>Shift</kbd> | Speed up
<kbd>Left Arrow</kbd> | Look left
<kbd>Right Arrow</kbd> | Look right
<kbd>Up Arrow</kbd> | Look up
<kbd>Down Arrow</kbd> | Look down
<kbd>O</kbd> | Slow down time
<kbd>P</kbd> | Speed up time
<kbd>H</kbd> | HARRIS!
<kbd>J</kbd> | NO HARRIS!
<kbd>I</kbd> | Scale planet distances
<kbd>K</kbd> | Normalize planet distances

## Group Members

Eryl Kenner, Kyle Brainard, Garrett Moore

## Notes

ImageMagick version 6.9.7-4.
