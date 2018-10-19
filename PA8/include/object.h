#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"

#include <assimp/Importer.hpp>

#include <vector>

class Object {
public:
	Object(const std::string & objFile, float orbitRadiusX, float orbitRadiusZ, const glm::vec3 & rotationAngles, float orbitSpeed, float rotationSpeed);
	~Object(void);
	void Update(unsigned int dt);
	void Render(void);

	glm::mat4 GetModel(void);

	void SetScale(const glm::vec3 & scale);
	glm::vec3 GetScale(void) const;
	
	void SetRotationAngles(const glm::vec3 & rotationAngles);
	glm::vec3 GetRotationAngles(void) const;
	
	void SetOrbitSpeed(float orbitSpeed);
	float GetOrbitSpeed(void) const;
	void SetOrbitCenter(const glm::vec3 & center);
	glm::vec3 GetOrbitCenter(void) const;
	
	glm::vec3 GetCurrentLocation(void) const;

	void SetRotationSpeed(float rotationSpeed);
	float GetRotationSpeed(void) const;


private:
	glm::mat4 m_model;

	glm::vec3 m_translation;
	glm::vec3 m_scale;
	glm::vec3 m_rotationAngles;

	std::vector<Vertex> m_vertices;
	std::vector<std::vector<unsigned int>> m_indices; //holds indicy set for each texture

	GLuint VB;
	std::vector<GLuint> IB; //vector of index set buffers

	//orbital variables
	float m_orbitRadiusX;
	float m_orbitRadiusZ;
	float m_orbitSpeed;
	float m_angleOfOrbit;
	glm::vec3 m_orbitCenter;

	float m_rotationSpeed;

	std::vector<GLuint> m_textures; //vector of texture locations on GPU
	std::vector<aiString> m_textureFiles; //vector of text file names

	bool loadObjAssimp(const std::string & objFile);
	void loadTextures(const std::string & objFile);
};

#endif /* OBJECT_H */
