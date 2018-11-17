#include "engine.h"

#include <chrono>
#include <assert.h>
#include <iostream>

Engine::Engine(const std::string & launchFile) :
		m_window(nullptr), m_graphics(nullptr), m_configFile(launchFile), m_shift(false), m_menu(nullptr), m_menuSize(0, 0), m_menuLastTime(0), m_dt(
				0), m_currentTimeMillis(Engine::GetCurrentTimeMillis()), m_running(false) {
	std::srand(time(nullptr));
}

Engine::~Engine(void) {
	delete m_menu;
	delete m_graphics;
	delete m_window;
}

bool Engine::Initialize(void) {

	//get window parameters and start the window
	std::string windowName;
	glm::uvec2 windowSize;
	if (!m_configFile.GetWindowInfo(windowName, windowSize)) {
		std::cerr << std::endl << "Could not get window information from configuration file!" << std::endl;
		return false;
	}
	m_window = new Window();
	if (!m_window->Initialize(windowName, windowSize.x, windowSize.y)) {
		std::cerr << std::endl << "The window failed to initialize!" << std::endl;
		return false;
	}

	//get camera information for configuration file
	glm::vec3 eyePos, eyeLoc;
	if (!m_configFile.GetCameraInfo(eyePos, eyeLoc)) {
		std::cerr << std::endl << "Could not get camera information from configuration file!" << std::endl;
		return false;
	}

	//start the graphics
	m_graphics = new Graphics();
	if (!m_graphics->Initialize(m_window->GetWindowWidth(), m_window->GetWindowHeight(), eyePos, eyeLoc)) {
		std::cerr << std::endl << "The graphics failed to initialize!" << std::endl;
		return false;
	}

	//add shader sets and set active one
	std::string shaderSetName, shaderSrcVert, shaderSrcFrag;
	while (m_configFile.GetShaderSet(shaderSetName, shaderSrcVert, shaderSrcFrag)) {
		m_graphics->AddShaderSet(shaderSetName, shaderSrcVert, shaderSrcFrag);
	}
	if (!m_configFile.GetShaderSetActive(shaderSetName)) {
		std::cerr << std::endl << "Could not get active shader set from configuration file!" << std::endl;
		return false;
	} else if (!m_graphics->UseShaderSet(shaderSetName)) {
		std::cerr << std::endl << "Could not set active shader in graphics!" << std::endl;
		return false;
	}

	//check if menu needs to be initialized
	bool menu;
	if (!m_configFile.GetMenuState(menu, m_menuSize)) {
		std::cerr << std::endl << "Could not get menu information from configuration file!" << std::endl;
		return false;
	}

	//start bullet
	glm::vec3 gravity;
	if (!m_configFile.GetWorldGravity(gravity)) {
		std::cerr << std::endl << "Could not get gravity information from configuration file!" << std::endl;
		return false;
	}
	if (!m_graphics->InitializeBt(gravity)) {
		std::cerr << std::endl << "The graphics failed to initialize bullet!" << std::endl;
		return false;
	}

	//get lighting info
	glm::vec3 ambientLevel;
	float shininess;
	std::vector < glm::vec3 > spotlightLocs;
	if (!m_configFile.GetLightingInfo(ambientLevel, shininess, spotlightLocs)) {
		std::cerr << std::endl << "Could not get lighting info from configuration file!" << std::endl;
		return false;
	}
	m_graphics->InitializeLighting(ambientLevel, shininess);
	for (const glm::vec3 & loc : spotlightLocs)
		m_graphics->AddSpotLight(loc);

	//add objects from configuration file
	std::vector < objectModel > objects;
	if (!m_configFile.GetObjects(objects)) {
		std::cerr << std::endl << "Could not get objects from configuration file!" << std::endl;
		return false;
	}
	for (const objectModel & obj : objects)
		m_graphics->AddObject(obj);

	//check if all main objects are loaded
	if (!m_graphics->VerifyObjects()) {
		std::cerr << std::endl << "Not all objects were loaded. Check configuration file!" << std::endl;
		return false;
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
	const float FPS = 500; //FPS cap, should obviously add a way to change this later
	const float minFrameTime = 1.0f / FPS * 1000;
	float duration;

	while (m_running) {

		t1 = std::chrono::high_resolution_clock::now();
		SDL_GL_MakeCurrent(m_window->GetWindow(), m_window->GetContext());

		EventChecker(); // Check for events input

		m_dt = GetDT();
		// Update and render the graphics
		m_graphics->Update(m_dt);
		m_graphics->Render();

		//End the game if lives == 0
		if (m_graphics->GetLives() < 1) {
			int score = m_graphics->GetScore();

			delete m_menu;
			delete m_graphics;
			delete m_window;
			m_window = nullptr;
			m_graphics = nullptr;
			m_menu = nullptr;

			std::cout << std::endl << "Game Over: You Scored: " << score << " points!" << std::endl;
			m_running = false;
			break;
		}
		// Swap to the Window
		m_window->Swap();

		//update menu and change variables if necessary
		if (m_running != false) {
			if (m_menu && m_menu->Update(m_window->GetContext(), m_graphics->GetEyePos(), m_graphics->GetLives(), m_graphics->GetScore())) {
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

unsigned int Engine::GetDT(void) {
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
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_QUIT) { //only works in no-menu mode
			m_running = false;
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			m_running = false;
		}

		//keep track of shift key being pressed/released
		if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_RSHIFT || event.key.keysym.sym == SDLK_LSHIFT))
			m_shift = true;
		else if (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_RSHIFT || event.key.keysym.sym == SDLK_LSHIFT))
			m_shift = false;

		//handle event based on correct window location
		if (event.window.windowID == SDL_GetWindowID(m_window->GetWindow())) {
			if (event.window.event == SDL_WINDOWEVENT_CLOSE) { //quits if main window is closed
				m_running = false;
			} else {
				HandleEvent(event);
			}
		} else if (m_menu && event.window.windowID == SDL_GetWindowID(m_menu->GetWindow())) {
			if (event.window.event == SDL_WINDOWEVENT_CLOSE
					|| (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_t && m_menuLastTime + 500 < Engine::GetCurrentTimeMillis()))
				CloseMenu();
			else if (event.key.keysym.sym == SDLK_r || event.key.keysym.sym == SDLK_EQUALS || event.key.keysym.sym == SDLK_MINUS
					|| event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_DOWN
					|| event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_i || event.key.keysym.sym == SDLK_o
					|| event.key.keysym.sym == SDLK_f || event.key.keysym.sym == SDLK_v || event.key.keysym.sym == SDLK_m
					|| event.key.keysym.sym == SDLK_l || event.key.keysym.sym == SDLK_z || event.key.keysym.sym == SDLK_x
					|| event.key.keysym.sym == SDLK_g || event.key.keysym.sym == SDLK_h || event.key.keysym.sym == SDLK_b)
				HandleEvent(event);
			else
				m_menu->HandleEvent(event);
		}
	}
}

void Engine::HandleEvent(const SDL_Event & event) {
	const int impulse = 30;

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE)
			m_running = false;
		else if (event.key.keysym.sym == SDLK_t && m_menuLastTime + 500 < Engine::GetCurrentTimeMillis())
			(m_menu) ? CloseMenu() : StartMenu(m_graphics->GetEyePos(), m_graphics->GetEyeLoc());
		else if (event.key.keysym.sym == SDLK_r)
			m_graphics->ResetBall();
		else if (event.key.keysym.sym == SDLK_EQUALS)
			m_graphics->SetAmbientLight(glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_MINUS)
			m_graphics->SetAmbientLight(-glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_m)
			(m_shift) ? m_graphics->SetSpecularofBall(glm::vec3(0.03, 0.03, 0.03)) : m_graphics->SetDiffuseofBall(glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_l)
			(m_shift) ? m_graphics->SetSpecularofBall(-glm::vec3(0.03, 0.03, 0.03)) : m_graphics->SetDiffuseofBall(-glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_z)
			m_graphics->SetSpotlightHeight(0.1);
		else if (event.key.keysym.sym == SDLK_x)
			m_graphics->SetSpotlightHeight(-0.1);
		else if (event.key.keysym.sym == SDLK_RIGHT)
			m_graphics->IncreaseEyePosX(1.0);
		else if (event.key.keysym.sym == SDLK_LEFT)
			m_graphics->DecreaseEyePosX(1.0);
		else if (event.key.keysym.sym == SDLK_UP)
			m_graphics->IncreaseEyePosZ(1.0);
		else if (event.key.keysym.sym == SDLK_DOWN)
			m_graphics->DecreaseEyePosZ(1.0);
		else if (event.key.keysym.sym == SDLK_i)
			m_graphics->ZoomIn(1.0);
		else if (event.key.keysym.sym == SDLK_o)
			m_graphics->ZoomOut(1.0);
		else if (event.key.keysym.sym == SDLK_f)
			m_graphics->UseShaderSet("fragmentLighting");
		else if (event.key.keysym.sym == SDLK_v)
			m_graphics->UseShaderSet("vertexLighting");
		else if (event.key.keysym.sym == SDLK_h)
			m_graphics->MovePaddleR(m_dt);
		else if (event.key.keysym.sym == SDLK_g)
			m_graphics->MovePaddleL(m_dt);
		else if (event.key.keysym.sym == SDLK_b)
			m_graphics->StartLife();
	} else if (event.type == SDL_KEYUP) {
		if (event.key.keysym.sym == SDLK_g)
			m_graphics->SetResetFlagPaddleL(true);
		else if (event.key.keysym.sym == SDLK_h)
			m_graphics->SetResetFlagPaddleR(true);

	}
}

bool Engine::StartMenu(const glm::vec3 & eyePos, const glm::vec3 & eyeLoc) {
	m_menu = new Menu(eyePos, eyeLoc, m_menuSize);
	if (!m_menu->Initialize(m_window->GetContext())) {
		std::cerr << std::endl << "The imgui menu failed to initialize. Running without it!" << std::endl;
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
