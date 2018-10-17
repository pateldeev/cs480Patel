#include <window.h>

Window::Window(void) :
		gWindow(nullptr), m_height(0), m_width(0) {
}

Window::~Window(void) {
	SDL_StopTextInput();
	SDL_DestroyWindow (gWindow);
	SDL_Quit();
}

bool Window::Initialize(const std::string & name) {
	// Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL failed to initialize: %s\n", SDL_GetError());
		return false;
	}

	// Start OpenGL for SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	// Create window
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	//use for fullscreen
	SDL_DisplayMode display;
	SDL_GetDesktopDisplayMode(0, &display);
	m_height = display.h;
	m_width = display.w;

	gWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!gWindow) {
		printf("Widow failed to create: %s\n", SDL_GetError());
		return false;
	}

	//SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);

	// Create context
	gContext = SDL_GL_CreateContext(gWindow);
	if (!gContext) {
		printf("OpenGL context not created: %s\n", SDL_GetError());
		return false;
	}

	// Use VSync
	if (SDL_GL_SetSwapInterval(1) < 0) {
		printf("Unable to use VSync: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

void Window::Swap(void) {
	SDL_GL_SwapWindow (gWindow);
}

SDL_Window * Window::GetWindow(void) {
	return gWindow;
}

SDL_GLContext & Window::GetContext(void) {
	return gContext;
}

int Window::GetWindowHeight(void) const{
	return m_height;
}

int Window::GetWindowWidth(void) const{
	return m_width;
}
