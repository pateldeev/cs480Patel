#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"

#include <assimp/Importer.hpp>
#include <vector>

#include <btBulletDynamicsCommon.h>

class Object {
public:
	Object(const std::string & objFile);
	~Object(void);
	void Update(unsigned int dt);
	void Render(void);

	glm::mat4 GetModel(void);
	
	void SetCurrentLocation(const glm::vec3 & location);
	glm::vec3 GetCurrentLocation(void) const;
	
	void SetScale(const glm::vec3 & scale);
	glm::vec3 GetScale(void) const;
	
	void SetRotationAngles(const glm::vec3 & rotationAngles);
	glm::vec3 GetRotationAngles(void) const;	

private:
	glm::mat4 m_model;

	glm::vec3 m_translation;
	glm::vec3 m_scale;
	glm::vec3 m_rotationAngles;

	std::vector<Vertex> m_vertices;
	std::vector<std::vector<unsigned int>> m_indices; //holds indicy set for each texture

	GLuint VB;
	std::vector<GLuint> IB; //vector of index set buffers

	std::vector<GLuint> m_textures; //vector of texture locations on GPU

	bool loadObjAssimp(const std::string & objFile);
	void loadTextures(const std::string & objFile, const std::vector<aiString> & textureFiles);
};

#endif /* OBJECT_H */