#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics_headers.h"

#include "camera.h"
#include "shader.h"
#include "object.h"
#include "configLoader.hpp"

#include <SDL2/SDL.h>
#include <vector>

class Graphics {
public:
	Graphics(void);
	~Graphics(void);

	bool Initialize(int windowWidth, int windowHeight, const std::string & vertShaderSrc, const std::string & fragShaderSrc, const glm::vec3 & eyePos,
			const glm::vec3 & focusPos);

	void AddPlanet(const Planet & planet, const std::string & moonObjFile);
	void FollowPlanet(const std::string & planetName);
	std::string GetFollowPlanet(void) const;
	void SystemView(void);
	void UserControlledView(void);
	void ZoomCloser(void);
	void ZoomAway(void);
	void SetViewDistance(int distance);
	void SetZoomFlag(bool setFlag);

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

	std::vector<std::string> m_planetNames;
	std::vector<Object *> m_planets;
	std::vector<std::vector<Object *>> m_moons;

	int m_followingPlanet; //planet being followed
	int m_viewDistance; //used when zooming in and out with a planet

	bool m_zoomFlag;  //whether the camera has zoomed closer or farther from the planet
};

#endif /* GRAPHICS_H */
