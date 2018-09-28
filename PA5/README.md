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
This program can load any object from a correctly formatted .obj file. By default the "objFiles/box.obj" file is loaded. This can be changed using the '-l' command line flag. See Command Line Arugments section for details. <br>
Note: The object that is loaded is colored randomly unless otherwise specified.

## Extra Credit - Color Loading
I also implimented the extra credit color loading via materials. By adding a '-c' flag at runtime, the program will look for a .mtl with the same name as the .obj file to load materials from. It will then load the color version if possible. <br>
The default box object can be loaded in color with the '-c' flag.

## IMGUI Menu System
There is a menu system that runs in a seperate window. The menu system can be used to update various parameters related to object rendering.

## Shaders
The data of the vertex shader and fragment shader must be located in a file under the '/shaders/' directory. <br>
By default, the vertex shader file is called 'vertShader.vert' and the fragment shader is called 'fragShader.frag'. You can specify the name of the file via command line arguments 'f' and '-v'. See Command Line Arugments section for details.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l: specify the .obj file to load the object from.
  * -c: add this flag to load the object with color. There must be corresponding .mtl file with the same name and location as the .obj file.
  * -v: specify name of vertex shader file in '/shaders/' directory <br>
  * -f: specify name of fragement shader file in '/shaders/' directory <br>

The default behavior can be achieved with the following arguments
```bash
-l objFiles/box.obj
-v vertShader.vert
-f fragShader.frag
```

Full Usage with Random Colors:
```bash
./Tutorial -l objFiles/box.obj -v vertShader.vert -f fragShader.frag
```

Extra Credit: Full Usage with Color Loading:
```bash
./Tutorial -l objFiles/box.obj -v vertShader.vert -f fragShader.frag -c
```

## Exiting
To correctly exit the program, press the ESC key.
