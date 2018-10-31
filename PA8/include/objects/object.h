#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"

#include <assimp/Importer.hpp>
#include <vector>

#include <btBulletDynamicsCommon.h>

class Object {
public:
	Object(const std::string & objFile, const glm::vec3 & traslation = glm::vec3(0, 0, 0), const glm::vec3 & rotation = glm::vec3(0, 0, 0),
			const glm::vec3 & scale = glm::vec3(1, 1, 1));
	virtual ~Object(void);

	virtual void EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass = 0) = 0;

	void applyImpulse(const glm::vec3 & impulse, const glm::vec3 & spin = glm::vec3(1, 1, 1));
	void applyForce(const glm::vec3 & force, const glm::vec3 & spin = glm::vec3(1, 1, 1));
        void setLinearVelocity(const glm::vec3 & vel);

	void Update(void);
	void Render(void);

	glm::mat4 GetModel(void);

	glm::vec3 GetTranslation(void) const;
	void SetTranslation(const glm::vec3 & translation);
	void ResetBt(const glm::vec3 & loc);

	glm::vec3 GetRotationAngles(void) const;
	void SetRotationAngles(const glm::vec3 & rotation);

	glm::vec3 GetScale(void) const;
	void SetScale(const glm::vec3 & scale);

	btRigidBody * GetRigidBody(void);

	float GetDistanceFromPoint(glm::vec3 point) const;

protected:
	glm::vec3 m_translation;
	glm::vec3 m_rotationAngles;
	glm::vec3 m_scale;

	btRigidBody * mbt_rigidBody;
	btTriangleMesh * mbt_mesh;
	btCollisionShape * mbt_shape;

private:
	glm::mat4 m_model;

	std::vector<Vertex> m_vertices;
	std::vector<std::vector<unsigned int>> m_indices; //holds indicy set for each texture

	GLuint VB;
	std::vector<GLuint> IB; //vector of index set buffers

	std::vector<GLuint> m_textures; //vector of texture locations on GPU

	bool loadObjAssimp(const std::string & objFile);
	void loadTextures(const std::string & objFile, const std::vector<aiString> & textureFiles);
};

#include "objects/board.h"
#include "objects/cube.h"
#include "objects/cylinder.h"
#include "objects/sphere.h"


#endif /* OBJECT_H */
