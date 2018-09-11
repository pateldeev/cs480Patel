# PA2: Keyboard Input

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
The movement of the planet is all done in the Object::Update(unsigned int) function of the Object class. By default, the planet has an orbit radius of 4.0 units.

## Keyboard & Mouse Controls - For movement control
The following keyboard keys can be used to control the movement of the cube <br>
  s: stop all movements again - both orbit and rotation <br>
  c: continue all movements - both orbit and rotation <br>
  o: toggle orbit on/off <br>
  r: toggle rotation on/off <br>
<br>
  right arrow: orbit clockwise <br>
  left arrow: orbit counterclockwise <br>
  up arrow: rotate right <br>
  down arrow: rotate left <br> 
<br><br>
The orbit and rotation can also be changed with the mouse. 
  Left click: reverse orbit direction
  Right click reverse rotation direction.

## Extra Credit - Menu
I tried to impliment the extra credit menu system, but was only partially successful. I managed to build the imgui source code and run the sample menu they gave. However, when I 
put it on top of my cube window, I got a problem with updating the windows. It kept flickering as both the cube and menu were updating too fast. I couldn't figure it out in time. You can run the program with the partially working menu system with the -m command line argument. Just run ./Tutorial -m. However, the menu system is nowhere near complete.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  -v: specify name of vertex shader file in '/shaders' directory <br>
  -f: specify name of fragement shader file in '/shaders' directory <br>
  -winH: specify window height <br>
  -winW: specify window width <br>
  -winN: specify window name <br>
  -eyeX: specify X-coordinate of camera eye position <br>
  -eyeY: specify Y-coordinate of camera eye position <br>
  -eyeZ: specify Z-coordinate of camera eye position <br>

The default behavior can be achieved with the following arguments
```bash
-v vertShader.vert
-f fragShader.frag
-winW 1600
-winH 1200
-winN PA02_Deev_Patel
-eyeX 0.0
-eyeY 8.0
-eyeZ -16.0 
```

Full Usage:
```bash
./Tutorial -v vertShader.vert -f fragShader.frag -winW 1600 -winH 1200 -winN PA02_Deev_Patel -eyeX 0.0 -eyeY 8.0 -eyeZ -16.0 
```
