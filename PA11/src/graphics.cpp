#include "graphics.h"

Graphics::Graphics(const glm::uvec2 & windowSize, const glm::vec3 & eyePos, const glm::vec3 & eyeFocus, const GameInfo & game) :
		m_camera(windowSize.x, windowSize.y, eyePos, eyeFocus), m_yaw(0.f), m_pitch(0.f), m_board(nullptr), m_screenSize(windowSize) {

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

	m_board = new Board(game); //load board
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
	m_board->UpdateSpotlightLoc(GetEyePos());
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

void Graphics::MoveUp(float moveAmount) {
	glm::vec3 newEyePos(GetEyePos());
	glm::vec3 newEyeLoc(GetEyeLoc());
	newEyePos += glm::vec3(0.0, 1.0, 0.0) * moveAmount;
	newEyeLoc += glm::vec3(0.0, 1.0, 0.0) * moveAmount;
	UpdateCamera(newEyePos, newEyeLoc);
}

void Graphics::MoveDown(float moveAmount) {
	glm::vec3 newEyePos(GetEyePos());
	glm::vec3 newEyeLoc(GetEyeLoc());
	newEyePos -= glm::vec3(0.0, 1.0, 0.0) * moveAmount;
	newEyeLoc -= glm::vec3(0.0, 1.0, 0.0) * moveAmount;
	UpdateCamera(newEyePos, newEyeLoc);
}

void Graphics::RotateCamera(float newX, float newY) {
	const float sensitivity = 0.9;
	newX *= sensitivity;
	newY *= sensitivity;

	m_yaw += newX;
	m_pitch += newY;

	if (m_pitch > 89.0)
		m_pitch = 89.0;
	else if (m_pitch < -89.0)
		m_pitch = -89.0;

	glm::vec3 front;
	front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
	front.y = -std::sin(glm::radians(m_pitch));
	front.z = std::sin(sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)));
	glm::vec3 newFront = glm::normalize(front);

	UpdateCamera(GetEyePos(), GetEyePos() + newFront);
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

//call this from engine, and it should successfully click on object
void Graphics::LeftClick(const glm::vec2 & mousePosition) {
	glm::uvec3 elementClicked;
	try {
		elementClicked = m_board->GetGameElementByPosition(GetPositionUnder(mousePosition));
	} catch (const std::string & e) {
#ifdef DEBUG
		printf("Not hit for mouse {%f, %f} \n", mousePosition.x, mousePosition.y);
		printf("   Msg: %s \n", e.c_str());
#endif
		return;
	}

	//just increment type by 2 to show it worked for now
	ObjType type = m_board->GetGameElementType(elementClicked);
	type = (ObjType)((static_cast<int>(type) + 2) % ObjType::NUM_TYPES);
	m_board->SetGameElementType(elementClicked, type);
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

//raycast and find position mouse is pointing at
glm::vec3 Graphics::GetPositionUnder(const glm::vec2 & mousePosition) {
	float mouseScreenX = (2.0f * mousePosition.x) / m_screenSize.x - 1.0f;
	float mouseScreenY = 1.0f - (2.0f * mousePosition.y) / m_screenSize.y;
	glm::vec4 mouseRayStart(mouseScreenX, mouseScreenY, -1.0f, 1.0f);
	glm::vec4 mouseRayEnd(mouseScreenX, mouseScreenY, 0, 1.0f);

	glm::mat4 inverseProjectionMatrix = glm::inverse(m_camera.GetProjection());
	glm::mat4 inverseViewMatrix = glm::inverse(m_camera.GetView());

	glm::vec4 cameraRayStart = inverseProjectionMatrix * mouseRayStart;
	cameraRayStart /= cameraRayStart.w;
	glm::vec4 cameraRayEnd = inverseProjectionMatrix * mouseRayEnd;
	cameraRayEnd /= cameraRayEnd.w;

	glm::vec4 worldRayStart = inverseViewMatrix * cameraRayStart;
	worldRayStart /= worldRayStart.w;
	glm::vec4 worldRayEnd = inverseViewMatrix * cameraRayEnd;
	worldRayEnd /= worldRayEnd.w;

	glm::vec3 worldRayDirection(worldRayEnd - worldRayStart);
	worldRayDirection = glm::normalize(worldRayDirection);
	glm::vec3 worldRayMax = glm::vec3(worldRayStart.x, worldRayStart.y, worldRayStart.z) + worldRayDirection * 1000.0f;

	btVector3 start, end;
	start = btVector3(worldRayStart.x, worldRayStart.y, worldRayStart.z);
	end = btVector3(worldRayMax.x, worldRayMax.y, worldRayMax.z);

	//get the raycast callback ready
	btCollisionWorld::ClosestRayResultCallback closestResults(start, end);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	m_board->GetBulletWorld()->rayTest(start, end, closestResults);

	//if it hit, grab the position of the collider, otherwise throw not found error
	if (closestResults.hasHit()) {
		btVector3 hitResults = closestResults.m_collisionObject->getWorldTransform().getOrigin();
		glm::vec3 cubePosition = glm::vec3(hitResults.x(), hitResults.y(), hitResults.z()); //get position 

#ifdef DEBUG
		printf("World position for cube for {%f, %f} was {%f, %f, %f}\n", mousePosition.x, mousePosition.y, cubePosition.x, cubePosition.y, cubePosition.z);
#endif
		return cubePosition;
	} else {
		throw std::string("No Hit Found");
	}
}
