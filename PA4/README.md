# PA4: Blender and Model Loading
This project has two parts: create a tray object in Blender, and load and display it in our program. The blender object should be triangulated and should be a single object.

## Keybindings

none

## Running the Project

```bash
mkdir build
cd build
cmake ..
make
./Tutorial -v ../shaders/vertexShader.txt -f ../shaders/fragmentShader.txt -o ../obj/Tray.obj
```
## Notes

The model spins so you can see it from all sides. Each vertex is assigned a color between black and red.
