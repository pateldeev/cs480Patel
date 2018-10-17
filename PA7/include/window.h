#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

class Window {
public:
	Window(void);
	~Window(void);
	bool Initialize(const std::string & name);
	void Swap(void);

	SDL_Window * GetWindow(void);
	SDL_GLContext & GetContext(void);

	int GetWindowHeight(void) const;
	int GetWindowWidth(void) const;
	
private:
	SDL_Window * gWindow;
	SDL_GLContext gContext;
	
	int m_height;
	int m_width;
};

#endif /* WINDOW_H */
