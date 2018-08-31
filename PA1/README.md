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
The data of the vertex shader and fragment shader should be located in a file under the '/shaders' directory. By default, the vertex shader file is called 'vertShader.vert' and the fragment shader is 'fragShader.frag'. To change this default behavior, you can pass the names of files as command line argument. The file must still be located in the '/shaders' subdirectory. Below is an example of the default behavior.

```bash
./Tutorial vertexFile=vertShader.vert fragmentFile=fragShader.frag
```
