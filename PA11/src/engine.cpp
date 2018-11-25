#include "engine.h"

#include <chrono>
#include <assert.h>

Engine::Engine(const std::string & launchFile, float frameCap) :
		m_window(nullptr), m_graphics(nullptr), m_configFile(launchFile), m_shift(false), m_menu(nullptr), m_menuSize(0, 0), m_menuLastTime(0), m_dt(
				0), m_currentTimeMillis(Engine::GetCurrentTimeMillis()), m_running(false), m_minFrameTime(1.0f / frameCap * 1000) {
	std::srand(time(nullptr));

	//get window parameters and start the window
	std::string windowName;
	glm::uvec2 windowSize;
	m_configFile.GetWindowInfo(windowName, windowSize);
	m_window = new Window(windowName, windowSize.x, windowSize.y);

	//get camera information for configuration file
	glm::vec3 eyePos, eyeLoc;
	m_configFile.GetCameraInfo(eyePos, eyeLoc);

	//check if menu needs to be initialized
	bool menu;
	m_configFile.GetMenuState(menu, m_menuSize);

	//get board information from configuration file
	boardInfo * board = m_configFile.GetBoardInfo();
	m_graphics = new Graphics(m_window->GetWindowWidth(), m_window->GetWindowHeight(), eyePos, eyeLoc, *board);	//start the graphics
	delete board;
	board = nullptr;

	//add shader sets and set active one
	std::string shaderSetName, shaderSrcVert, shaderSrcFrag;
	try {
		m_configFile.GetShaderSet(shaderSetName, shaderSrcVert, shaderSrcFrag);
		m_graphics->AddShaderSet(shaderSetName, shaderSrcVert, shaderSrcFrag);
	} catch (bool) { //no more shader sets to parse
	}
	m_configFile.GetShaderSetActive(shaderSetName);
	m_graphics->UseShaderSet(shaderSetName);

	if (menu)
		StartMenu(m_graphics->GetEyePos(), m_graphics->GetEyeLoc());
}

Engine::~Engine(void) {
	delete m_menu;
	delete m_graphics;
}

void Engine::Run(void) {
	m_running = true;

	std::chrono::high_resolution_clock::time_point t1, t2;
	float duration;

	while (m_running) {

		t1 = std::chrono::high_resolution_clock::now();
		SDL_GL_MakeCurrent(m_window->GetWindow(), m_window->GetContext());

		EventChecker(); // Check for events input

		// Update and render the graphics
		m_dt = GetDT();
		m_graphics->Update(m_dt);
		m_graphics->Render();

		// Swap to the Window
		m_window->Swap();

		//update menu and change variables if necessary
		if (m_menu && m_menu->Update(m_window->GetContext(), m_graphics->GetEyePos()))
			m_graphics->UpdateCamera(m_menu->GetEyeLocation(), m_menu->GetEyeFocus());

	}
	//enforce max frame rate
	t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast < std::chrono::milliseconds > (t2 - t1).count();
	if (duration < m_minFrameTime)
		SDL_Delay(m_minFrameTime - duration);
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
			else if (event.key.keysym.sym == SDLK_EQUALS || event.key.keysym.sym == SDLK_MINUS || event.key.keysym.sym == SDLK_m
					|| event.key.keysym.sym == SDLK_l || event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT
					|| event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_i
					|| event.key.keysym.sym == SDLK_o || event.key.keysym.sym == SDLK_w
          || event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_a
          || event.key.keysym.sym == SDLK_d)
				HandleEvent(event);
			else
				m_menu->HandleEvent(event);
		}
	}
}

void Engine::HandleEvent(const SDL_Event & event) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE)
			m_running = false;
		else if (event.key.keysym.sym == SDLK_t && m_menuLastTime + 500 < Engine::GetCurrentTimeMillis())
			(m_menu) ? CloseMenu() : StartMenu(m_graphics->GetEyePos(), m_graphics->GetEyeLoc());
		else if (event.key.keysym.sym == SDLK_EQUALS)
			m_graphics->ChangeAmbientLight(glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_MINUS)
			m_graphics->ChangeAmbientLight(-glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_m)
			(m_shift) ? m_graphics->ChangeSpecularLight(glm::vec3(0.03, 0.03, 0.03)) : m_graphics->ChangeDiffuseLight(glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_l)
			(m_shift) ? m_graphics->ChangeSpecularLight(-glm::vec3(0.03, 0.03, 0.03)) : m_graphics->ChangeDiffuseLight(-glm::vec3(0.03, 0.03, 0.03));
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
    else if (event.key.keysym.sym == SDLK_w)
      m_graphics->MoveForward(0.01 * m_dt);
    else if (event.key.keysym.sym == SDLK_s)
      m_graphics->MoveBackward(0.01 * m_dt);
    else if (event.key.keysym.sym == SDLK_d)
      m_graphics->MoveRight(0.01 * m_dt);
    else if (event.key.keysym.sym == SDLK_a)
      m_graphics->MoveLeft(0.01 * m_dt);
	}
}

bool Engine::StartMenu(const glm::vec3 & eyePos, const glm::vec3 & eyeLoc) {
	try {
		m_menu = new Menu(m_window->GetContext(), eyePos, eyeLoc, m_menuSize);
		m_menuLastTime = Engine::GetCurrentTimeMillis();
		return true;
	} catch (...) {
		printf("The imgui menu failed to initialize. Running without it!");
		delete m_menu;
		m_menu = nullptr;
		return false;
	}
}

bool Engine::CloseMenu(void) {
	delete m_menu;
	m_menu = nullptr;
	m_menuLastTime = Engine::GetCurrentTimeMillis();
	return true;
}
