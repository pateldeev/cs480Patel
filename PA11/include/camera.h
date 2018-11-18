#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera {
public:
	Camera(unsigned int width, unsigned int height, const glm::vec3 & eyePos, const glm::vec3 & eyeFocus = glm::vec3(0.0, 0.0, 0.0));
	~Camera(void);

	//allow coping and moving, but no assignment
	Camera(const Camera & other) = default;
	Camera(Camera && other) = default;
	Camera& operator=(const Camera) = delete;
	Camera& operator=(Camera&&) = delete;

	void UpdateCamera(const glm::vec3 & eyePos, const glm::vec3 & eyeFocus);
	void ReturnToDefault(void);

	glm::mat4 GetProjection(void) const;
	glm::mat4 GetView(void) const;

	glm::vec3 GetEyePos(void) const;
	glm::vec3 GetEyeFocus(void) const;

private:
	void UpdateViewMat(void);

	//camera position
	glm::vec3 m_position;
	const glm::vec3 m_positionDefault;

	//camera focus
	glm::vec3 m_focus;
	const glm::vec3 m_focusDefault;

	glm::mat4 m_projection;
	glm::mat4 m_view;
};

#endif /* CAMERA_H */
