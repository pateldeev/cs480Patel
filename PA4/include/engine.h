#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"

class Engine {
public:
	Engine(const std::string & winName, int winWidth, int winHeight);
	Engine(const std::string & name);
	~Engine(void);
	bool Initialize(const glm::vec3 & eyePos, const std::string & objFile);
	void Run(void);
	void Keyboard(void);

private:
	// Window related variables
	Window * m_window;
	std::string m_WINDOW_NAME;
	int m_WINDOW_WIDTH;
	int m_WINDOW_HEIGHT;
	SDL_Event m_event;

	Graphics * m_graphics;

	bool m_running;

};

#endif // ENGINE_H
