# PA4: Object Loader

## Dependency Instructions
To run this project installation of three programs is required: [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3.

### Ubuntu/Linux
```bash 
 sudo apt-get install libglew-dev libglm-dev libsdl2-dev
```

<br>
## Building and Running - CMake Instructions
The building of the project is done using CMake. 
```bash
mkdir build
cd build
cmake ..
make
./Tutorial
```

## Object Files
This program can load any object from a correctly formatted .obj file. By default the "objFiles/box.obj" file is loaded. This can be changed using the '-l' command line flag. See Command Line Arugments section for details. Note: The object that is loaded is colored randomly unless otherwise specified.
<br>

## Extra Credit - Color Loading
I also implimented the extra credit color loading via materials. By add a '-c' flag at runtime, the program will look for a .mtl with the same name as the obj file to load materials from. It will then load the color version if possible. The default box object can be loaded in color with the '-c' flag.
<br>

## IMGUI Menu System
There is a menu system that runs in a seperate window. The menu system can be used to update various parameters dynamically. One can change the camera position of main window. Additionally, the object rendering can be altered by changing the translation, scale, and rotation vectors associated with displaying the object.
<br>

## Shaders
The data of the vertex shader and fragment shader must be located in a file under the '/shaders' directory. By default, the vertex shader file is called 'vertShader.vert' and the fragment shader is called 'fragShader.frag'. You can specify the name of the file via command line arguments 'f' and '-v'. See Command Line Arugments section for details.
<br>

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l specify the .obj file to load the object from.
  * -c: add this flag to load the object with color. There must be corresponding .mtl file with the same name and location as the .obj file
  * -v: specify name of vertex shader file in '/shaders' directory <br>
  * -f: specify name of fragement shader file in '/shaders' directory <br>

Default behavior can be achieved with the following arguments
```bash
-l objFiles/box.obj
-v vertShader.vert
-f fragShader.frag
```
<br>

Full Usage with Random Colors:
```bash
./Tutorial -l objFiles/box.obj -v vertShader.vert -f fragShader.frag
```
<br>

Extra Credit: Full Usage with Color Loading:
```bash
./Tutorial -l objFiles/box.obj -v vertShader.vert -f fragShader.frag -c
```
<br>

## Exiting
To correctly exit the program, press the ESC key.
