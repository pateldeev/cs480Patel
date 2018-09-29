#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"
#include "menu.h"

class Engine {
public:
	Engine(const std::string & winName, int winWidth, int winHeight);
	Engine(const std::string & name);
	~Engine(void);
	bool Initialize(const glm::vec3 & eyePos, const std::string & objFile, bool menu, bool readColor);
	void Run(void);
	void Keyboard(const SDL_Event & event);

private:
	// Window related variables
	Window * m_window;
	std::string m_WINDOW_NAME;
	int m_WINDOW_WIDTH;
	int m_WINDOW_HEIGHT;
	SDL_Event m_event;

	Graphics * m_graphics;
	
	Menu * m_menu;

	bool m_running;

};

#endif // ENGINE_H
