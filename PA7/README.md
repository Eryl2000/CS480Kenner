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
<kbd>O</kbd> | Slow down time
<kbd>P</kbd> | Speed up time
<kbd>H</kbd> | HARRIS!
<kbd>J</kbd> | NO HARRIS!
<kbd>I</kbd> | Scale planet distances
<kbd>K</kbd> | Normalize planet distances

## Group Members

Eryl Kenner, Kyle Brainard, Garrett Moore

## Extra Credit

The planets are loaded in according to the planets.csv file inside the obj folder

Press O to slow down the simulation speed\
Press P to speed up the simulation speed

Press I to move the planets into the actual proportional distance from the sun\
Press K to move the planets into the normalized distance in which they as close to each other as possible

Press H to change the textures to a picture of Dr. Harris\
Press J to change the textures back to normal

## Notes

ImageMagick version 6.9.7-4.
