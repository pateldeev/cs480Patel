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
	Menu(const glm::vec3 & eyeLoc, const glm::vec3 & translationVec = { 0.0, 0.0, 0.0 }, const glm::vec3 & scaleVec = { 1.0, 1.0, 1.0 },
			const glm::vec3 & rotationAnglesVec = { 0.0, 0.0, 0.0 });
	~Menu(void);

	bool Initialize(SDL_GLContext & gl_context);
	bool Update(SDL_GLContext & gl_context); //returns if menu has been undated

	void HandleEvent(SDL_Event & event);

	SDL_Window * GetWindow(void);

	glm::vec3 GetEyePosition(void) const;
	glm::vec3 GetTranslationVec(void) const;
	glm::vec3 GetScaleVec(void) const;
	glm::vec3 GetRotationVec(void) const;

private:
	SDL_Window * m_window;

	glm::vec3 m_eyeLoc;
	glm::vec3 m_translation;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;

	//variables bound to menu system
	float mn_eyeLoc[3];
	float mn_translation[3];
	float mn_scale[3];
	float mn_rotation[3];

	void UpdateVariables(void);

	static ImVec4 clear_color;

};

#endif /* MENU_H */
