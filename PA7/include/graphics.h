#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics_headers.h"

#include "camera.h"
#include "shader.h"
#include "object.h"

#include <SDL2/SDL.h>
#include <vector>

class Graphics {
public:
	Graphics(void);
	~Graphics(void);

	bool Initialize(int width, int height, const std::string & vertShaderSrc, const std::string & fragShaderSrc, const glm::vec3 & eyePos,
			const glm::vec3 & focusPos);

	void AddObject(const std::string & objFile);

	void Update(unsigned int dt);

	bool UpdateCamera(const glm::vec3 & eyePos, const glm::vec3 & eyeFocus);

	void Render(void);

	glm::vec3 GetEyePos(void) const;
	glm::vec3 GetEyeLoc(void) const;

private:
	std::string ErrorString(const GLenum error) const;

	Camera * m_camera;
	Shader * m_shader;

	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;

	std::vector<Object *> m_objects;
};

#endif /* GRAPHICS_H */
