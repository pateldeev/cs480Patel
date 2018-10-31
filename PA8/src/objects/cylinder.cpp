#include "objects/cylinder.h"

Cylinder::Cylinder(const std::string & objFile, const glm::vec3 & translation, const glm::vec3 & rotationAngles, const glm::vec3 & scale) :
		Object(objFile, translation, rotationAngles, scale) {
}

Cylinder::~Cylinder(void) {

}

void Cylinder::EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass) {
	float width = 2 * ((m_scale.x + m_scale.y + m_scale.z) / 3);
	float radius = (m_scale.x + m_scale.y + m_scale.z) / 3;

	mbt_shape = new btCylinderShape(btVector3(radius, width, radius));

	btQuaternion startRotations;
	startRotations.setEulerZYX(m_rotationAngles.z, m_rotationAngles.y, m_rotationAngles.x);
	btTransform startTransform(startRotations, btVector3(m_translation.x, m_translation.y, m_translation.z));
	btDefaultMotionState * shapeMotionState = new btDefaultMotionState(startTransform);

	btVector3 inertia(0, 0, 0);
	mbt_shape->calculateLocalInertia(0, inertia);

	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(0, shapeMotionState, mbt_shape, inertia);
	mbt_rigidBody = new btRigidBody(shapeRigidBodyCI);
	dynamicsWorld->addRigidBody(mbt_rigidBody);
}

