# PA9: Lighting with Bullet - UNDER DEVELOPMENT

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
./PA09
```
Full Usage with all variables
```bash
mkdir build
cd build
cmake .. -DUSE_COMPLEX_BOARD_MESH=OFF
make
./PA09 -l launch/DefaultConfig.txt
```

## General Comments
This project implements a basic version of collisions as required. If there are any inconsistencies or anything gets stuck, press 'r' to reset. You can only move the ball (Earth) with the WASD keys. The Cube is a dynamic collision object while the cylinder is a static collision object, as required.

## Collision Shapes
All collisions are done via basic collision shapes (plane, sphere, cylinder, cube). We tried to add complex shapes, but it resulted in very choppy motion. You can make the version that loads the full triangles meshes into bullet by using the '-DUSE_COMPLEX_BOARD_MESH=ON' CMake flag. However, we need to find a way to make the meshes smoother for this to work in the future.

## Keyboard Controls
IMPORTANT: For controls to work as intended, ensure the main window (not menu window) is selected. <br>
* Keyboard controls
  * m: toggle (open/close) IMGUI menu window
  * w: add upward impulse to ball
  * s: add downward impulse to ball
  * d: add rightward impulse to ball
  * a: add leftward impulse to ball
  * r: reset all objects
* Exiting
  * ESC: Will properly close all windows and exit program

## Configuration File
This program makes use of configuration files to initialize various parameters. The configuration file can be loaded with the '-l' command line flag. By default, the "launch/DefaultConfig.txt" file is loaded. The data in the file must appear in the order shown. Note that changing certain parameters may cause unintended consequences. Adding new lines/variables may cause errors.

## IMGUI Menu System
There is a menu system that runs in a separate window. The menu system may be opened and closed with the 'm' key. <br>

## Shaders
The GLSL source code for the various vertex and fragment shaders are located under the "shaders/" directory. <br>
The shader sets loaded into the program are specified via the configuration file. See the example file given.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l: load configuration file
