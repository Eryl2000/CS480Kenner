# PA6: Assimp Texture Loading

This project uses ASSIMP to load and display textures on objects.

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

##Group Members

Eryl Kenner, Kyle Brainard, Garrett Moore

## Notes

We used ImageMagick in our project, version 6.9.7-4.\
Also, you can move the object using the WASD keys.
