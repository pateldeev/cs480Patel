#ifndef ENGINE_H
#define ENGINE_H

#include "menu.h"
#include "window.h"
#include "graphics.h"

#include "configLoader.hpp"

class Engine {
public:	
	Engine(const std::string & launchFile);
	~Engine(void);
	bool Initialize(void);
	void Run(void);

	unsigned int getDT(void);
	
	static long long GetCurrentTimeMillis(void);
	
private:
	Window * m_window;
	Graphics * m_graphics;

	SDL_Event m_event;

	Menu * m_menu;
	long long m_menuLastTime; // last time when menu was created or destoryed
	
	ConfigFileParser m_configFile; //used to get parameters from

	unsigned int m_DT;
	float m_simulationSpeed;
	long long m_currentTimeMillis;
	bool m_running;
	
	void HandleEvent(const SDL_Event & event);
	void EventChecker(void);

	bool StartMenu(const glm::vec3 & eyePos, const glm::vec3 & eyeLoc);
	void CloseMenu(void);
	
};

#endif // ENGINE_H
