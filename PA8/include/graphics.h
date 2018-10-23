#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics_headers.h"

#include "camera.h"
#include "shader.h"
#include "object.h"
#include "configLoader.hpp"

#include <SDL2/SDL.h>
#include <vector>

#include <btBulletDynamicsCommon.h>

class Graphics {
public:
	Graphics(void);
	~Graphics(void);

	bool Initialize(int windowWidth, int windowHeight, const std::string & vertShaderSrc, const std::string & fragShaderSrc, const glm::vec3 & eyePos,
			const glm::vec3 & focusPos);

	void Update(unsigned int dt);
	
	void AddObject(const std::string & objFile);

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

	std::vector<Object> m_objects;
};

#endif /* GRAPHICS_H */
