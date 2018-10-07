#include "engine.h"

#include <chrono>
#include <assert.h>

Engine::Engine(const std::string & winName, int winWidth, int winHeight) :
		m_window(nullptr), m_graphics(nullptr), m_menu(nullptr) {
	m_WINDOW_NAME = winName;
	m_WINDOW_WIDTH = winWidth;
	m_WINDOW_HEIGHT = winHeight;
}

Engine::~Engine(void) {
	delete m_window;
	delete m_graphics;
	delete m_menu;
}

bool Engine::Initialize(const glm::vec3 & eyePos, const std::string & objFile, bool menu) {
	//Start the window
	m_window = new Window();
	if (!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT)) {
		printf("The window failed to initialize.\n");
		return false;
	}

	//Start the graphics
	m_graphics = new Graphics();
	if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, eyePos, objFile)) {
		printf("The graphics failed to initialize.\n");
		return false;
	}

	//Start the menu if necessary
	if (menu) {
		StartMenu(m_graphics->GetEyePos());
	}

	// No errors
	return true;
}

void Engine::Run(void) {
	m_running = true;

	std::chrono::high_resolution_clock::time_point t1, t2;
	const float FPS = 120; //FPS cap, should obviously add a way to change this later
	const float minFrameTime = 1.0f / FPS * 1000;
	float duration;

	while (m_running) {

		t1 = std::chrono::high_resolution_clock::now();
		SDL_GL_MakeCurrent(m_window->GetWindow(), m_window->GetContext());

		EventChecker(); // Check for events input

		// Update and render the graphics
		m_graphics->Update();
		m_graphics->Render();

		// Swap to the Window
		m_window->Swap();

		//update menu and change variables if necessary
		if (m_running != false) {
			if (m_menu && m_menu->Update(m_window->GetContext())) {
				if (!m_graphics->UpdateParameters(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, m_menu->GetEyePosition(), m_menu->GetTranslationVec(),
						m_menu->GetScaleVec(), m_menu->GetRotationVec())) {
					printf("Error updating parameters from menu update. Shutting down /n");
					m_running = false;
				}
			}
		}

		//enforce max frame rate
		t2 = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast < std::chrono::milliseconds > (t2 - t1).count();
		if (duration < minFrameTime)
			SDL_Delay(minFrameTime - duration);
	}
}

void Engine::HandleEvent(const SDL_Event & event) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			m_running = false;
		} else if (event.key.keysym.sym == SDLK_m) {
			if (m_menu)
				CloseMenu();
			else
				StartMenu(m_graphics->GetEyePos());
		}
	}
}

void Engine::EventChecker(void) {
	while (SDL_PollEvent (&m_event)) {

		if (m_event.type == SDL_QUIT) { //only works in no-menu mode
			m_running = false;
		}

		if (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_ESCAPE) {
			m_running = false;
		}

		//handle event based on correct window location
		if (m_event.window.windowID == SDL_GetWindowID(m_window->GetWindow())) {
			if (m_event.window.event == SDL_WINDOWEVENT_CLOSE) { //quits if main window is closed
				m_running = false;
			} else {
				HandleEvent(m_event);
			}
		} else if (m_menu && m_event.window.windowID == SDL_GetWindowID(m_menu->GetWindow())) {
			if (m_event.window.event == SDL_WINDOWEVENT_CLOSE) {
				CloseMenu();
			} else {
				m_menu->HandleEvent(m_event);
			}
		}
	}
}

bool Engine::StartMenu(const glm::vec3 & eyePos) {
	m_menu = new Menu(eyePos);
	if (!m_menu->Initialize(m_window->GetContext())) {
		printf("The imgui menu failed to initialize. Running without it. \n");
		m_menu = nullptr;
		return false;
	}
	return true;
}

void Engine::CloseMenu(void) {
	delete m_menu;
	m_menu = nullptr;
}

