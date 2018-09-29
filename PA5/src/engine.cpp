#include "engine.h"

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

bool Engine::Initialize(const glm::vec3 & eyePos, const std::string & objFile, bool menu, bool readColor) {
	//Start the window
	m_window = new Window();
	if (!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT)) {
		printf("The window failed to initialize.\n");
		return false;
	}

	//Start the graphics
	m_graphics = new Graphics();
	if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, eyePos, objFile, readColor)) {
		printf("The graphics failed to initialize.\n");
		return false;
	}

	//Start the menu if necessary
	if (menu) {
		m_menu = new Menu(eyePos);
		if (!m_menu->Initialize(m_window->GetContext())) {
			printf("The imgui menu failed to initialize. Running without it. \n");
			m_menu = nullptr;
		}
	}

	// No errors
	return true;
}

void Engine::Run(void) {
	m_running = true;

	while (m_running) {

		SDL_GL_MakeCurrent(m_window->GetWindow(), m_window->GetContext());

		// Check for events input
		while (SDL_PollEvent (&m_event)) {

			if (m_event.type == SDL_QUIT) { //only works in no-menu mode
				m_running = false;
			}

			if (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_ESCAPE) {
				m_running = false;
			}

			//handle event based on correct window location
			if (m_event.window.windowID == SDL_GetWindowID(m_window->GetWindow())) {
                            if(m_event.window.event == SDL_WINDOWEVENT_CLOSE) { //quits if main window is closed
                                m_running = false;
                            } else {
				Keyboard(m_event);
                            }
			} else if (m_menu && m_event.window.windowID == SDL_GetWindowID(m_menu->GetWindow())) {
                            if(m_event.window.event == SDL_WINDOWEVENT_CLOSE) {
                                delete m_menu;
                                m_menu = nullptr;
                            } else {
                                m_menu->HandleEvent(m_event);

                            }
			}

		}

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
    }
}

void Engine::Keyboard(const SDL_Event & event) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			m_running = false;
		}
	}
}
