#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera {
public:
	Camera(const glm::vec3 & eyePos, const glm::vec3 & focusPos = glm::vec3(0.0, 0.0, 0.0));
	~Camera(void);
	bool Initialize(int width, int height);
	
	void UpdatePosition(const glm::vec3 & eyePos, const glm::vec3 & focusPos);
        void ReturnToDefault(void);

	glm::mat4 GetProjection(void) const;
	glm::mat4 GetView(void) const;
	
	glm::vec3 GetEyePos(void) const;
	glm::vec3 GetFocusPos(void) const;

private:

        glm::vec3 default_eyePos;
        glm::vec3 default_focusPos;
    
	glm::vec3 m_eyePos;
	glm::vec3 m_focusPos;

	glm::mat4 m_projection;
	glm::mat4 m_view;
};

#endif /* CAMERA_H */
