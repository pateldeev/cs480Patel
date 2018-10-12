#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"

#include <assimp/Importer.hpp>

#include <vector>

class Object {
public:
	Object(const std::string & objFile, float orbitRadiusX, float orbitRadiusZ, float rotationSpeed, float orbitSpeed);
	~Object(void);
	void Update(unsigned int dt);
	void Render(void);

	glm::mat4 GetModel(void);

	void SetRotationSpeed(float rotationSpeed);
	float GetRotationSpeed(void) const;
	void SetOrbitSpeed(float orbitSpeed);
	float GetOrbitSpeed(void) const;
	
	void ToggleRotation(bool rotate);
	bool IsRotating(void) const;
	void ToggleOrbit(bool orbit);
	bool IsOrbiting(void) const;
	
	void SetOrbitCenter(const glm::vec3 & center);
	glm::vec3 GetOrbitCenter(void) const;
	glm::vec3 GetOrbitLoc(void) const;
	
	void SetScale(const glm::vec3 & scale);
	glm::vec3 GetScale(void) const;

private:
	glm::mat4 m_model;

	glm::vec3 m_translation;
	glm::vec3 m_scale;
	glm::vec3 m_rotationAngles;

	std::vector<Vertex> Vertices;
	std::vector<std::vector<unsigned int>> Indices; //holds indicy set for each texture

	GLuint VB;
	std::vector<GLuint> IB; //vector of index set buffers

	float angleRotation;
	float angleOrbit;
	float orbitRadiusX;
	float orbitRadiusZ;

	glm::vec3 orbitCenter; //center of object's orbital
	glm::vec3 orbitLoc; //position of object on the orbit 
	glm::vec3 objectScale; //scaling vector applied to object

	float speedRotation;
	float speedOrbit;
	bool pauseRotation;
	bool pauseOrbit;

	std::vector<GLuint> m_textures; //vector of texture locations on GPU
	std::vector<aiString> m_textureFiles; //vector of text file names

	bool loadObjAssimp(const std::string & objFile);
	void loadTextures(const std::string & objFile);
};

#endif /* OBJECT_H */
