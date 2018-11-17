# PA5: Assimp Object Loader

## Group
This is the main repository for the code. The group members include
  * Jordan Andrieu
  * Deev Patel
  * Braeden Richards

## Dependency Instructions
To run this project installation of four programs is required: [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), [SDL2](https://wiki.libsdl.org/Tutorials), and [assimp](https://github.com/assimp/assimp/wiki).
This project uses OpenGL 3.3.

### Ubuntu/Linux
```bash 
 sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev

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
This program can load any object from a correctly formatted .obj file. By default the "objFiles/box.obj" file is loaded. This can be changed using the '-l' command line flag. See Command Line Arguments section for details. <br>
Note: The object that is loaded is colored randomly unless otherwise specified.

## Extra Credit - Color Loading
We also implimented the extra credit color loading via materials. By adding the '-c 1' flag at runtime, the program will look for a .mtl with the same name as the .obj file to load materials from. The .mtl file must be there for color loading to work. <br>
The default box object can be loaded in color with the '-c 1' flag.

## IMGUI Menu System
There is a menu system that runs in a seperate window. The menu system can be used to update various parameters related to object rendering. <br>
This can be turned off using the '-m 0' command line flags. See Command Line Arguments section for deatils.

## Shaders
The GLSL source code for the vertex and fragment shaders must be located under the "shaders/" directory. <br>
By default, the vertex shader code is in "vertShader.vert" and the fragment shader code is in "fragShader.frag". You can specify the name of the files via the '-f' and '-v' flags. See Command Line Arugments section for details.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l: specify the name of .obj file to load the object from.
  * -v: specify file with source code of vertex shader in "shaders/" directory
  * -f: specify file with source code of fragement shader in "shaders/" directory
  * -m: specify usage of menu (0 for false, 1 for true - any other argument may cause undefined behavior) - enabled by default.
  * -c: specify color loading (0 for false, 1 for true - any other argument may cause undefined behavior) - disabled by default

The default behavior can be achieved with the following arguments
```bash
-l objFiles/box.obj
-v vertShader.vert
-f fragShader.frag
-m 1
-c 0
```

Full Usage with Random Colors:
```bash
./Tutorial -l objFiles/box.obj -v vertShader.vert -f fragShader.frag -m 1 -c 0
```

Extra Credit: Full Usage with Color Loading:
```bash
./Tutorial -l objFiles/box.obj -v vertShader.vert -f fragShader.frag -m 1 -c 1
```

## Exiting
To correctly exit the program, press the ESC key.
