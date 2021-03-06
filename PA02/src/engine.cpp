#include "engine.h"

Engine::Engine(const std::string & name, int width, int height) {
	m_WINDOW_NAME = name;
	m_WINDOW_WIDTH = width;
	m_WINDOW_HEIGHT = height;
	m_FULLSCREEN = false;
}

Engine::Engine(const std::string & name) {
	m_WINDOW_NAME = name;
	m_WINDOW_HEIGHT = 0;
	m_WINDOW_WIDTH = 0;
	m_FULLSCREEN = true;
}

Engine::~Engine(void) {
	delete m_window;
	delete m_graphics;
	delete m_menu;
}

bool Engine::Initialize(const glm::vec3 & eyePos, bool initMenu) {
	// Start a window
	m_window = new Window();
	if (!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT)) {
		printf("The window failed to initialize.\n");
		return false;
	}

	// Start the graphics
	m_graphics = new Graphics();
	if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, eyePos)) {
		printf("The graphics failed to initialize.\n");
		return false;
	}

	// Start imgui menu if necessasry
	if (initMenu) {
		m_menu = new Menu(eyePos);
		if (!m_menu->Initialize(m_window->GetContext())) {
			printf("The imgui menu failed to initialize.\n");
			return false;
		}
	} else {
		m_menu = nullptr;
	}

	// Set the time
	m_currentTimeMillis = GetCurrentTimeMillis();

	// No errors
	return true;
}

void Engine::Run(void) {
	m_running = true;

	while (m_running) {

		SDL_GL_MakeCurrent(m_window->GetWindow(), m_window->GetContext());

		// Update the DT
		m_DT = getDT();

		// Check the keyboard input
		while (SDL_PollEvent(&m_event) != 0) {
			Keyboard();
		}

		// Update and render the graphics
		m_graphics->Update(m_DT);
		m_graphics->Render();

		// Swap to the Window
		m_window->Swap();

		//update camera position if necessary
		if (m_menu && m_menu->Update(m_window->GetContext())) {
			delete m_graphics;
			m_graphics = nullptr;
			m_graphics = new Graphics();
			if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, m_menu->GetNewPosition())) {
				printf("The graphics failed to change camera position.\n");
				m_running = false;
				break;
			}
		}
	}
}

void Engine::Keyboard(void) {

	if (m_event.type == SDL_QUIT) {
		m_running = false;
	}

	//check for events on cube window
	if (m_event.window.windowID == SDL_GetWindowID(m_window->GetWindow())) {
		if (m_event.type == SDL_KEYDOWN) {
			if (m_event.key.keysym.sym == SDLK_ESCAPE) {
				m_running = false;
			} else {
				if (!m_graphics->handleEvent(m_event)) {
					printf("Unhandeled SDL key press: %d \n", m_event.key.keysym.sym);
				}
			}
		} else {
			m_graphics->handleEvent(m_event);
		}
	}

	//check for events on menu window if necessasry
	else if (m_menu && m_event.window.windowID == SDL_GetWindowID(m_menu->GetWindow())) {
		if (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_ESCAPE) {
			delete m_menu;
			m_menu = nullptr;
		}
	}
}

unsigned int Engine::getDT(void) {
	long long TimeNowMillis = GetCurrentTimeMillis();
	assert(TimeNowMillis >= m_currentTimeMillis);
	unsigned int DeltaTimeMillis = (unsigned int) (TimeNowMillis - m_currentTimeMillis);
	m_currentTimeMillis = TimeNowMillis;
	return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis(void) {
	timeval t;
	gettimeofday(&t, NULL);
	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
}
