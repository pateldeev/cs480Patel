#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"

#include <vector>

enum ObjType {
	DEAD = 0, P1 = 1, P2 = 2
};

class Object {
public:
	Object(void) = delete;
	Object(const std::string & objFile, unsigned int numIstances, const glm::vec3 & traslation = glm::vec3(0, 0, 0), const glm::vec3 & rotation =
			glm::vec3(0, 0, 0), const glm::vec3 & scale = glm::vec3(1, 1, 1));
	~Object(void);

//Object is not meant to be copied or moved
	Object(const Object &) = delete;
	Object(Object &&) = delete;
	Object& operator=(const Object &) = delete;
	Object& operator=(Object&&) = delete;

	void Update(void);
	void Render(void);

	glm::mat4 GetModel(void);

	glm::vec3 GetTranslation(void) const;
	void SetTranslation(const glm::vec3 & translation);

	glm::vec3 GetRotation(void) const;
	void SetRotation(const glm::vec3 & rotation);

	glm::vec3 GetScale(void) const;
	void SetScale(const glm::vec3 & scale);

	ObjType GetType(unsigned int id) const;
	std::vector<ObjType> & GetTypesList(void);
	void SetType(const ObjType type, unsigned int id);

	float GetDistanceFromPoint(glm::vec3 point) const;

	void LoadTexture(const std::string & textureFile, ObjType type = ObjType::DEAD);

private:
	void LoadObjAssimp(const std::string & objFile);

	glm::mat4 m_model;

	glm::vec3 m_translation;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	int m_numInstances;
	std::vector<ObjType> m_types;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	GLuint VB;
	GLuint IB;

	GLuint m_textures[3]; //vector of texture locations on GPU {0=dead, 1=P1, 2=P2}
};

#endif /* OBJECT_H */
