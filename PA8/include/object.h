#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"

#include <assimp/Importer.hpp>
#include <vector>

#include <btBulletDynamicsCommon.h>

class Object {
public:
	Object(const std::string & objFile, const glm::vec3 & translation = glm::vec3(0, 0, 0), const glm::vec3 & rotationAngles = glm::vec3(0, 0, 0),
			const glm::vec3 & scale = glm::vec3(1, 1, 1), bool loadBtMesh = false);
	virtual ~Object(void);

	virtual void Update(void) = 0;
	virtual void IntializeBt(btDiscreteDynamicsWorld * dynamicsWorld) = 0;

	virtual void Render(void);

	glm::mat4 GetModel(void);

	void SetName(const std::string & name);
	std::string GetName(void) const;

	float GetDistanceFromPoint(glm::vec3 point);
	unsigned int GetMass(void) const;

protected:
	unsigned int m_mass;
	void SetMass(unsigned int mass);

	std::string m_name;

	glm::mat4 m_model;

	glm::vec3 m_translation;
	glm::vec3 m_scale;
	glm::vec3 m_rotationAngles;

	std::vector<Vertex> m_vertices;
	std::vector<std::vector<unsigned int>> m_indices; //holds indicy set for each texture

	GLuint VB;
	std::vector<GLuint> IB; //vector of index set buffers

	std::vector<GLuint> m_textures; //vector of texture locations on GPU

	bool loadObjAssimp(const std::string & objFile, bool loadBtMesh);
	void loadTextures(const std::string & objFile, const std::vector<aiString> & textureFiles);

	btTriangleMesh * mbt_mesh;
};

//sub classes
#include "objects/board.h"
#include "objects/sphere.h"
#include "objects/cube.h"

#endif /* OBJECT_H */
