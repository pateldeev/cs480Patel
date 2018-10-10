#ifndef ENGINE_H
#define ENGINE_H

#include "menu.h"
#include "window.h"
#include "graphics.h"

class Engine {
public:	
	Engine(const std::string & winName, int winWidth, int winHeight);
	Engine(const std::string & name);
	~Engine(void);
	bool Initialize(const glm::vec3 & eyePos, const std::string & objFile, bool menu);
	void Run(void);

	static long long GetCurrentTimeMillis(void);
	
private:
	// Window related variables
	Window * m_window;
	std::string m_WINDOW_NAME;
	int m_WINDOW_WIDTH;
	int m_WINDOW_HEIGHT;
	SDL_Event m_event;

	Graphics * m_graphics;

	Menu * m_menu;
	long long m_menuLastTime; // last time when menu was created or destoryed

	bool m_running;
	
	void HandleEvent(const SDL_Event & event);
	void EventChecker(void);

	bool StartMenu(const glm::vec3 & eyePos);
	void CloseMenu(void);
	
};

#endif // ENGINE_H
