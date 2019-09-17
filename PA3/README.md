# PA3: Moons
This project adds another object to the scene. This object is a "moon" which orbits our original cube.

## Keybindings

These Keybindingsare for the planet cube. The moon will continue to orbit the planet regardless of the planet's motion.
Key | Function
------------ | -------------
<kbd>A</kbd> | reverse the spinning direction of the planet
<kbd>S</kbd> | reverse the orbiting direction of the planet
<kbd>Z</kbd> | stop the spinning of the planet
<kbd>X</kbd> | stop the orbiting of the planet
Left Mouse | reverse the spinning direction of the planet
Right Mouse | reverse the orbiting direction of the planet

## Running the Project

```bash
mkdir build
cd build
cmake ..
make
./Tutorial -v ../shaders/vertexShader.txt -f ../shaders/fragmentShader.txt
```
