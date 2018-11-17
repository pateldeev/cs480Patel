#include "objects/sphere.h"

Sphere::Sphere(const std::string & objFile, const glm::vec3 & translation, const glm::vec3 & rotationAngles, const glm::vec3 & scale) :
		Object(objFile, translation, rotationAngles, scale) {
}

Sphere::~Sphere(void) {

}

void Sphere::EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass) {
	mbt_shape = new btSphereShape(btScalar((m_scale.x + m_scale.y + m_scale.z) / 3));

	btQuaternion startRotations;
	startRotations.setEulerZYX(m_rotationAngles.z, m_rotationAngles.y, m_rotationAngles.x);
	btTransform startTransform(startRotations, btVector3(m_translation.x, m_translation.y, m_translation.z));
	btDefaultMotionState * shapeMotionState = new btDefaultMotionState(startTransform);

	btVector3 inertia(0, 0, 0);
	mbt_shape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, mbt_shape, inertia);
	mbt_rigidBody = new btRigidBody(shapeRigidBodyCI);
	dynamicsWorld->addRigidBody(mbt_rigidBody);

	mbt_rigidBody->setSleepingThresholds(0, 0);
}
