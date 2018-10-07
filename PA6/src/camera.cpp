#include "camera.h"

Camera::Camera(void) {

}

Camera::~Camera(void) {

}

bool Camera::Initialize(int w, int h, const glm::vec3 & eyePos) {
	m_eyePos = eyePos;
	//--Init the view and projection matrices
	//  if you will be having a moving camera the view matrix will need to more dynamic
	//  ...Like you should update it before you render more dynamic 
	//  for this project having them static will be fine
	m_view = glm::lookAt(m_eyePos, //Eye Position
			glm::vec3(0.0, 0.0, 0.0), //Focus point
			glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

	m_projection = glm::perspective(45.0f, //the FoV typically 90 degrees is good which is what this is set to
			float(w) / float(h), //Aspect Ratio, so Circles stay Circular
			0.01f, //Distance to the near plane, normally a small value like this
			100.0f); //Distance to the far plane, 

	return true;
}

glm::mat4 Camera::GetProjection(void) const {
	return m_projection;
}

glm::mat4 Camera::GetView(void) const {
	return m_view;
}


glm::vec3 Camera::GetEyePos(void) const{
	return m_eyePos;
}
