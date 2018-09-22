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

## Extra Credit - Color Loading

## IMGUI menu System

## Shaders
The data of the vertex shader and fragment shader must be located in a file under the '/shaders' directory. By default, the vertex shader file is called 'vertShader.vert' and the fragment shader is called 'fragShader.frag'. You can specify the name of the file via command line arguments, as documented below.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l specify the .obj file to load the object from.
  * -c: add this flag to load the object with color. There must be corresponding .mat file with the same name and location as the .obj file
  * -v: specify name of vertex shader file in '/shaders' directory <br>
  * -f: specify name of fragement shader file in '/shaders' directory <br>

The default behavior can be achieved with the following arguments
```bash
-l objFiles/box.obj
-v vertShader.vert
-f fragShader.frag
```

Full Usage For Random Colors:
```bash
./Tutorial -l objFiles/box.obj -v vertShader.vert -f fragShader.frag
```

Extra Credit: Full Usage Color loading:
```bash
./Tutorial -l objFiles/box.obj -v vertShader.vert -f fragShader.frag -c
```

## Exiting
To correctly exit the program, press the ESC key.
