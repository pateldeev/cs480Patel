#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

class Window {
public:
	Window(void);
	~Window(void);
	bool Initialize(const std::string & name, unsigned int height, unsigned int width);
	void Swap(void);

	SDL_Window * GetWindow(void);
	SDL_GLContext & GetContext(void);

	unsigned int GetWindowHeight(void) const;
	unsigned int GetWindowWidth(void) const;

private:
	SDL_Window * gWindow;
	SDL_GLContext gContext;

	unsigned int m_height;
	unsigned int m_width;
};

#endif /* WINDOW_H */
