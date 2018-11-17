# PA2: Keyboard/Mouse Input & Menu Debugging

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
IMPORTANT: For any of the controls to work, the cube window must be selected. Simply click on that window if another one is selected instead <br><br>

Keyboard controls - to control the movement of the cube <br>
  * s: stop all movements - both orbit and rotation <br>
  * c: continue all movements - both orbit and rotation <br>
  * o: toggle orbit on/off <br>
  * r: toggle rotation on/off <br>
  * right arrow: orbit counterclockwise <br>
  * left arrow: orbit clockwise <br>
  * up arrow: rotate right <br>
  * down arrow: rotate left <br><br>
Mouse controls - to change directions <br>
  * left click: reverse orbit direction <br>
  * right click: reverse rotation direction <br><br>
Exiting - press the ESC key during execution to close everything correctly.

## Menu - Extra Credit
I have also implimented an extra credit menu system. It is turned on by default, but can be turned off with the -m command line argument. The menu can be used to reinitalize the program at a different camera position. This is helpful in debugging and viewing the exact movement of the cube. Simply change the coordinates and click the button to change the camera location.
<br>


## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -v: specify name of vertex shader file in '/shaders' directory <br>
  * -f: specify name of fragement shader file in '/shaders' directory <br>
  * -winH: specify window height <br>
  * -winW: specify window width <br>
  * -winN: specify window name <br>
  * -eyeX: specify X-coordinate of camera eye position <br>
  * -eyeY: specify Y-coordinate of camera eye position <br>
  * -eyeZ: specify Z-coordinate of camera eye position <br>
  * -m: specify if menu should be used (0 for false, anything else for true)

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
-m 1
```

Full Usage:
```bash
./Tutorial -v vertShader.vert -f fragShader.frag -winW 1600 -winH 1200 -winN PA02_Deev_Patel -eyeX 0.0 -eyeY 8.0 -eyeZ -16.0 -m 1
```
