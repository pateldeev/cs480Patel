#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

#include "text.h"

class Graphics {
public:
	Graphics(void);
	~Graphics(void);
	bool Initialize(int width, int height, const glm::vec3 & eyePos);
	void Update(unsigned int dt);
	void Render(void);
	bool handleEvent(const SDL_Event & event);
		
private:
	std::string ErrorString(GLenum error);
	
	Camera * m_camera;
	Shader * m_shader;
	
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;
	
	Object * m_planet;
	Object * m_moon;
	
	Text * m_textPlanetIntro;
	Text * m_textMoonIntro;
	Text * m_textC;
	Text * m_textCC;
	Text * m_textNot;
};

#endif /* GRAPHICS_H */
