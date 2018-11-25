#include "graphics.h"

Graphics::Graphics(unsigned int windowWidth, unsigned int windowHeight, const glm::vec3 & eyePos, const glm::vec3 & eyeFocus, const boardInfo & board) :
		m_camera(windowWidth, windowHeight, eyePos, eyeFocus), m_board(nullptr) {

// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
	glewExperimental = GL_TRUE;

	const GLenum status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK) {
		std::string errorMsg = "Graphics could not be created! GLEW Error: ";
		errorMsg += ((char*) glewGetErrorString(status));
		throw errorMsg;
	}
#endif

	// For OpenGL 3
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//enable depth testing
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);

	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_board = new Board(board);
}

Graphics::~Graphics(void) {
	delete m_board;
}

void Graphics::AddShaderSet(const std::string & setName, const std::string & vertexShaderSrc, const std::string & fragmentShaderSrc) {
	m_board->AddShaderSet(setName, vertexShaderSrc, fragmentShaderSrc);
}

void Graphics::UseShaderSet(const std::string & setName) {
	m_board->UseShaderSet(setName);
	m_board->UpdateLightBindings();
	UpdateCameraBindings();
}

void Graphics::Update(unsigned int dt) {
	m_board->Update();
}

void Graphics::Render(void) {
	//Clear the screen
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_board->Render();

	//Get any errors from OpenGL
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::string errMsg = std::string("Error initializing OpenGL! ") + std::to_string(error) + std::string(" : ") + ErrorString(error);
		throw errMsg;
	}
}

void Graphics::IncreaseEyePosX(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.x += moveAmount;
	UpdateCamera(newEyePos, GetEyeLoc());

}

void Graphics::DecreaseEyePosX(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.x -= moveAmount;
	UpdateCamera(newEyePos, GetEyeLoc());
}

void Graphics::IncreaseEyePosZ(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.z -= moveAmount;
	UpdateCamera(newEyePos, GetEyeLoc());
}

void Graphics::DecreaseEyePosZ(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.z += moveAmount;
	UpdateCamera(newEyePos, GetEyeLoc());
}

void Graphics::ZoomIn(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - newEyePos);
	newEyePos += moveVector;
	UpdateCamera(newEyePos, GetEyeLoc());
}

void Graphics::ZoomOut(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - newEyePos);
	newEyePos -= moveVector;
	UpdateCamera(newEyePos, GetEyeLoc());
}

void Graphics::MoveForward(float moveAmount) {
  glm::vec3 newEyePos = GetEyePos();
  glm::vec3 newEyeLoc = GetEyeLoc();
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - newEyePos);
	newEyePos += moveVector * moveAmount;
  newEyeLoc += moveVector * moveAmount;
	UpdateCamera(newEyePos, newEyeLoc);
}

void Graphics::MoveBackward(float moveAmount) {
  glm::vec3 newEyePos = GetEyePos();
  glm::vec3 newEyeLoc = GetEyeLoc();
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - newEyePos);
	newEyePos -= moveVector * moveAmount;
  newEyeLoc -= moveVector * moveAmount;
	UpdateCamera(newEyePos, newEyeLoc);
}

void Graphics::MoveRight(float moveAmount) {
  glm::vec3 newEyePos = GetEyePos();
  glm::vec3 newEyeLoc = GetEyeLoc();
	glm::vec3 pointVector = newEyeLoc - newEyePos;
  glm::vec3 upVector = glm::vec3(0, 1, 0);
  glm::vec3 moveVector = glm::normalize(glm::cross(pointVector, upVector));
	newEyePos += moveVector * moveAmount;
  newEyeLoc += moveVector * moveAmount;
	UpdateCamera(newEyePos, newEyeLoc);
}

void Graphics::MoveLeft(float moveAmount) {
  glm::vec3 newEyePos = GetEyePos();
  glm::vec3 newEyeLoc = GetEyeLoc();
	glm::vec3 pointVector = newEyeLoc - newEyePos;
  glm::vec3 upVector = glm::vec3(0, 1, 0);
  glm::vec3 moveVector = glm::normalize(glm::cross(pointVector, upVector));
	newEyePos -= moveVector * moveAmount;
  newEyeLoc -= moveVector * moveAmount;
	UpdateCamera(newEyePos, newEyeLoc);
}

void Graphics::UpdateCamera(const glm::vec3 & eyePos, const glm::vec3 & eyeFocus) {
	m_camera.UpdateCamera(eyePos, eyeFocus);
	UpdateCameraBindings();
}

glm::vec3 Graphics::GetEyePos(void) const {
	return m_camera.GetEyePos();
}

glm::vec3 Graphics::GetEyeLoc(void) const {
	return m_camera.GetEyeFocus();
}

void Graphics::ChangeAmbientLight(const glm::vec3 & change) {
	m_board->ChangeAmbientLight(change);
}

void Graphics::ChangeDiffuseLight(const glm::vec3 & change) {
	m_board->ChangeDiffuseLight(change);
}

void Graphics::ChangeSpecularLight(const glm::vec3 & change) {
	m_board->ChangeSpecularLight(change);
}

std::string Graphics::ErrorString(const GLenum error) const {
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

//updates bindings for camera in shader - need to call for camera change to take effect
void Graphics::UpdateCameraBindings(void) {
	m_board->UpdateCameraBindings(m_camera.GetView(), m_camera.GetProjection(), m_camera.GetEyePos());
}
