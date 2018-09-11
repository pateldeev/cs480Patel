#ifndef MENU_H
#define MENU_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>

#include <GL/glew.h>    // Initialize with gl3wInit()

#include <iostream>

class Menu{

public:
Menu(void);
~Menu(void);

bool Initialize(SDL_Window * window, SDL_GLContext & gl_context);
bool Refresh(SDL_GLContext & gl_context);

private:
SDL_Window * m_window;

static ImVec4 clear_color;


};

#endif /* MENU_H */
