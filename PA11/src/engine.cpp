#include "engine.h"

#include <chrono>
#include <assert.h>

Engine::Engine(const std::string & launchFile, float frameCap) :
		m_window(nullptr), m_graphics(nullptr), m_configFile(launchFile), m_shift(false), m_w(false), m_a(false), m_s(false), m_d(false), m_spacebar(
				false), m_leftShift(false), m_captureMouse(true), m_mouseWarp(true), m_menu(nullptr), m_menuSize(0, 0), m_menuLastTime(0), m_dt(0), m_currentTimeMillis(
				Engine::GetCurrentTimeMillis()), m_running(false), m_minFrameTime(1.0f / frameCap * 1000) {
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
	GameInfo game;
	m_configFile.GetGameInfo(game);
	m_graphics = new Graphics(glm::uvec2(m_window->GetWindowWidth(), m_window->GetWindowHeight()), eyePos, eyeLoc, game); //start the graphics

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
		StartMenu(m_graphics->GetEyePos(), m_graphics->GetEyeFocus());
}

Engine::~Engine(void) {
	delete m_menu;
	delete m_graphics;
}

void Engine::Run(void) {
	m_running = true;

	//variables to measure & enforce framerate
	std::chrono::high_resolution_clock::time_point t1, t2;
	float duration;

	while (m_running) {

		t1 = std::chrono::high_resolution_clock::now();
		SDL_GL_MakeCurrent(m_window->GetWindow(), m_window->GetContext());

		EventChecker(); // Check for events input

		// Update and render the graphics
		m_dt = GetDT();

		//check if we need to perform movement
		if (m_w)
			m_graphics->MoveForward(0.01 * m_dt);
		if (m_a)
			m_graphics->MoveLeft(0.01 * m_dt);
		if (m_s)
			m_graphics->MoveBackward(0.01 * m_dt);
		if (m_d)
			m_graphics->MoveRight(0.01 * m_dt);

		if (m_spacebar)
			m_graphics->MoveUp(0.01 * m_dt);
		if (m_leftShift)
			m_graphics->MoveDown(0.01 * m_dt);

		//update graphics worls
		m_graphics->Update(m_dt);
		m_graphics->Render();

		// Swap to the Window
		m_window->Swap();

		//update menu and change variables if necessary
		if (m_menu && m_menu->Update(m_window->GetContext(), m_graphics->GetEyePos(), m_graphics->GetEyeFocus()))
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

		//keep track of right shift key being pressed/released
		if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_RSHIFT))
			m_shift = true;
		else if (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_RSHIFT))
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
					|| event.key.keysym.sym == SDLK_l || event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_s
					|| event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_SPACE
					|| event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_c || event.key.keysym.sym == SDLK_p
					|| event.key.keysym.sym == SDLK_g || event.key.keysym.sym == SDLK_n)
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
			(m_menu) ? CloseMenu() : StartMenu(m_graphics->GetEyePos(), m_graphics->GetEyeFocus());
		else if (event.key.keysym.sym == SDLK_EQUALS)
			m_graphics->ChangeAmbientLight(glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_MINUS)
			m_graphics->ChangeAmbientLight(-glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_m)
			(m_shift) ? m_graphics->ChangeSpecularLight(glm::vec3(0.03, 0.03, 0.03)) : m_graphics->ChangeDiffuseLight(glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_l)
			(m_shift) ? m_graphics->ChangeSpecularLight(-glm::vec3(0.03, 0.03, 0.03)) : m_graphics->ChangeDiffuseLight(-glm::vec3(0.03, 0.03, 0.03));
		else if (event.key.keysym.sym == SDLK_w)
			m_w = true;
		else if (event.key.keysym.sym == SDLK_s)
			m_s = true;
		else if (event.key.keysym.sym == SDLK_d)
			m_d = true;
		else if (event.key.keysym.sym == SDLK_a)
			m_a = true;
		else if (event.key.keysym.sym == SDLK_r)
			m_graphics->Reset();
		else if (event.key.keysym.sym == SDLK_g) {
			if (!m_graphics->IsGenerating() && !m_graphics->IsAutoplaying()) {
				m_graphics->MoveForwardGeneration();
				printf("\nIt is now Player 1 (Blue) turn\n");
				printf("mark 2 of your cells for death & 1 dead cell for life OR 1 opponent cell for death\n");
				printf("press 'p' to progress to next players turn\n");
			}
		} else if (event.key.keysym.sym == SDLK_p)
			m_graphics->ChangePlayer();
		else if (event.key.keysym.sym == SDLK_n) {
			if (!m_graphics->IsAutoplaying())
				m_graphics->ChangeGamemode();
#ifdef DEBUG
			else
			printf("Can't change gamemodes while autoplaying\n");
#endif

		} else if (event.key.keysym.sym == SDLK_SPACE)
			m_spacebar = true;
		else if (event.key.keysym.sym == SDLK_LSHIFT)
			m_leftShift = true;
		else if (event.key.keysym.sym == SDLK_c) { //for turning off mouse movement for selection
			m_captureMouse = !m_captureMouse;
			if (m_captureMouse) {
				SDL_WarpMouseInWindow(nullptr, m_window->GetWindowWidth() / 2, m_window->GetWindowHeight() / 2);
				SDL_SetRelativeMouseMode (SDL_TRUE);
			} else {
				SDL_SetRelativeMouseMode (SDL_FALSE);
				SDL_WarpMouseInWindow(nullptr, m_window->GetWindowWidth() / 2, m_window->GetWindowHeight() / 2);
			}
		} else if (event.key.keysym.sym == SDLK_p) {
			if (!m_graphics->IsGenerating())
				m_graphics->MoveForwardGeneration();
		} else if (event.key.keysym.sym == SDLK_RETURN && !m_graphics->IsMultiplayer()) { //press enter to pause/play in single player
			if (m_graphics->IsAutoplaying())
				m_graphics->StopAutoplay();
			else
				m_graphics->StartAutoplay();
		}
	} else if (event.type == SDL_KEYUP) {
		if (event.key.keysym.sym == SDLK_w)
			m_w = false;
		else if (event.key.keysym.sym == SDLK_s)
			m_s = false;
		else if (event.key.keysym.sym == SDLK_d)
			m_d = false;
		else if (event.key.keysym.sym == SDLK_a)
			m_a = false;
		else if (event.key.keysym.sym == SDLK_SPACE)
			m_spacebar = false;
		else if (event.key.keysym.sym == SDLK_LSHIFT)
			m_leftShift = false;
	} else if (event.type == SDL_MOUSEMOTION && m_captureMouse) {
		if (!m_mouseWarp) {
			m_graphics->RotateCamera(event.motion.xrel, event.motion.yrel);
			m_mouseWarp = true;
			SDL_WarpMouseInWindow(nullptr, m_window->GetWindowWidth() / 2, m_window->GetWindowHeight() / 2);
		} else {
			m_mouseWarp = false;
		}
	} else if (event.type == SDL_MOUSEBUTTONDOWN && !m_graphics->IsGenerating()) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			if (!m_captureMouse)
				m_graphics->LeftClick(glm::vec2((float) event.button.x, (float) event.button.y));
			else
				m_graphics->LeftClick(glm::vec2(m_window->GetWindowWidth() / 2, m_window->GetWindowHeight() / 2));
		}
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
