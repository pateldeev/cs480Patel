# PA7: Solar System - UNDER DEVELOPMENT

## TODO
  * Camera movement/reset view
  * Create sun, planets, moons - possibly more planets with rings for EC
  * Implement accurate orbit and rotation speeds
  * Accurate scaling of distance - planets vs sun
  * Possibly have configuration file with parameters - file name, orbit/rotation speeds, scaling, position
  * Instruction Manual
  * Extra: Adjust simulation speeds, drawing orbit, going to scaled view

## Group
This is the main repository for the code. The group members include
  * Jordan Andrieu
  * Deev Patel
  * Braeden Richards

## Dependency Instructions
To run this project installation for the following is required: [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), [SDL2](https://wiki.libsdl.org/Tutorials), [assimp](https://github.com/assimp/assimp/wiki), and [magick++](http://www.imagemagick.org/Magick%2B%2B/).
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
./Solar_System
```

## Configuration File - Extra Credit
This program makes use of configuration files to specifcy the characteristics of the solar system. The configuration file can be loaded with the '-l' command line flag. By default, the "launch/DefaultConfig.txt" file is loaded. The data in the file must appear in the order shown. You should only change the values. Adding new lines/variables cause errors.

## IMGUI Menu System
There is a menu system that runs in a separate window. The menu system can be used to update various parameters related to object rendering. <br>
This can be turned off via the configuration file. See the example file given.

## Shaders
The GLSL source code for the vertex and fragment shaders must be located under the "shaders/" directory. <br>
By default, the vertex shader code is in "vertShader.vert" and the fragment shader code is in "fragShader.frag". You can specify the name of the files via the configuration. See the example file given.

## Keyboard & Mouse Controls
IMPORTANT: For any of the controls to work, the main window (not menu window) must be selected. <br>
* Keyboard controls
  * m: toggle (open/close) IMGUI menu window
* Exiting
  * ESC: Will properly close all windows and exit program

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l: load configuration file

The default behavior can be achieved with the following arguments
```bash
-l launch/DefaultConfig.txt
```

Full Usage
```bash
./Solar_System -l launch/DefaultConfig.txt
```
