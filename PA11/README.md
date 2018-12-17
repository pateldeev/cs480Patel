# PA11: Final Project - 3-D multiplayer derivation of Conway's Game of Life

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
This project is a 2-player adaptation of Conway’s Game of Life. Notably, it is played on a 3-D board that surrounds the players on all sides. Notably the project makes use of instancing, ray casting, and realistic movement controls to create a usable 2-player game. The automatic simulation mode is done with multithreading.

## Keyboard Controls
IMPORTANT: For controls to work as intended, ensure the main window (not menu window) is selected. <br>
* Keyboard Controls
  * Game Controls
    * c: switch to selection mode - can now click on cubes
    * g: move one generation forward
    * p: switch between player 1's and player 2's turns
    * enter/return: begin autoplay mode (only works while in single player)
    * n: switch between a multiplayer and singleplayer game
    * r: reset the board  * Camera Movement
    * w/s/a/d: move forward/backward/left/right
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
For the most part, the game’s progression is dictated by the 4 rules of Conway's Game of Life:
* Death by underpopulation
  * Any live cell with fewer than two live neighbors dies
* Continuation of life
  * Any live cell with two or three live neighbors lives on to the next generation
* Death by overpopulation
  * Any live cell with more than three live neighbors dies
* Reproduction
  * Any dead cell with exactly three live neighbors becomes a live cell
<br>
However, since this is a multiplayer game, any cell that comes to life takes the dominant color of its neighbors. So, if a cell is surrounded by two cells of Player 1 and one cell of Player 2, it will belong to Player 1 at the end of the round. Additionally, before each round, players are given the ability to either (1) Mark 2 of their own cells for death & mark 1 dead cell for life or (2) Mark 1 opposing cell for destruction

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
