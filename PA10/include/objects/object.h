#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"

#include <assimp/Importer.hpp>
#include <vector>

#include <btBulletDynamicsCommon.h>

#if DEBUG
#include <LinearMath/btIDebugDraw.h>
#endif

class Object {
public:
	Object(const std::string & objFile, const glm::vec3 & traslation = glm::vec3(0, 0, 0), const glm::vec3 & rotation = glm::vec3(0, 0, 0),
			const glm::vec3 & scale = glm::vec3(1, 1, 1));
	virtual ~Object(void);

	virtual void EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass, float restitution, float friction) = 0;

	void applyImpulse(const glm::vec3 & impulse, const glm::vec3 & spin = glm::vec3(1, 1, 1));
	void applyForce(const glm::vec3 & force, const glm::vec3 & spin = glm::vec3(1, 1, 1));
	void applyTorque(const glm::vec3 & torque);
	void setLinearVelocity(const glm::vec3 & vel, bool accumulate = false);
	void setAngularVelocity(const glm::vec3 & vel, bool accumulate = false);
	void scaleVelocities(float scale);

	void Update(void);
	void Render(void);

	glm::mat4 GetModel(void);

	glm::vec3 GetTranslation(void) const;
	void SetTranslation(const glm::vec3 & translation);
	void ResetBt(const glm::vec3 & loc, const glm::vec3 & rotation);

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

#if DEBUG
	virtual void DrawDebug() = 0;
#endif

private:
	glm::mat4 m_model;

	std::vector<Vertex> m_vertices;
	std::vector<std::vector<unsigned int>> m_indices; //holds indicy set for each texture

	GLuint VB;
	std::vector<GLuint> IB; //vector of index set buffers

	std::vector<GLuint> m_textures; //vector of texture locations on GPU

	glm::vec3 m_kDiffuse;
	glm::vec3 m_kSpecular;

	bool loadObjAssimp(const std::string & objFile);
	void loadTextures(const std::string & objFile, const std::vector<aiString> & textureFiles);
};

#include "objects/board.h"
#include "objects/cube.h"
#include "objects/cylinder.h"
#include "objects/sphere.h"
#include "objects/paddle.h"
#include "objects/complex.h"

#endif /* OBJECT_H */
