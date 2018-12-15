#include <window.h>

Window::Window(const std::string & name, unsigned int height, unsigned int width) :
		m_gWindow(nullptr) {

	// Start SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::string errMsg = "SDL failed to initialize. Window object not created properly: ";
		errMsg += SDL_GetError();
		throw errMsg;
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
	m_height = (height) ? height : display.h;
	m_width = (width) ? width : display.w;

	m_gWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_CAPTURE);
	if (!m_gWindow) {
		std::string errMsg = "Widow failed to create. Window object not created properly: ";
		errMsg += SDL_GetError();
		throw errMsg;
	}

	// Create context
	m_gContext = SDL_GL_CreateContext(m_gWindow);
	if (!m_gContext) {
		std::string errMsg = "OpenGL context not created. Window object not created properly: ";
		errMsg += SDL_GetError();
		throw errMsg;
	}

	// Use VSync
	if (SDL_GL_SetSwapInterval(1) < 0) {
		std::string errMsg = "Unable to use VSync. Window object not created properly: ";
		errMsg += SDL_GetError();
		throw errMsg;
	}
	SDL_WarpMouseInWindow(nullptr, m_width / 2, m_height / 2);
	SDL_SetRelativeMouseMode (SDL_TRUE);
}

Window::~Window(void) {
	SDL_StopTextInput();
	SDL_DestroyWindow (m_gWindow);
	SDL_Quit();
}

void Window::Swap(void) {
	SDL_GL_SwapWindow (m_gWindow);
}

SDL_Window * Window::GetWindow(void) {
	return m_gWindow;
}

SDL_GLContext & Window::GetContext(void) {
	return m_gContext;
}

unsigned int Window::GetWindowHeight(void) const {
	return m_height;
}

unsigned int Window::GetWindowWidth(void) const {
	return m_width;
}
