# PA1: Spinning Cube

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
```

### Mac OSX
Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.
```bash
brew install glew glm sdl2
```

## Building and Running - CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary.

```bash
mkdir build
cd build
cmake ..
make
./Tutorial
```

## Shaders
The data of the vertex shader and fragment shader must be located in a file under the '/shaders' directory. By default, the vertex shader file is called 'vertShader.vert' and the fragment shader is called 'fragShader.frag'. You can specify the name of the file via command line arguments, as documented below.

## Orbit/Rotation
The movement of the planet is all done in the Update(unsigned int) function of the object class. By default, the planet has an orbit radius of 4.0 units.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project

-v: specify name of vertex shader file in '/shaders' directory
-f: specify name of fragement shader file in '/shaders' directory
-winH: specify window height
-winW: specify window width
-winN: specify window name
-eyeX: specify X-coordinate of camera eye position
-eyeY: specify Y-coordinate of camera eye position
-eyeZ: specify Z-coordinate of camera eye position

The default behavior can be achieved with the following arguments
```bash
-v vertShader.vert
-f fragShader.frag
-winW 1600
-winH 1200
-winN PA01_Deev_Patel
-eyeX 0.0
-eyeY 8.0
-eyeZ -16.0 
```

Full Usage:
```bash
./Tutorial -v vertShader.vert -f fragShader.frag -winW 1600 -winH 1200 -winN PA01_Deev_Patel -eyeX 0.0 -eyeY 8.0 -eyeZ -16.0 
```
