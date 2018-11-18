#include "camera.h"

Camera::Camera(unsigned int width, unsigned int height, const glm::vec3 & eyePos, const glm::vec3 & eyeFocus) :
		m_position(eyePos), m_positionDefault(eyePos), m_focus(eyeFocus), m_focusDefault(eyeFocus), m_projection(1.0f), m_view(1.0f) {
	UpdateViewMat();

	m_projection = glm::perspective(45.0f, //the FoV typically 90 degrees is good which is what this is set to
			float(width) / float(height), //Aspect Ratio, so Circles stay Circular
			0.01f, //Distance to the near plane, normally a small value like this
			500.0f); //Distance to the far plane, 
}

Camera::~Camera(void) {
}

void Camera::UpdateCamera(const glm::vec3 & eyePos, const glm::vec3 & eyeFocus) {
	m_position = eyePos;
	m_focus = eyeFocus;
	UpdateViewMat();
}

void Camera::ReturnToDefault(void) {
	m_position = m_positionDefault;
	m_focus = m_focusDefault;
	UpdateViewMat();
}

glm::mat4 Camera::GetProjection(void) const {
	return m_projection;
}

glm::mat4 Camera::GetView(void) const {
	return m_view;
}

glm::vec3 Camera::GetEyePos(void) const {
	return m_position;
}

glm::vec3 Camera::GetEyeFocus(void) const {
	return m_focus;
}

void Camera::UpdateViewMat(void) {
	//reinitialize the view matrix
	m_view = glm::lookAt(m_position, //Eye Position
			m_focus, //Focus point
			glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
}

