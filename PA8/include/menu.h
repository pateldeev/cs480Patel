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
	Menu(const glm::vec3 & eyeLoc, const glm::vec3 & eyeFocus, const std::string & following);
	~Menu(void);

	bool Initialize(const SDL_GLContext & gl_context);
	bool Update(const SDL_GLContext & gl_context, const glm::vec3 & currrentEyeLoc); //returns if menu has been undated

	void HandleEvent(SDL_Event event);

	SDL_Window * GetWindow(void);

	glm::vec3 GetEyeLocation(void) const;
	glm::vec3 GetEyeFocus(void) const;

	void UpdateEyeParams(const glm::vec3 & eyeLoc, const glm::vec3 & eyeFocus);

	std::string GetFocusPlanet(void) const;
	void SetFocusPlanet(const std::string & planet);

private:
	SDL_Window * m_window;

	glm::vec3 m_eyeLoc;
	glm::vec3 m_eyeFocus;

	//variables bound to menu system
	float mn_eyeLoc[3];
	float mn_eyeFocus[3];

	void UpdateMenuParams(void);

	std::string focus_planet;
};

#endif /* MENU_H */
