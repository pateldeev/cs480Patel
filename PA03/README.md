# PA3: Moon & Text

## Dependency Instructions
To run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3.

### Ubuntu/Linux
```bash 
 sudo apt-get install libglew-dev libglm-dev libsdl2-dev
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

## Shaders
The data of the vertex shader and fragment shader must be located in a file under the '/shaders' directory. By default, the vertex shader file is called 'vertShader.vert' and the fragment shader is called 'fragShader.frag'. You can specify the name of the file via command line arguments, as documented below.

## Elliptical Orbits & Rotation
The movement of the planet and moon is all done in the Object::Update(unsigned int) function of the Object class. The planet has an elliptical orbit with a major radius of 6.25 units and a minor radius of 3.75 around the origin. The moon has an elliptical orbit with a major radius of 3.1 and a minor radius of 2.9 units around the planet.

## Keyboard & Mouse Controls - For movement control of Planet & Moon
IMPORTANT: For any of the controls to work, the cube window must be selected. Simply click on that window if another one is selected instead <br><br>

* Keyboard controls - to control the movement of the cube <br>
  * s: stop all movements - both orbit and rotation <br>
  * c: continue all movements - both orbit and rotation <br>
  * o: toggle orbit on/off <br>
  * r: toggle rotation on/off <br>
  * rightArrow: orbit counterclockwise <br>
  * leftArrow: orbit clockwise <br>
  * upArrow: rotate right <br>
  * downArrow: rotate left <br>
* Mouse controls - to change directions of cube <br>
  * leftClick: reverse orbit direction <br>
  * rightClick: reverse rotation direction <br>
* Exiting <br>
  * ESC: Will properly close all windows
<br>
You can also control the moon's movement by pressing any of the above keys in conjuction with the left shift key. For instace: <br>
&nbsp;&nbsp;&nbsp; lShift + s: stops rotation and orbit of moon <br>
&nbsp;&nbsp;&nbsp; lShift + leftClick: reverse orbit direction of moon <br>

## Menu
I implimented an extra credit menu system. It is turned on by default, but can be turned off with the -m command line argument. The menu can be used to reinitalize the program at a different camera position. This is helpful in debugging and viewing the exact movement of the cube. Simply change the coordinates and click the button to change the camera location.

## Text output of direction - Extra Credit
I implimented the extra credit text output. In anticipation of the next assignment, I created all the possibilties in Blender and wrote a simple OBJ file loader. The OBJ files of the texts are located in the "objFiles" directory. They must be there for the text to be rendered. The text output is sort of limited in that I hardcoded the rendering position. As such, the text will be best visible when run with the default window size and camera position. Given more time, I would have liked to create a system that could render the text dynamically for any specified camera position and window.

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
-winN PA03_Deev_Patel
-eyeX 0.0
-eyeY 8.0
-eyeZ -16.0 
-m 1
```

Full Usage:
```bash
./Tutorial -v vertShader.vert -f fragShader.frag -winW 1600 -winH 1200 -winN PA03_Deev_Patel -eyeX 0.0 -eyeY 8.0 -eyeZ -16.0 -m 1
```
