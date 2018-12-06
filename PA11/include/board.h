#ifndef BOARD_H
#define BOARD_H

#include "graphics_headers.h"
#include "object.h"
#include "shader.h"

class Board {
public:
	Board(void) = delete;
	Board(const gameInfo & game);
	~Board(void);

	//Board is not meant to be copied or moved
	Board(const Board &) = delete;
	Board(Board &&) = delete;
	Board& operator=(const Board &) = delete;
	Board& operator=(Board&&) = delete;

	//to add and change shaders
	void AddShaderSet(const std::string & setName, const std::string & vertexShaderSrc, const std::string & fragmentShaderSrc);
	void UseShaderSet(const std::string & setName);

	void Update(void);
	void Render(void);

	//for changing lighting
	void ChangeAmbientLight(const glm::vec3 & change);
	void ChangeDiffuseLight(const glm::vec3 & change);
	void ChangeSpecularLight(const glm::vec3 & change);
	void UpdateSpotlightLoc(const glm::vec3 & location);

	//for updating the bindings of uniforms
	void UpdateCameraBindings(const glm::mat4 & viewMat, const glm::mat4 & projectionMat, const glm::vec3 & cameraPos);
	void UpdateLightBindings(void);
	void UpdateInstanceBindings(Object * obj);

private:
	void EnforceBounds(glm::vec3 & v, float min = 0.f, float max = 1.f); //rounds everything to be in range [min, max]

	Object * m_floor;
	Object * m_roof;

	//variable to keep track of shaders
	std::vector<std::pair<std::string, Shader *>> m_shaders;
	Shader * m_shaderCurrent;

	//for unifroms in shaders for translation and lighting
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;
	GLint m_lightPos;
	GLint m_cameraPos;
	GLint m_ambientProduct;
	GLint m_diffuseProduct;
	GLint m_specularProduct;
	GLint m_shininess;
	glm::vec3 m_ambientLevel;
	glm::vec3 m_diffuseLevel;
	glm::vec3 m_specularLevel;
	float m_shininessConst;
	glm::vec3 m_spotlightLoc;

	//for instancing
	GLint m_instanceChangeRow;
	GLint m_instanceChangeCol;
	GLint m_instanceNumPerRow;
	GLint m_samplers;
	GLint m_sampleTypes; //type of texture to use on each object
};

#endif /* BOARD_H */
