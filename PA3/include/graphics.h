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
	std::string ErrorString(GLenum error) const;
	
	Camera * m_camera;
	Shader * m_shader;
	
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;
	
	Object * m_planet;
	Object * m_moon;
	
	//variables for displaying text
	Text * m_textPlanetIntro;
	Text * m_textPlanetInfo;
	int m_planetDir; //keeps track of which text needs to be displayed for planet orbit direction
	Text * m_textMoonIntro;
	Text * m_textMoonInfo; 
	int m_moonDir; //keeps track of which text needs to be displayed for moon orbit direction
	
	void loadTexts(void); //internal function to load new texts if needed
};

#endif /* GRAPHICS_H */
