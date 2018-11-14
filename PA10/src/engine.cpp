#include "engine.h"

#include <chrono>
#include <assert.h>

Engine::Engine(const std::string & launchFile) :
		m_window(nullptr), m_graphics(nullptr), m_menu(nullptr), m_menuLastTime(0), m_configFile(launchFile), m_DT(0), m_currentTimeMillis(
				Engine::GetCurrentTimeMillis()), m_running(false), m_shift(false) {
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
	if (!m_configFile.getShaderSetActive(shaderSetName)) {
		printf("Could not get active shader set from configuration file \n");
		return false;
	}
	m_graphics->UseShaderSet(shaderSetName);

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

	//get lighting info
	glm::vec3 ambientLevel;
	float shininess;
	std::vector < glm::vec3 > spotlightLocs;
	if (!m_configFile.getLightingInfo(ambientLevel, shininess, spotlightLocs)) {
		printf("Could not get lighting info from configuration file \n");
		return false;
	}
	m_graphics->InitializeLighting(ambientLevel, shininess);
	for (const glm::vec3 & loc : spotlightLocs)
		m_graphics->AddSpotLight(loc);

	//add objects from configuration file
	std::vector < objectModel > objects;
	if (!m_configFile.getObjects(objects)) {
		printf("Could not get objects from configuration file \n");
		return false;
	}
	for (const objectModel & obj : objects)
		m_graphics->AddObject(obj);

	//check if all main objects are loaded
	if (!m_graphics->VerifyObjects()) {
		printf("Not all objects were loaded. Check configuration file!");
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

		//End the game if lives == 0
		if (m_graphics->GetLives() < 1) {
			printf("\n Game Over: You Scored: %d points! \n", m_graphics->GetScore());
			m_running = false;
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

		//keep track of shift key being pressed/released
		if (m_event.type == SDL_KEYDOWN && (m_event.key.keysym.sym == SDLK_RSHIFT || m_event.key.keysym.sym == SDLK_LSHIFT))
			m_shift = true;
		else if (m_event.type == SDL_KEYUP && (m_event.key.keysym.sym == SDLK_RSHIFT || m_event.key.keysym.sym == SDLK_LSHIFT))
			m_shift = false;

		//handle event based on correct window location
		if (m_event.window.windowID == SDL_GetWindowID(m_window->GetWindow())) {
			if (m_event.window.event == SDL_WINDOWEVENT_CLOSE) { //quits if main window is closed
				m_running = false;
			} else {
				HandleEvent(m_event);
			}
		} else if (m_menu && m_event.window.windowID == SDL_GetWindowID(m_menu->GetWindow())) {
			if (m_event.window.event == SDL_WINDOWEVENT_CLOSE
					|| (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_t && m_menuLastTime + 500 < Engine::GetCurrentTimeMillis()))
				CloseMenu();
			else if (m_event.key.keysym.sym == SDLK_r || m_event.key.keysym.sym == SDLK_EQUALS || m_event.key.keysym.sym == SDLK_MINUS
					|| m_event.key.keysym.sym == SDLK_RIGHT || m_event.key.keysym.sym == SDLK_LEFT || m_event.key.keysym.sym == SDLK_DOWN
					|| m_event.key.keysym.sym == SDLK_UP || m_event.key.keysym.sym == SDLK_i || m_event.key.keysym.sym == SDLK_o
					|| m_event.key.keysym.sym == SDLK_f || m_event.key.keysym.sym == SDLK_v || m_event.key.keysym.sym == SDLK_m
					|| m_event.key.keysym.sym == SDLK_l || m_event.key.keysym.sym == SDLK_z || m_event.key.keysym.sym == SDLK_x
					|| m_event.key.keysym.sym == SDLK_g || m_event.key.keysym.sym == SDLK_h || m_event.key.keysym.sym == SDLK_b)
				HandleEvent(m_event);
			else
				m_menu->HandleEvent(m_event);
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
		else if (m_event.key.keysym.sym == SDLK_z)
			m_graphics->SetSpotlightHeight(0.1);
		else if (m_event.key.keysym.sym == SDLK_x)
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
			m_graphics->MovePaddleR();
		else if (event.key.keysym.sym == SDLK_g)
			m_graphics->MovePaddleL();
		else if (event.key.keysym.sym == SDLK_b)
			m_graphics->StartLife();
	} else if (event.type == SDL_KEYUP) {
		if (event.key.keysym.sym == SDLK_h)
			m_graphics->SetResetFlagPaddleR(true);
		if (event.key.keysym.sym == SDLK_g)
			m_graphics->SetResetFlagPaddleL(true);
	}
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
