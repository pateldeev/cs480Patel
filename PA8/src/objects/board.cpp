#include "objects/board.h"

Board::Board(const std::string & objFile, const glm::vec3 & translation, const glm::vec3 & rotationAngles, const glm::vec3 & scale) :
		Object(objFile, translation, rotationAngles, scale) {
}

Board::~Board(void) {

}

void Board::EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass) {

#if USE_COMPLEX_BOARD_MESH //use mesh
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

#else //use plane colliders
	//bottom floor
	AddPlane(dynamicsWorld, btVector3(0, 1, 0), btScalar(0));

	//side walls
	AddPlane(dynamicsWorld, btVector3(1, 0, 0), btScalar(-36));
	AddPlane(dynamicsWorld, btVector3(-1, 0, 0), btScalar(-38));
	AddPlane(dynamicsWorld, btVector3(0, 0, 1), btScalar(-19));
	AddPlane(dynamicsWorld, btVector3(0, 0, -1), btScalar(-20));

	//top
	AddPlane(dynamicsWorld, btVector3(0, -1, 0), btScalar(-15));
#endif
}

void Board::AddPlane(btDiscreteDynamicsWorld * dynamicsWorld, const btVector3 & normal, const btScalar & offset, const float friction) {
	btCollisionShape * shape = new btStaticPlaneShape(normal, offset);
	btDefaultMotionState * motionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(btScalar(0), motionState, shape, btVector3(0, 0, 0));

	rigidBodyCI.m_friction = rigidBodyCI.m_rollingFriction = rigidBodyCI.m_spinningFriction = friction;

	btRigidBody * rigidBody = new btRigidBody(rigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBody);
}
