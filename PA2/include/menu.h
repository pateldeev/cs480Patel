#ifndef MENU_H
#define MENU_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>

#include <GL/glew.h>    // Initialize with gl3wInit()

#include "graphics.h"

//struct for changable parameters
struct params {
	std::string vertexFile, fragmentFile, winName;
	bool menu;
	int winWidth, winHeight;
	glm::vec3 eyePos;

	//default parameters
	params() :
			vertexFile("shaders/vertShader.vert"), fragmentFile("shaders/fragShader.frag"), winName("PA02_Deev_Patel"), menu(true), winWidth(1600), winHeight(
					1200), eyePos(0.0, 8.0, -16.0) {
	}
};

class Menu {

public:
	Menu(const glm::vec3 & camPos);
	~Menu(void);

	bool Initialize(SDL_GLContext & gl_context);
	bool Update(SDL_GLContext & gl_context); //returns if menu has been undated

	SDL_Window * GetWindow(void);
	glm::vec3 GetNewPosition(void) const;

private:
	SDL_Window * m_window;
	glm::vec3 m_camLoc;
	
	
	static ImVec4 clear_color;

};

#endif /* MENU_H */
