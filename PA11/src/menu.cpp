#include "menu.h"

Menu::Menu(const SDL_GLContext & gl_context, const glm::vec3 & eyeLoc, const glm::vec3 & eyeFocus, const glm::uvec2 & menuSize) :
		m_window(nullptr), m_eyeLoc(eyeLoc), m_eyeFocus(eyeFocus), m_menuSize(menuSize), m_menuTL(50, 50) {
	UpdateMenuParams();

	m_window = SDL_CreateWindow("Menu", m_menuTL.y, m_menuTL.x, m_menuSize.y, m_menuSize.x, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	const char * glsl_version = "#version 330";
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(m_window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsDark();
}

Menu::~Menu(void) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyWindow (m_window);
}

bool Menu::Update(const SDL_GLContext & gl_context, const glm::vec3 & currrentEyeLoc, const glm::vec3 & currentEyeFocus) {

	bool updated = false;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame (m_window);
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
	ImGui::SetNextWindowSize(ImVec2((m_menuSize.y) > 25 ? m_menuSize.y - 25 : 0, (m_menuSize.x) > 25 ? m_menuSize.x - 25 : 0));
	ImGui::Begin("Menu");

	ImGui::Text("Use this section of the menu to move the camera around.");
	ImGui::Text("\n");

	ImGui::InputFloat3("Camera Position", mn_eyeLoc);
	ImGui::Text("Current Camera Position: (%.2f, %.2f, %.2f)", currrentEyeLoc.x, currrentEyeLoc.y, currrentEyeLoc.z);
	ImGui::Text("\n");

	ImGui::InputFloat3("Camera Focus", mn_eyeFocus);
	ImGui::Text("Current Camera Focus: (%.2f, %.2f, %.2f)", currentEyeFocus.x, currentEyeFocus.y, currentEyeFocus.z);
	ImGui::Text("\n");
	if (ImGui::Button("Update Location")) {
		updated = true;
		m_eyeLoc = glm::vec3(mn_eyeLoc[0], mn_eyeLoc[1], mn_eyeLoc[2]);
		m_eyeFocus = glm::vec3(mn_eyeFocus[0], mn_eyeFocus[1], mn_eyeFocus[2]);
	}

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
