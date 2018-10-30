#include "objects/board.h"

Board::Board(const std::string & objFile, const glm::vec3 & translation, const glm::vec3 & rotationAngles, const glm::vec3 & scale) :
		Object(objFile, translation, rotationAngles, scale) {
}

Board::~Board(void) {

}

void Board::EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass) {
	mbt_shape = new btBvhTriangleMeshShape(mbt_mesh, true);
	mbt_shape->setLocalScaling(btVector3(m_scale.x, m_scale.y, m_scale.z));

	btQuaternion startRotations;
	startRotations.setEulerZYX(m_rotationAngles.z, m_rotationAngles.y, m_rotationAngles.x);
	btTransform startTransform(startRotations, btVector3(m_translation.x, m_translation.y, m_translation.z));
	btDefaultMotionState * shapeMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(btScalar(0), shapeMotionState, mbt_shape, btVector3(0, 0, 0));
	//shapeRigidBodyCI.m_friction = 000;
	//shapeRigidBodyCI.m_rollingFriction = 000;
	//shapeRigidBodyCI.m_spinningFriction = 000;

	mbt_rigidBody = new btRigidBody(shapeRigidBodyCI);
	dynamicsWorld->addRigidBody(mbt_rigidBody);
}
