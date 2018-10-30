#include "objects/board.h"

Board::Board(const std::string & objFile, const glm::vec3 & translation, const glm::vec3 & rotationAngles, const glm::vec3 & scale) :
		Object(objFile, translation, rotationAngles, scale) {
}

Board::~Board(void) {

}

void Board::EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass) {

#define USEMESH 0 //use mesh or plane colliders

#if USEMESH
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

#else 
	//bottom floor
	btCollisionShape * floor = new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));
	btDefaultMotionState * floorMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo floorRigidBodyCI(btScalar(0), floorMotionState, floor, btVector3(0, 0, 0));
	floorRigidBodyCI.m_friction = 100;
	floorRigidBodyCI.m_rollingFriction = 100;
	floorRigidBodyCI.m_spinningFriction = 100;
	btRigidBody * rigidBodyFloor = new btRigidBody(floorRigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBodyFloor);

	//side walls
	btCollisionShape * wallL = new btStaticPlaneShape(btVector3(1, 0, 0), btScalar(-40));
	btDefaultMotionState * wallLMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo wallLRigidBodyCI(btScalar(0), wallLMotionState, wallL, btVector3(0, 0, 0));
	wallLRigidBodyCI.m_friction = 100;
	wallLRigidBodyCI.m_rollingFriction = 100;
	wallLRigidBodyCI.m_spinningFriction = 100;
	btRigidBody * rigidBodyWallL = new btRigidBody(wallLRigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBodyWallL);

#endif
}
