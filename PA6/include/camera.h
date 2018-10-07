#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera {
public:
	Camera(void);
	~Camera(void);
	bool Initialize(int w, int h, const glm::vec3 & eyePos);
	
	glm::mat4 GetProjection(void) const;
	glm::mat4 GetView(void) const;
	glm::vec3 GetEyePos(void) const;

private:
	
	glm::vec3 m_eyePos;
	
	glm::mat4 m_projection;
	glm::mat4 m_view;
};

#endif /* CAMERA_H */
