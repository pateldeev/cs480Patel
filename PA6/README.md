# PA6: Object & Texture Loader - STILL DEVELOPING

## Group
This is the main repository for the code. The group members include
  * Jordan Andrieu
  * Deev Patel
  * Braeden Richards

## Dependency Instructions
To run this project installation for the following is required: [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), [SDL2](https://wiki.libsdl.org/Tutorials), [assimp](https://github.com/assimp/assimp/wiki), and [magic](http://www.imagemagick.org/Magick%2B%2B/).
This project uses OpenGL 3.3.

### Ubuntu 18.04
```bash
 sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev libmagick++-dev
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
This program can load any object from a correctly formatted .obj file. By default the "objFiles/budda.obj" file is loaded. This can be changed using the '-l' command line flag. See Command Line Arguments section for details. <br>
Note: The object that is assumed to have an associated .mtl file unless otherwise specified.

## Textures
All objects being loaded must have a texture. If the obj file does not have texture coordiantes, the object will not be loaded. The texture files must be contained in the same directory as the .obj and .mtl files.

## IMGUI Menu System
There is a menu system that runs in a separate window. The menu system can be used to update various parameters related to object rendering. <br>
This can be turned off using the '-m 0' command line flags. See Command Line Arguments section for details.

## Shaders
The GLSL source code for the vertex and fragment shaders must be located under the "shaders/" directory. <br>
By default, the vertex shader code is in "vertShader.vert" and the fragment shader code is in "fragShader.frag". You can specify the name of the files via the '-f' and '-v' flags. See Command Line Arguments section for details.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l: specify the name of .obj file to load the object from.
  * -v: specify file with source code of vertex shader in "shaders/" directory
  * -f: specify file with source code of fragment shader in "shaders/" directory
  * -m: specify usage of menu (0 for false, 1 for true - any other argument may cause undefined behavior) - enabled by default.

The default behavior can be achieved with the following arguments
```bash
-l objFiles/box.obj
-v vertShader.vert
-f fragShader.frag
-m 1
```

Full Usage - must have Textures
```bash
./Tutorial -l objFiles/box.obj -v vertShader.vert -f fragShader.frag -m 1
```

## Exiting
To correctly exit the program, press the ESC key.
