#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"

#include <assimp/Importer.hpp>

#include <vector>

class Object {
public:
	Object(const std::string & objFile);	
	~Object(void);
	void Update(void);
	void Update(const glm::vec3 & translation, const glm::vec3 & scale, const glm::vec3 & rotationAngles);
	void Render(void);

	glm::mat4 GetModel(void);

	glm::vec3 GetTranslation(void) const;
	glm::vec3 GetScale(void) const;
	glm::vec3 GetRotationAngles(void) const;

private:
	glm::mat4 m_model;

	glm::vec3 m_translation;
	glm::vec3 m_scale;
	glm::vec3 m_rotationAngles;

	std::vector<Vertex> Vertices;
	std::vector<std::vector<unsigned int>> Indices; //holds indicy set for each texture
	
	GLuint VB;
	std::vector<GLuint> IB; //vector of index set buffers
	
	std::vector<GLuint> m_textures; //vector of texture locations on GPU
	std::vector<aiString> m_textureFiles; //vector of text file names

	bool loadObjAssimp(const std::string & objFile);
	void loadTextures(const std::string & objFile);

};

#endif /* OBJECT_H */
