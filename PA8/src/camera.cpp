#include "camera.h"

Camera::Camera(const glm::vec3 & eyePos, const glm::vec3 & focusPos) :
		m_eyePos(eyePos), m_focusPos(focusPos) {
    default_eyePos = eyePos;
    default_focusPos = focusPos;
}

Camera::~Camera(void) {

}

bool Camera::Initialize(int width, int height) {
	//--Init the view and projection matrices
	m_view = glm::lookAt(m_eyePos, //Eye Position
			m_focusPos, //Focus point
			glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

	m_projection = glm::perspective(45.0f, //the FoV typically 90 degrees is good which is what this is set to
			float(width) / float(height), //Aspect Ratio, so Circles stay Circular
			0.01f, //Distance to the near plane, normally a small value like this
			500.0f); //Distance to the far plane, 

	return true;
}

void Camera::UpdatePosition(const glm::vec3 & eyePos, const glm::vec3 & focusPos) {
	m_eyePos = eyePos;
	m_focusPos = focusPos;

	//reinitialize the view matrix
	m_view = glm::lookAt(m_eyePos, //Eye Position
			m_focusPos, //Focus point
			glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
}

void Camera::ReturnToDefault(void) {
    m_eyePos = default_eyePos;
	m_focusPos = default_focusPos;

	//reinitialize the view matrix
	m_view = glm::lookAt(m_eyePos, //Eye Position
			m_focusPos, //Focus point
			glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
}

glm::mat4 Camera::GetProjection(void) const {
	return m_projection;
}

glm::mat4 Camera::GetView(void) const {
	return m_view;
}

glm::vec3 Camera::GetEyePos(void) const {
	return m_eyePos;
}

glm::vec3 Camera::GetFocusPos(void) const {
	return m_focusPos;
}

