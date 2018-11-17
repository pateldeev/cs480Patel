#include "menu.h"

Menu::Menu(const glm::vec3 & eyeLoc, const glm::vec3 & eyeFocus, const std::string & following) :
		m_window(nullptr), m_eyeLoc(eyeLoc), m_eyeFocus(eyeFocus), focus_planet(following) {
	UpdateMenuParams();
}

Menu::~Menu(void) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyWindow (m_window);
}

bool Menu::Initialize(const SDL_GLContext & gl_context) {

	m_window = SDL_CreateWindow("Movement_Menu", 50, 50, 600, 400, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	const char * glsl_version = "#version 330";

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(m_window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsDark();

	return true;

}

bool Menu::Update(const SDL_GLContext & gl_context, const glm::vec3 & currrentEyeLoc) {

	bool updated = false;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame (m_window);
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
	ImGui::SetNextWindowSize(ImVec2(575, 375));
	ImGui::Begin("Menu");

	ImGui::Text("Use this menu to move the camera around or follow a planet.");
	ImGui::Text("\n");

	ImGui::InputFloat3("Camera Position", mn_eyeLoc);
	ImGui::Text("Current Camera Position: (%.2f, %.2f, %.2f)", currrentEyeLoc.x, currrentEyeLoc.y, currrentEyeLoc.z);
	ImGui::Text("\n");

	ImGui::InputFloat3("Camera Focus", mn_eyeFocus);
	ImGui::Text("Current Camera Focus: (%.2f, %.2f, %.2f)", m_eyeFocus.x, m_eyeFocus.y, m_eyeFocus.z);
	ImGui::Text("\n");
	if (ImGui::Button("Update")) {
		updated = true;
		focus_planet = "UserDefined";
		m_eyeLoc = glm::vec3(mn_eyeLoc[0], mn_eyeLoc[1], mn_eyeLoc[2]);
		m_eyeFocus = glm::vec3(mn_eyeFocus[0], mn_eyeFocus[1], mn_eyeFocus[2]);
	}

	ImGui::Text("\n");
	ImGui::Text("Chosen focus for camera: ");
	ImGui::SameLine();
	if (ImGui::CollapsingHeader(focus_planet.empty() ? "No focus selected" : focus_planet.c_str())) {
		if (ImGui::Button("System")) {
			focus_planet = "System";
			updated = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Mercury")) {
			focus_planet = "Mercury";
			updated = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Venus")) {
			focus_planet = "Venus";
			updated = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Earth")) {
			focus_planet = "Earth";
			updated = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Mars")) {
			focus_planet = "Mars";
			updated = true;
		}
		if (ImGui::Button("Jupiter")) {
			focus_planet = "Jupiter";
			updated = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Saturn")) {
			focus_planet = "Saturn";
			updated = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Uranus")) {
			focus_planet = "Uranus";
			updated = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Neptune")) {
			focus_planet = "Neptune";
			updated = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Pluto")) {
			focus_planet = "Pluto";
			updated = true;
		}
	}

	ImGui::Text("\n");
	ImGui::Text("Note: You can make the simulation slower/faster with 's'/'f'  keys \n      You can zoom in/out with the 'i'/'o' keys. \n      You can close this menu with the 'm' key");
	ImGui::Text("\n");

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	// Rendering
	ImGui::Render();
	SDL_GL_MakeCurrent(m_window, gl_context);
	glClearColor(0.0, 0.0, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(m_window);

	return updated;
}

void Menu::HandleEvent(SDL_Event event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

SDL_Window * Menu::GetWindow(void) {
	return m_window;
}

glm::vec3 Menu::GetEyeLocation(void) const {
	return m_eyeLoc;
}

glm::vec3 Menu::GetEyeFocus(void) const {
	return m_eyeFocus;
}

void Menu::UpdateEyeParams(const glm::vec3 & eyeLoc, const glm::vec3 & eyeFocus) {
	m_eyeLoc = eyeLoc;
	m_eyeFocus = eyeFocus;
}

void Menu::UpdateMenuParams(void) {
	mn_eyeLoc[0] = m_eyeLoc.x;
	mn_eyeLoc[1] = m_eyeLoc.y;
	mn_eyeLoc[2] = m_eyeLoc.z;

	mn_eyeFocus[0] = m_eyeFocus.x;
	mn_eyeFocus[1] = m_eyeFocus.y;
	mn_eyeFocus[2] = m_eyeFocus.z;
}

std::string Menu::GetFocusPlanet(void) const {
	return focus_planet;
}

void Menu::SetFocusPlanet(const std::string & planet) {
	focus_planet = planet;
}
