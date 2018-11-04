#include "engine.h"

#include <chrono>
#include <assert.h>

Engine::Engine(const std::string & launchFile) :
		m_window(nullptr), m_graphics(nullptr), m_menu(nullptr), m_menuLastTime(0), m_configFile(launchFile), m_DT(0), m_currentTimeMillis(
				Engine::GetCurrentTimeMillis()), m_running(false) {
	std::srand(time(nullptr));
}

Engine::~Engine(void) {
	delete m_window;
	delete m_graphics;
	delete m_menu;
}

bool Engine::Initialize(void) {

	std::string windowName;
	glm::uvec2 windowSize;
	if (!m_configFile.getWindowInfo(windowName, windowSize)) {
		printf("Could not get window information from configuration file \n");
		return false;
	}
	//start the window
	m_window = new Window();
	if (!m_window->Initialize(windowName, windowSize.x, windowSize.y)) {
		printf("The window failed to initialize.\n");
		return false;
	}

	glm::vec3 eyePos, eyeLoc;
	if (!m_configFile.getCameraInfo(eyePos, eyeLoc)) {
		printf("Could not get camera information from configuration file \n");
		return false;
	}
	//start the graphics
	m_graphics = new Graphics();
	if (!m_graphics->Initialize(m_window->GetWindowWidth(), m_window->GetWindowHeight(), eyePos, eyeLoc)) {
		printf("The graphics failed to initialize.\n");
		return false;
	}

	//add shader sets
	std::string shaderSetName, shaderSrcVert, shaderSrcFrag;
	while (m_configFile.getShaderSet(shaderSetName, shaderSrcVert, shaderSrcFrag)) {
		m_graphics->AddShaderSet(shaderSetName, shaderSrcVert, shaderSrcFrag);
	}
	m_graphics->UseShaderSet("passThroughTexture");

	//check if menu needs to be initialized
	bool menu;
	if (!m_configFile.getMenuState(menu, m_menuSize)) {
		printf("Could not get menu information from configuration file \n");
		return false;
	}

	//start bullet
	glm::vec3 gravity;
	if (!m_configFile.getWorldGravity(gravity)) {
		printf("Could not get gravity information from configuration file \n");
		return false;
	}
	if (!m_graphics->InitializeBt(gravity)) {
		printf("The graphics failed to initialize bullet.\n");
		return false;
	}

	//add objects from configuration file
	objectModel obj;
	while (m_configFile.getObject(obj)) {
		m_graphics->AddObject(obj, obj.name == "Ball");
	}

	//Start the menu if necessary
	if (menu)
		StartMenu(m_graphics->GetEyePos(), m_graphics->GetEyeLoc());

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
		m_graphics->Update(m_DT);
		m_graphics->Render();

		// Swap to the Window
		m_window->Swap();

		//update menu and change variables if necessary
		if (m_running != false) {
			if (m_menu && m_menu->Update(m_window->GetContext(), m_graphics->GetEyePos())) {
				if (!m_graphics->UpdateCamera(m_menu->GetEyeLocation(), m_menu->GetEyeFocus())) {
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
			else if (m_event.key.keysym.sym == SDLK_w || m_event.key.keysym.sym == SDLK_s || m_event.key.keysym.sym == SDLK_d
					|| m_event.key.keysym.sym == SDLK_a || m_event.key.keysym.sym == SDLK_r) {
				HandleEvent(m_event);
			} else
				m_menu->HandleEvent(m_event);
		}
	}
}

void Engine::HandleEvent(const SDL_Event & event) {

#define USEIMPULSE 1 //have arrow keys apply impulse

#if USEIMPULSE
	const int impulse = 30;
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			m_running = false;
		} else if (event.key.keysym.sym == SDLK_m && m_menuLastTime + 500 < Engine::GetCurrentTimeMillis()) {
			(m_menu) ? CloseMenu() : StartMenu(m_graphics->GetEyePos(), m_graphics->GetEyeLoc());
		} else if (event.key.keysym.sym == SDLK_w) {
			m_graphics->ApplyImpulse(glm::vec3(0, 0, -impulse), glm::vec3(0, 0, 0));
		} else if (event.key.keysym.sym == SDLK_s) {
			m_graphics->ApplyImpulse(glm::vec3(0, 0, impulse), glm::vec3(0, 0, 0));
		} else if (event.key.keysym.sym == SDLK_a) {
			m_graphics->ApplyImpulse(glm::vec3(-impulse, 0, 0), glm::vec3(0, 0, 0));
		} else if (event.key.keysym.sym == SDLK_d) {
			m_graphics->ApplyImpulse(glm::vec3(impulse, 0, 0), glm::vec3(0, 0, 0));
		} else if (event.key.keysym.sym == SDLK_r) {
			m_graphics->ResetObjects();
		}
	}

#else //have arrow keys change linear velocity
	const float velocity = 23;

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			m_running = false;
		} else if (event.key.keysym.sym == SDLK_m && m_menuLastTime + 500 < Engine::GetCurrentTimeMillis()) {
			(m_menu) ? CloseMenu() : StartMenu(m_graphics->GetEyePos(), m_graphics->GetEyeLoc());
		} else if (event.key.keysym.sym == SDLK_w) {
			m_graphics->SetLinearVelocity(glm::vec3(0, 0, -velocity));
		} else if (event.key.keysym.sym == SDLK_s) {
			m_graphics->SetLinearVelocity(glm::vec3(0, 0, velocity));
		} else if (event.key.keysym.sym == SDLK_a) {
			m_graphics->SetLinearVelocity(glm::vec3(-velocity, 0, 0));
		} else if (event.key.keysym.sym == SDLK_d) {
			m_graphics->SetLinearVelocity(glm::vec3(velocity, 0, 0));
		} else if (event.key.keysym.sym == SDLK_r) {
			m_graphics->ResetObjects();
		}
	}
#endif
}

bool Engine::StartMenu(const glm::vec3 & eyePos, const glm::vec3 & eyeLoc) {
	m_menu = new Menu(eyePos, eyeLoc, m_menuSize);
	if (!m_menu->Initialize(m_window->GetContext())) {
		printf("The imgui menu failed to initialize. Running without it. \n");
		delete m_menu;
		m_menu = nullptr;
		return false;
	}
	m_menuLastTime = Engine::GetCurrentTimeMillis();
	return true;
}

bool Engine::CloseMenu(void) {
	delete m_menu;
	m_menu = nullptr;
	m_menuLastTime = Engine::GetCurrentTimeMillis();
	return true;
}
