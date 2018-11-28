#ifndef MENU_H
#define MENU_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#include <SDL.h>
#include <GL/glew.h>    // Initialize with gl3wInit()

#include "graphics.h"
#include <iostream>

class Menu {

public:
	Menu(const SDL_GLContext & gl_context, const glm::vec3 & eyeLoc, const glm::vec3 & eyeFocus, const glm::uvec2 & menuSize);
	~Menu(void);

	//Menu is not meant to be copied or moved
	Menu(const Menu &) = delete;
	Menu(Menu &&) = delete;
	Menu& operator=(const Menu &) = delete;
	Menu& operator=(Menu&&) = delete;

	bool Update(const SDL_GLContext & gl_context, const glm::vec3 & currrentEyeLoc, const glm::vec3 & currentEyeFocus); //returns true if menu has been undated

	void HandleEvent(SDL_Event event);

	SDL_Window * GetWindow(void);

	glm::vec3 GetEyeLocation(void) const;
	glm::vec3 GetEyeFocus(void) const;
	void UpdateEyeParams(const glm::vec3 & eyeLoc, const glm::vec3 & eyeFocus);

private:
	void UpdateMenuParams(void);

	SDL_Window * m_window;

	glm::vec3 m_eyeLoc;
	glm::vec3 m_eyeFocus;

	glm::uvec2 m_menuSize;
	glm::uvec2 m_menuTL; //top left point of window

	//variables bound to menu system
	float mn_eyeLoc[3];
	float mn_eyeFocus[3];
};

#endif /* MENU_H */
