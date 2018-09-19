# PA4: Object Loader

## Dependency Instructions
To run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3.

### Ubuntu/Linux
```bash 
 sudo apt-get install libglew-dev libglm-dev libsdl2-dev
```

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
This program can load any object in given the correct obj file. The file can specified using the '-l' command line flag. See the Command Line Arugments section for more details. Note: The object that is loaded is colored randomly.

## Shaders
The data of the vertex shader and fragment shader must be located in a file under the '/shaders' directory. By default, the vertex shader file is called 'vertShader.vert' and the fragment shader is called 'fragShader.frag'. You can specify the name of the file via command line arguments, as documented below.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l specify the obj file to load the object from.
  * -v: specify name of vertex shader file in '/shaders' directory <br>
  * -f: specify name of fragement shader file in '/shaders' directory <br>
  * -eyeX: specify X-coordinate of camera eye position <br>
  * -eyeY: specify Y-coordinate of camera eye position <br>
  * -eyeZ: specify Z-coordinate of camera eye position <br>

The default behavior can be achieved with the following arguments
```bash
-l objFiles/box.obj
-v vertShader.vert
-f fragShader.frag
-eyeX 0.0
-eyeY 8.0
-eyeZ -16.0 
```

Full Usage:
```bash
./Tutorial -l objFiles/box.obj -v vertShader.vert -f fragShader.frag -eyeX 0.0 -eyeY 8.0 -eyeZ -16.0
```
