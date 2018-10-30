#include "objects/cube.h"

Cube::Cube(const std::string & objFile, unsigned int mass, const glm::vec3 & translation, const glm::vec3 & rotationAngles, const glm::vec3 & scale) :
		Object(objFile, translation, rotationAngles, scale), mbt_rigidBody(nullptr) {
	SetMass(mass);
}

Cube::~Cube(void) {
	m_vertices.clear();

	for (std::vector<unsigned int> & temp : m_indices)
		temp.clear();
	m_indices.clear();

	m_textures.clear();
}

void Cube::IntializeBt(btDiscreteDynamicsWorld * dynamicsWorld) {
	btCollisionShape * shape = new btBoxShape(btVector3(m_scale.x * 0.5, m_scale.y * 0.5, m_scale.z * 0.5));

	btQuaternion startRotations;
	startRotations.setEulerZYX(m_rotationAngles.z, m_rotationAngles.y, m_rotationAngles.x);
	btTransform startTransform(startRotations, btVector3(m_translation.x, m_translation.y, m_translation.z));
	btDefaultMotionState * shapeMotionState = new btDefaultMotionState(startTransform);

	btScalar mass(m_mass);
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);

	mbt_rigidBody = new btRigidBody(shapeRigidBodyCI);
	dynamicsWorld->addRigidBody(mbt_rigidBody);
}

#include <iostream>

void Cube::Update(void) {
	btTransform trans;
	btScalar m[16];

	mbt_rigidBody->getMotionState()->getWorldTransform(trans);

	if (trans.getOrigin().getY() < -1) {
		std::cout << "HERE" << std::endl;
		//mbt_rigidBody->translate(btVector3(0, 6, 0));
		//mbt_rigidBody->applyImpulse(btVector3(0, 10, 0), btVector3(0, 0, 0));
	}

	trans.getOpenGLMatrix(m);

	m_model = glm::make_mat4(m) * glm::scale(m_scale);
}

