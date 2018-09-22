#include "menu.h"

ImVec4 Menu::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

Menu::Menu(const glm::vec3 & camPos) :
		m_eyeLoc(camPos), m_translationMat(0.0, 0.0, 0.0), m_scaleMat(1.0,1.0,1.0), m_rotationMat(0.0,0.0,0.0) {

	mn_eyeLoc[0] = m_eyeLoc.x;
	mn_eyeLoc[1] = m_eyeLoc.y;
	mn_eyeLoc[2] = m_eyeLoc.z;

	mn_translationMat[0] = m_translationMat.x;
	mn_translationMat[1] = m_translationMat.y;
	mn_translationMat[2] = m_translationMat.z;

	mn_scaleMat[0] = m_scaleMat.x;
	mn_scaleMat[1] = m_scaleMat.y;
	mn_scaleMat[2] = m_scaleMat.z;

	mn_rotationMatp3[0] = m_rotationMat.x;
	mn_rotationMatp3[1] = m_rotationMat.y;
	mn_rotationMatp3[2] = m_rotationMat.z;
}

Menu::~Menu(void) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyWindow (m_window);
}

bool Menu::Initialize(SDL_GLContext & gl_context) {

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

bool Menu::Update(SDL_GLContext & gl_context) {

	bool updated = false;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame (m_window);
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0.0,0.0));
	ImGui::SetNextWindowSize(ImVec2(630,680));
	ImGui::Begin("Menu");

	ImGui::Text("Use this menu to change the parameters of the OPENGL rendering.");
	ImGui::Text("\n");

	ImGui::InputFloat3("Camera Position",mn_eyeLoc);
	ImGui::Text("Current Camera Position: (%.2f, %.2f, %.2f)", m_eyeLoc.x, m_eyeLoc.y, m_eyeLoc.z);

	ImGui::Text("\n");
	
	if (ImGui::Button("Update")) {
		updated = true;
		m_eyeLoc.x = mn_eyeLoc[0];
		m_eyeLoc.y = mn_eyeLoc[1];
		m_eyeLoc.z = mn_eyeLoc[2];
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

void Menu::HandleEvent(SDL_Event & event){
	ImGui_ImplSDL2_ProcessEvent(&event);
}

SDL_Window * Menu::GetWindow(void) {
	return m_window;
}

glm::vec3 Menu::GetEyePosition(void) const{
	return m_eyeLoc;
}
