#include "engine.h"

#include <chrono>
#include <assert.h>

Engine::Engine(const std::string & launchFile) :
		m_window(nullptr), m_graphics(nullptr), m_menu(nullptr), m_menuLastTime(0), m_configFile(launchFile), m_DT(0), m_simulationSpeed(1), m_currentTimeMillis(
				Engine::GetCurrentTimeMillis()), m_running(false) {
	std::srand(time(nullptr));
}

Engine::~Engine(void) {
	delete m_window;
	delete m_graphics;
	delete m_menu;
}

bool Engine::Initialize(void) {

	std::string shaderSrcVert, shaderSrcFrag;
	if (!m_configFile.getShaderFileNames(shaderSrcVert, shaderSrcFrag)) {
		printf("Could not get shader file information from configuration file \n");
		return false;
	}

	bool menu;
	if (!m_configFile.getMenuState(menu)) {
		printf("Could not get menu information from configuration file \n");
		return false;
	}

	std::string windowName;
	if (!m_configFile.getWindowInfo(windowName)) {
		printf("Could not get window information from configuration file \n");
		return false;
	}
	//Start the window
	m_window = new Window();
	if (!m_window->Initialize(windowName)) {
		printf("The window failed to initialize.\n");
		return false;
	}

	glm::vec3 eyePos, eyeLoc;
	if (!m_configFile.getCameraInfo(eyePos, eyeLoc)) {
		printf("Could not get camera information from configuration file \n");
		return false;
	}
	//Start the graphics
	m_graphics = new Graphics();
	if (!m_graphics->Initialize(m_window->GetWindowWidth(), m_window->GetWindowHeight(), shaderSrcVert, shaderSrcFrag, eyePos, eyeLoc)) {
		printf("The graphics failed to initialize.\n");
		return false;
	}

	std::string moonObjFile;
	if (!m_configFile.getMoonObjFile(moonObjFile)) {
		printf("Could not get moon information from configuration file \n");
		return false;
	}

	Planet planet;
	while (m_configFile.getPlanetInfo(planet)) {
		m_graphics->AddPlanet(planet, moonObjFile);
	}

	//Start the menu if necessary
	if (menu)
		StartMenu(m_graphics->GetEyePos(), m_graphics->GetEyeLoc());

	//Start in Whole System View
	m_graphics->SystemView();
	m_menu->SetFocusPlanet("System");

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

		m_DT = getDT();
		// Update and render the graphics
		m_graphics->Update(m_DT * m_simulationSpeed);
		m_graphics->Render();

		// Swap to the Window
		m_window->Swap();

		//update menu and change variables if necessary
		if (m_running != false) {
			if (m_menu && m_menu->Update(m_window->GetContext(), m_graphics->GetEyePos())) {
				m_graphics->SetZoomFlag(false);
				m_graphics->SetViewDistance(10);

				std::string focus_planet = m_menu->GetFocusPlanet();

				if (focus_planet == "UserDefined") {
					m_graphics->UserControlledView();
					if (!m_graphics->UpdateCamera(m_menu->GetEyeLocation(), m_menu->GetEyeFocus())) {
						printf("Error updating parameters from menu update. Shutting down /n");
						m_running = false;
					}
				} else if (focus_planet == "System") {
					m_graphics->SystemView();
				} else {
					m_graphics->FollowPlanet(focus_planet);
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

unsigned int Engine::getDT(void) {
	long long TimeNowMillis = Engine::GetCurrentTimeMillis();
	assert(TimeNowMillis >= m_currentTimeMillis);
	unsigned int DeltaTimeMillis = (unsigned int) (TimeNowMillis - m_currentTimeMillis);
	m_currentTimeMillis = TimeNowMillis;
	return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis() {
	return std::chrono::duration_cast < std::chrono::milliseconds > (std::chrono::system_clock::now().time_since_epoch()).count();
}

void Engine::HandleEvent(const SDL_Event & event) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			m_running = false;
		} else if (event.key.keysym.sym == SDLK_m && m_menuLastTime + 500 < Engine::GetCurrentTimeMillis()) {
			if (m_menu)
				CloseMenu();
			else
				StartMenu(m_graphics->GetEyePos(), m_graphics->GetEyeLoc());
		} else if (event.key.keysym.sym == SDLK_f) {
			m_simulationSpeed += 0.1;
		} else if (event.key.keysym.sym == SDLK_s && m_simulationSpeed >= 0.05) {
			m_simulationSpeed -= 0.1;
		} else if (event.key.keysym.sym == SDLK_i || event.key.keysym.sym == SDLK_o) {

			std::string focusPlanet = (m_menu) ? m_menu->GetFocusPlanet() : "UserDefined";

			if (focusPlanet == "UserDefined" || focusPlanet == "System") {
				glm::vec3 eyeFocus = m_graphics->GetEyeLoc();
				glm::vec3 eyePosition = m_graphics->GetEyePos();
				glm::vec3 newEyePosition = glm::normalize(eyeFocus - eyePosition);
				m_graphics->UserControlledView();

				(event.key.keysym.sym == SDLK_i) ?
						m_graphics->UpdateCamera(eyePosition + newEyePosition, eyeFocus) :
						m_graphics->UpdateCamera(eyePosition - newEyePosition, eyeFocus);
			} else {
				m_graphics->SetZoomFlag(true);
				(event.key.keysym.sym == SDLK_i) ? m_graphics->ZoomCloser() : m_graphics->ZoomAway();
			}
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
			if (m_event.window.event == SDL_WINDOWEVENT_CLOSE
					|| (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_m && m_menuLastTime + 500 < Engine::GetCurrentTimeMillis()))
				CloseMenu();
			else if (m_event.type == SDL_KEYDOWN
					&& (m_event.key.keysym.sym == SDLK_f || m_event.key.keysym.sym == SDLK_s || m_event.key.keysym.sym == SDLK_i
							|| m_event.key.keysym.sym == SDLK_o))
				HandleEvent(m_event); //send certain key controls to main window anyways
			else
				m_menu->HandleEvent(m_event);

		}
	}
}

bool Engine::StartMenu(const glm::vec3 & eyePos, const glm::vec3 & eyeLoc) {
	m_menu = new Menu(eyePos, eyeLoc, m_graphics->GetFollowPlanet());
	if (!m_menu->Initialize(m_window->GetContext())) {
		printf("The imgui menu failed to initialize. Running without it. \n");
		delete m_menu;
		m_menu = nullptr;
		return false;
	}
	m_menuLastTime = Engine::GetCurrentTimeMillis();
	return true;
}

void Engine::CloseMenu(void) {
	delete m_menu;
	m_menu = nullptr;
	m_menuLastTime = Engine::GetCurrentTimeMillis();
}
