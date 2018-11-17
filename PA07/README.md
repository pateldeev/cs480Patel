# PA7: Solar System

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
This program makes use of configuration files to specify the characteristics of the solar system. It contains all the information about the relative size and orbits of all the planets and moons. The configuration file can be loaded with the '-l' command line flag. By default, the "launch/DefaultConfig.txt" file is loaded. The data in the file must appear in the order shown. You should only change the values. Adding new lines/variables may cause errors.

## Simulation Speed - Extra Credit
The program offers the ability to change the simulation speed of the solar system. Pressing and holding down 'f' makes the speed faster while pressing and holding down 's' will make the speed slower. The solar system can be stopped entirely by holding down 's' for long enough.

## Planets & Rings - Extra Credit
The solar system includes all planets, including Pluto. Each planet can be focused on directly from the IMGUI menu. The orbits of other planets can seem strange from the perspective of a specific planet because the orbits are elliptical. Jupiter, Saturn, Neptune, & Uranus have rings. Some of the larger planets have many moons. Since planets such as Jupiter have a large number of moons, we only showed the major ones.

## Keyboard Controls
IMPORTANT: For controls to work as intended, ensure the main window (not menu window) is selected. <br>
* Keyboard controls
  * i: zoom inwards towards the focus point
  * o: zoom outwards away from the focus point
  * f: make the simulation speed faster
  * s: make the simulation speed slower
  * m: toggle (open/close) IMGUI menu window
* Exiting
  * ESC: Will properly close all windows and exit program

## IMGUI Menu System
There is a menu system that runs in a separate window. The menu system can be used to help move around in the solar system. <br>

## Shaders
The GLSL source code for the vertex and fragment shaders must be located under the "shaders/" directory. <br>
By default, the vertex shader code is in "vertShader.vert" and the fragment shader code is in "fragShader.frag". You can specify the name of the files via the configuration. See the example file given.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l: load configuration file.
The default behavior can be achieved with the following arguments
```bash
-l launch/DefaultConfig.txt
```

Full Usage
```bash
./Solar_System -l launch/DefaultConfig.txt
```
