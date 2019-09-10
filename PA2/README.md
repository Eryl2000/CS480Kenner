# PA2: Interaction
This project reads keyboard/mouse input and stops the cube or changes its direction based on the input.

## Keybindings
Key | Function
------------ | -------------
<kbd>A</kbd> | reverse the spinning direction of the cube
<kbd>S</kbd> | reverse the orbiting direction of the cube
<kbd>Z</kbd> | stop the spinning of the cube
<kbd>X</kbd> | stop the orbiting of the cube
Left Mouse | reverse the spinning direction of the cube
Right Mouse | reverse the orbiting direction of the cube

## Running the Project

```bash
mkdir build
cd build
cmake ..
make
./Tutorial -v ../shaders/vertexShader.txt -f ../shaders/fragmentShader.txt
```
