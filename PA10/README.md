# PA10: Pinball

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
./Pinball
```

Full Usage with all variables
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_COMPLEX_BOARD_MESH=On
make
./Pinball -l launch/DefaultConfig.txt
```

## General Comments
This is a fully functional pinball game. You are given 3 lives and score 100 points for hitting the cylindirical bumpers and 25 points for hitting the rectangular bumpers at the top. You can launch the ball by pressing 'b'.

## Extra Credit
* Extra spotlight that follows the ball (in addition to two other fixed spotlights on the board). Can be controlled via z/x keys.
* Top 10 scoreboard. Can be seen at bottom of IMGUI menu.

## Keyboard Controls
IMPORTANT: For controls to work as intended, ensure the main window (not menu window) is selected. <br>
* Keyboard Controls
  * Game Contorls
    * b: launch ball and begin game
    * g/h: move left/right bumpers
    * r: reset everything - (in the rare instance of something getting stuck)
  * Camera Movement 
    * left_arrow/right_arrow: move along the x-axis
    * up_arrow/down_arrow: move along the z-axis
    * i/o: zoom toward/away from the focus point
  * Lighting
    * Global
      * f: change to fragment lighting
      * v: change to vertex lighting
      * +/-: adjust ambient lighting
      * z/x: move spotlight up/down with repect to ball
    * For Ball only
      * m/l: adjust diffuse lighting of ball only
      * shift + m/l: adjust specular lighting of ball only
   * Other
      * t: toggle (open/close) IMGUI menu window
   * Exiting
      * ESC: Will properly close all windows and exit program

## Scoring and Gameplay
The player has 3 lives before the game ends. The number of lives and points scored can be seen in the terimnal window as well as IMGUI menu.

## Configuration File
This program makes use of configuration files to initialize various parameters. The configuration file can be loaded with the '-l' command line flag. By default, the "launch/DefaultConfig.txt" file is loaded. The data in the file must appear in the order shown. Note that changing certain parameters may cause unintended consequences. Adding new lines/variables may cause errors.

## IMGUI Menu System
There is a menu system that runs in a separate window. It keeps track of the camera location and focus point. It also keeps track of the score and number of lives <br>

## Shaders
The GLSL source code for the various vertex and fragment shaders are located under the "shaders/" directory. <br>
The shader sets loaded into the program are specified via the configuration file. See the example file given.

## Command Line Arguments
The following command line arguments can be used to change the default behavior of the project
  * -l: load configuration file
