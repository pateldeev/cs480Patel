#include "engine.h"

Engine::Engine(const std::string & winName, int winWidth, int winHeight) {
	m_WINDOW_NAME = winName;
	m_WINDOW_WIDTH = winWidth;
	m_WINDOW_HEIGHT = winHeight;
}

Engine::~Engine(void) {
	delete m_window;
	delete m_graphics;
}

bool Engine::Initialize(const glm::vec3 & eyePos, const std::string & objFile) {
	// Start a window
	m_window = new Window();
	if (!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT)) {
		printf("The window failed to initialize.\n");
		return false;
	}

	// Start the graphics
	m_graphics = new Graphics();
	if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, eyePos, objFile)) {
		printf("The graphics failed to initialize.\n");
		return false;
	}

	// No errors
	return true;
}

void Engine::Run(void) {
	m_running = true;

	while (m_running) {

		SDL_GL_MakeCurrent(m_window->GetWindow(), m_window->GetContext());

		// Check the keyboard input
		while (SDL_PollEvent(&m_event) != 0) {
			Keyboard();
		}

		// Update and render the graphics
		m_graphics->Update();
		m_graphics->Render();

		// Swap to the Window
		m_window->Swap();

	}
}

void Engine::Keyboard(void) {

	if (m_event.type == SDL_QUIT) {
		m_running = false;
	}

	if (m_event.type == SDL_KEYDOWN) {
		if (m_event.key.keysym.sym == SDLK_ESCAPE) {
			m_running = false;
		}
	}
}
