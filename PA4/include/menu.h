#ifndef MENU_H
#define MENU_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>

#include <GL/glew.h>    // Initialize with gl3wInit()

#include "graphics.h"

class Menu {

public:
	Menu(const glm::vec3 & camPos);
	~Menu(void);

	bool Initialize(SDL_GLContext & gl_context);
	bool Update(SDL_GLContext & gl_context); //returns if menu has been undated

	void HandleEvent(SDL_Event & event);

	SDL_Window * GetWindow(void);
	glm::vec3 GetEyePosition(void) const;

private:
	SDL_Window * m_window;

	glm::vec3 m_eyeLoc;
	glm::vec3 m_translationMat;
	glm::vec3 m_scaleMat;
	glm::vec3 m_rotationMat;

	//variables bound to menu system
	float mn_eyeLoc[3];
	float mn_translationMat[3];
	float mn_scaleMat[3];
	float mn_rotationMatp3[3];

		
	static ImVec4 clear_color;

};

#endif /* MENU_H */
