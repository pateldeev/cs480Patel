#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

class Window {
public:
	Window(void) = delete;
	Window(const std::string & name, unsigned int height, unsigned int width);
	~Window(void);

	//Window is not meant to be copied or moved
	Window(const Window &) = delete;
	Window(Window &&) = delete;
	Window& operator=(const Window &) = delete;
	Window& operator=(Window&&) = delete;

	void Swap(void);

	SDL_Window * GetWindow(void);
	SDL_GLContext & GetContext(void);

	unsigned int GetWindowHeight(void) const;
	unsigned int GetWindowWidth(void) const;

private:
	SDL_Window * m_gWindow;
	SDL_GLContext m_gContext;

	unsigned int m_height;
	unsigned int m_width;
};

#endif /* WINDOW_H */
