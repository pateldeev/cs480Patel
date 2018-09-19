#include "graphics.h"

Graphics::Graphics(void) {

}

Graphics::~Graphics(void) {
	delete m_planet;
	delete m_moon;
	delete m_camera;
	delete m_shader;
}

bool Graphics::Initialize(int width, int height, const glm::vec3 & eyePos) {

// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
	// std::cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK) {
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif

	// For OpenGL 3
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height, eyePos)) {
		printf("Camera Failed to Initialize\n");
		return false;
	}

	//Create the objects
	m_planet = new Object(6.25, 3.75, 0.00120, 0.00038);
	m_moon = new Object(2.9, 3.1, 0.00210, 0.00110);
	m_moon->SetScale(glm::vec3(0.35, 0.35, 0.35));

	//Create text objects for information
	m_textPlanetIntro = new Text("objFiles/planetIntro.obj", glm::vec3(1.0, 0.0, 0.0));
	m_textPlanetIntro->Update(glm::vec3(8.55, 7.75, 0.0), glm::vec3(0.70, 0.70, 0.70));
	m_textMoonIntro = new Text("objFiles/moonIntro.obj", glm::vec3(1.0, 0.0, 0.0));
	m_textMoonIntro->Update(glm::vec3(8.75, 6.5, 0.0), glm::vec3(0.70, 0.70, 0.70));
	m_planetDir = m_moonDir = 1;
	loadTexts();

	//Set up the shaders
	m_shader = new Shader();
	if (!m_shader->Initialize()) {
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER)) {
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER)) {
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize()) {
		printf("Program to Finalize\n");
		return false;
	}

	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) {
		printf("m_projectionMatrix not found\n");
		return false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION) {
		printf("m_viewMatrix not found\n");
		return false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION) {
		printf("m_modelMatrix not found\n");
		return false;
	}

	//enable depth testing
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);

	return true;
}

void Graphics::Update(unsigned int dt) {
	//Update the planet
	m_planet->Update(dt);

	//Update moon after setting its new orbit center
	m_moon->SetOrbitCenter(m_planet->GetOrbitLoc());
	m_moon->Update(dt);

	//Update text 
	m_planetDir = m_planet->IsOrbiting() ? (m_planet->GetOrbitSpeed() > 0.0 ? 1 : -1) : 0;
	m_moonDir = m_moon->IsOrbiting() ? (m_moon->GetOrbitSpeed() > 0.0 ? 1 : -1) : 0;
	loadTexts();
	m_textMoonInfo->Update(glm::vec3(5.05, 6.5, 0.0), glm::vec3(0.70, 0.70, 0.70));
	m_textPlanetInfo->Update(glm::vec3(5.15, 7.75, 0.0), glm::vec3(0.70, 0.70, 0.70));

}

void Graphics::Render(void) {
	//Clear the screen
	glClearColor(0.0, 0.0, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Start the correct program
	m_shader->Enable();

	//Send in the projection and view to the shader
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	//Render the planet
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_planet->GetModel()));
	m_planet->Render();

	//Render the moon
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_moon->GetModel()));
	m_moon->Render();

	//Render text information
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_textPlanetIntro->GetModel()));
	m_textPlanetIntro->Render();
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_textPlanetInfo->GetModel()));
	m_textPlanetInfo->Render();
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_textMoonIntro->GetModel()));
	m_textMoonIntro->Render();
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_textMoonInfo->GetModel()));
	m_textMoonInfo->Render();

	//Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		std::string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}

bool Graphics::handleEvent(const SDL_Event & event) {

	static bool lShift = false; //keeps track if left shift key is curently pressed

	if (event.type == SDL_KEYDOWN) {

		if (event.key.keysym.sym == SDLK_LSHIFT) {
			lShift = true;
			return true;
		}

		Object * affectedObj = (lShift) ? m_moon : m_planet;

		switch (event.key.keysym.sym) {

		case SDLK_s:
			affectedObj->ToggleRotation(false);
			affectedObj->ToggleOrbit(false);
			return true;

		case SDLK_c:
			affectedObj->ToggleRotation(true);
			affectedObj->ToggleOrbit(true);
			return true;

		case SDLK_r:
			affectedObj->ToggleRotation(!affectedObj->IsRotating());
			return true;

		case SDLK_o:
			affectedObj->ToggleOrbit(!affectedObj->IsOrbiting());
			return true;

		case SDLK_LEFT:
			affectedObj->SetOrbitSpeed(std::abs(affectedObj->GetOrbitSpeed()));
			return true;

		case SDLK_RIGHT:
			affectedObj->SetOrbitSpeed(-1 * std::abs(affectedObj->GetOrbitSpeed()));
			return true;

		case SDLK_UP:
			affectedObj->SetRotationSpeed(std::abs(affectedObj->GetRotationSpeed()));
			return true;

		case SDLK_DOWN:
			affectedObj->SetRotationSpeed(-1 * std::abs(affectedObj->GetRotationSpeed()));
			return true;
		}
	}

	if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LSHIFT) {
		lShift = false;
		return true;
	}

	if (event.type == SDL_MOUSEBUTTONDOWN) {

		Object * affectedObj = (lShift) ? m_moon : m_planet;

		if (event.button.button == SDL_BUTTON_LEFT) {
			affectedObj->SetOrbitSpeed(-1 * affectedObj->GetOrbitSpeed());
			return true;
		} else if (event.button.button == SDL_BUTTON_RIGHT) {
			affectedObj->SetRotationSpeed(-1 * affectedObj->GetRotationSpeed());
			return true;
		} else {
			printf("Unhandeled SDL mouse press. Could not understand button pressed: %d \n", event.button.button);
		}
	}
	return false;
}

std::string Graphics::ErrorString(GLenum error) const {
	if (error == GL_INVALID_ENUM)
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	else if (error == GL_INVALID_VALUE)
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	else if (error == GL_INVALID_OPERATION)
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	else if (error == GL_OUT_OF_MEMORY)
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	else
		return "None";

}

void Graphics::loadTexts(void) {
	if (m_planetDir == 1)
		m_textPlanetInfo = new Text("objFiles/msgOrbitC.obj", glm::vec3(1.0, 0.8, 0.6));
	else if (m_planetDir == 0)
		m_textPlanetInfo = new Text("objFiles/msgOrbitNot.obj", glm::vec3(1.0, 0.8, 0.6));
	else if (m_planetDir == -1)
		m_textPlanetInfo = new Text("objFiles/msgOrbitCC.obj", glm::vec3(1.0, 0.8, 0.6));
	else
		printf("Error: could not get correct direction text for planet \n");
	
	if (m_moonDir == 1)
		m_textMoonInfo = new Text("objFiles/msgOrbitC.obj", glm::vec3(1.0, 0.8, 0.6));
	else if (m_moonDir == 0)
		m_textMoonInfo = new Text("objFiles/msgOrbitNot.obj", glm::vec3(1.0, 0.8, 0.6));
	else if (m_moonDir == -1)
		m_textMoonInfo = new Text("objFiles/msgOrbitCC.obj", glm::vec3(1.0, 0.8, 0.6));
	else
		printf("Error: could not get correct direction text for moon \n");
}
