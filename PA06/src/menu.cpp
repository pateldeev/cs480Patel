#include "menu.h"

Menu::Menu(const glm::vec3 & eyeLoc, const glm::vec3 & translationVec, const glm::vec3 & scaleVec, const glm::vec3 & rotationAnglesVec) :
		m_window(nullptr), m_eyeLoc(eyeLoc), m_translation(translationVec), m_scale(scaleVec), m_rotation(rotationAnglesVec) {

	mn_eyeLoc[0] = m_eyeLoc.x;
	mn_eyeLoc[1] = m_eyeLoc.y;
	mn_eyeLoc[2] = m_eyeLoc.z;

	mn_translation[0] = m_translation.x;
	mn_translation[1] = m_translation.y;
	mn_translation[2] = m_translation.z;

	mn_scale[0] = m_scale.x;
	mn_scale[1] = m_scale.y;
	mn_scale[2] = m_scale.z;

	mn_rotation[0] = m_rotation.x;
	mn_rotation[1] = m_rotation.y;
	mn_rotation[2] = m_rotation.z;
}

Menu::~Menu(void) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyWindow (m_window);
}

bool Menu::Initialize(const SDL_GLContext & gl_context) {

	m_window = SDL_CreateWindow("Updator_Menu", 50, 50, 650, 700, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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

bool Menu::Update(const SDL_GLContext & gl_context) {

	bool updated = false;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame (m_window);
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
	ImGui::SetNextWindowSize(ImVec2(630, 680));
	ImGui::Begin("Menu");

	ImGui::Text("Use this menu to change the parameters of the OPENGL rendering.");
	ImGui::Text("\n");

	ImGui::InputFloat3("Camera Position", mn_eyeLoc);
	ImGui::Text("Current Camera Position: (%.2f, %.2f, %.2f)", m_eyeLoc.x, m_eyeLoc.y, m_eyeLoc.z);
	ImGui::Text("\n");

	ImGui::InputFloat3("Object Traslation", mn_translation);
	ImGui::Text("Current Translation Vector: (%.2f, %.2f, %.2f)", m_translation.x, m_translation.y, m_translation.z);
	ImGui::Text("\n");

	ImGui::InputFloat3("Object Scale", mn_scale);
	ImGui::Text("Current Scale Vector: (%.2f, %.2f, %.2f)", m_scale.x, m_scale.y, m_scale.z);
	ImGui::Text("\n");

	ImGui::InputFloat3("Object Rotion", mn_rotation);
	ImGui::Text("Current Rotation Angles (in Radians): (%.2f, %.2f, %.2f)", m_rotation.x, m_rotation.y, m_rotation.z);
	ImGui::Text("\n");

	ImGui::Text("\n");
	if (ImGui::Button("Update")) {
		updated = true;
		UpdateVariables();
	}

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

glm::vec3 Menu::GetEyePosition(void) const {
	return m_eyeLoc;
}

glm::vec3 Menu::GetTranslationVec(void) const {
	return m_translation;
}

glm::vec3 Menu::GetScaleVec(void) const {
	return m_scale;
}

glm::vec3 Menu::GetRotationVec(void) const {
	return m_rotation;
}

void Menu::UpdateVariables(void) {
	m_eyeLoc.x = mn_eyeLoc[0];
	m_eyeLoc.y = mn_eyeLoc[1];
	m_eyeLoc.z = mn_eyeLoc[2];

	m_translation.x = mn_translation[0];
	m_translation.y = mn_translation[1];
	m_translation.z = mn_translation[2];

	m_scale.x = mn_scale[0];
	m_scale.y = mn_scale[1];
	m_scale.z = mn_scale[2];

	m_rotation.x = mn_rotation[0];
	m_rotation.y = mn_rotation[1];
	m_rotation.z = mn_rotation[2];
}
