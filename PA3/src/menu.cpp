#include "menu.h"

ImVec4 Menu::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

Menu::Menu(const glm::vec3 & camPos) :
		m_camLoc(camPos){
}

Menu::~Menu(void) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyWindow (m_window);
}

bool Menu::Initialize(SDL_GLContext & gl_context) {

	m_window = SDL_CreateWindow("Menu_CameraPosition", 50, 50, 500, 300, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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

	ImGui::Begin("Camera Position");

	ImGui::Text("Use this menu to change the camera position.");

	static float x = m_camLoc.x, y = m_camLoc.y, z = m_camLoc.z;

	ImGui::SliderFloat("x", &x, -20.0f, 20.0f);
	ImGui::SliderFloat("y", &y, -20.0f, 20.0f);
	ImGui::SliderFloat("z", &z, -20.0f, 20.0f);

	ImGui::Text("Current camera position: (%.2f, %.2f, %.2f)", m_camLoc.x, m_camLoc.y, m_camLoc.z);
	ImGui::Text("\n");
	
	if (ImGui::Button("Restart with new camera position")) {
		updated = true;
		m_camLoc.x = x;
		m_camLoc.y = y;
		m_camLoc.z = z;
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

SDL_Window * Menu::GetWindow(void) {
	return m_window;
}

glm::vec3 Menu::GetNewPosition(void) const{
	return m_camLoc;
}
