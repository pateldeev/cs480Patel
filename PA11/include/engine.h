#ifndef ENGINE_H
#define ENGINE_H

#include "menu.h"
#include "window.h"
#include "graphics.h"
#include "configLoader.hpp"

class Engine {
public:
	Engine(void) = delete;
	Engine(const std::string & launchFile, float frameCap = 100);
	~Engine(void);

	//Engine is not meant to be copied or moved
	Engine(const Engine &) = delete;
	Engine(Engine &&) = delete;
	Engine& operator=(const Engine &) = delete;
	Engine& operator=(Engine&&) = delete;

	void Run(void);

private:
	unsigned int GetDT(void);
	static long long GetCurrentTimeMillis(void);

	void EventChecker(void);
	void HandleEvent(const SDL_Event & event);

	bool StartMenu(const glm::vec3 & eyePos, const glm::vec3 & eyeLoc);
	bool CloseMenu(void);

	Window * m_window;
	Graphics * m_graphics;
	ConfigFileParser m_configFile; //used to get parameters from

	bool m_shift; //keeps track of shift being pressed

	bool m_w; // W key
	bool m_a; // A key
	bool m_s; // S key
	bool m_d; // D key

	bool m_spacebar; // Space Bar Key
	bool m_leftShift; // Left Shift Key

	bool m_captureMouse;
	bool m_mouseWarp;

	Menu * m_menu;
	glm::uvec2 m_menuSize;
	long long m_menuLastTime; // last time when menu was created or destoryed

	unsigned int m_dt;
	long long m_currentTimeMillis;
	bool m_running;

	const float m_minFrameTime; //for max frame rate
};

#endif // ENGINE_H
