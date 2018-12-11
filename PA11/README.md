# PA11: Final Project - UNDER DEVELOPMENT

## Group
This is the main repository for the code. The group members include
  * Jordan Andrieu
  * Deev Patel
  * Braeden Richards

## Dependency Instructions
This project uses OpenGL 3.3. Newer versions should work, but nothing is guaranteed. <br>
In addition to OpenGL, this project requires installation of the following libaries:
 * [ASSIMP](https://github.com/assimp/assimp/wiki)
 * [Bullet](https://github.com/bulletphysics/bullet3)
 * [GLEW](http://glew.sourceforge.net/)
 * [GLM](http://glm.g-truc.net/0.9.7/index.html)
 * [Magick++](http://www.imagemagick.org/Magick%2B%2B/)
 * [SDL2](https://wiki.libsdl.org/Tutorials)

### Ubuntu 18.04
```bash
 sudo apt-get install libassimp-dev libbullet-dev libglew-dev libglm-dev libmagick++-dev libsdl2-dev
```

## Building and Running - CMake Instructions
The building of the project is done using CMake.

Quick Usage with defaults
```bash
mkdir build
cd build
cmake ..
make
./PA11_FinalProject
```

Full Usage with all variables
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./PA11_FinalProject -l launch/DefaultConfig.txt
```

## General Comments & Features
* Movement
* Raycasting with bullet for clicking

## Keyboard Controls
IMPORTANT: For controls to work as intended, ensure the main window (not menu window) is selected. <br>
* Keyboard Controls
  * Game Controls
    * c: switch to selection mode - can how click on cubes
  * Camera Movement
    * w: move forward
    * s: move backward
    * a: move left
    * d: move right
    * space: move up
    * l-Shift: move down
    * mouse: Look around (left/right/up/down) without moving your position
  * Lighting - Global
      * +/-: adjust ambient lighting
   * Other
      * t: toggle (open/close) IMGUI menu window
   * Exiting
      * ESC: Will properly close all windows and exit program

## Gameplay

## Configuration File
This program makes use of configuration files to initialize various parameters. The configuration file can be loaded with the '-l' command line flag. By default, the "launch/DefaultConfig.txt" file is loaded. The data in the file must appear in the order shown. Note that changing certain parameters may cause unintended consequences. Adding new lines/variables may cause errors.

## IMGUI Menu System
There is a menu system that runs in a separate window. It keeps track of the camera location and focus point.<br>

## Shaders
The GLSL source code for the various vertex and fragment shaders are located under the "shaders/" directory. <br>
The shader sets loaded into the program are specified via the configuration file. See the example file given.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l: load configuration file
