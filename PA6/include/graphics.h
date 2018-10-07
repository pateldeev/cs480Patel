#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics_headers.h"

#include "camera.h"
#include "shader.h"
#include "object.h"

#include <SDL2/SDL.h>

class Graphics {
public:
	Graphics(void);
	~Graphics(void);
	bool Initialize(int width, int height, const glm::vec3 & eyePos, const std::string & objFile);
	bool UpdateParameters(int width, int height, const glm::vec3 & eyePos, const glm::vec3 & translationVec, const glm::vec3 & scaleVec,
			const glm::vec3 rotationAnglesVec);
	void Update(void);
	void Render(void);
	
	glm::vec3 GetEyePos(void) const;

private:
	std::string ErrorString(GLenum error) const;

	Camera * m_camera;
	Shader * m_shader;

	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;

	Object * m_object;
};

#endif /* GRAPHICS_H */
