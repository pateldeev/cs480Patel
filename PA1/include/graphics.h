#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics {
public:
	Graphics(void);
	~Graphics(void);
	bool Initialize(int width, int height, const glm::vec3 & eyePos);
	void Update(unsigned int dt);
	void Render(void);

private:
	std::string ErrorString(GLenum error);
	
	Camera * m_camera;
	Shader * m_shader;
	
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;
	
	Object * m_cube;
};

#endif /* GRAPHICS_H */
