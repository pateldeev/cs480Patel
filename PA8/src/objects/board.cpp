#include "objects/board.h"

Board::Board(const std::string & objFile, const glm::vec3 & translation, const glm::vec3 & rotationAngles, const glm::vec3 & scale) :
		Object(objFile, translation, rotationAngles, scale) {
}

Board::~Board(void) {

}


void Board::EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass) {
#define USEMESH 0 //use mesh or plane colliders

#if USEMESH
        btScaledBvhTriangleMeshShape* mesh = new btScaledBvhTriangleMeshShape(new btBvhTriangleMeshShape(mbt_mesh, true, true), btVector3(m_scale.x, m_scale.y, m_scale.z));
	btQuaternion startRotations;
	startRotations.setEulerZYX(m_rotationAngles.z, m_rotationAngles.y, m_rotationAngles.x);
	btTransform startTransform(startRotations, btVector3(m_translation.x, m_translation.y, m_translation.z));
	btDefaultMotionState * shapeMotionState = new btDefaultMotionState(startTransform);
                
	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(btScalar(0), shapeMotionState, mesh, btVector3(0, 0, 0));
	//shapeRigidBodyCI.m_friction = 000;
	//shapeRigidBodyCI.m_rollingFriction = 000;
	//shapeRigidBodyCI.m_spinningFriction = 000;
        mbt_shape = mesh;
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
	btCollisionShape * wallL = new btStaticPlaneShape(btVector3(1, 0, 0), btScalar(-36));
	btDefaultMotionState * wallLMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo wallLRigidBodyCI(btScalar(0), wallLMotionState, wallL, btVector3(0, 0, 0));
	wallLRigidBodyCI.m_friction = 100;
	wallLRigidBodyCI.m_rollingFriction = 100;
	wallLRigidBodyCI.m_spinningFriction = 100;
	btRigidBody * rigidBodyWallL = new btRigidBody(wallLRigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBodyWallL);

	btCollisionShape * wallR = new btStaticPlaneShape(btVector3(-1, 0, 0), btScalar(-38));
	btDefaultMotionState * wallRMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo wallRRigidBodyCI(btScalar(0), wallRMotionState, wallR, btVector3(0, 0, 0));
	wallRRigidBodyCI.m_friction = 100;
	wallRRigidBodyCI.m_rollingFriction = 100;
	wallRRigidBodyCI.m_spinningFriction = 100;
	btRigidBody * rigidBodyWallR = new btRigidBody(wallRRigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBodyWallR);

	btCollisionShape * wallU = new btStaticPlaneShape(btVector3(0, 0, 1), btScalar(-19));
	btDefaultMotionState * wallUMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo wallURigidBodyCI(btScalar(0), wallUMotionState, wallU, btVector3(0, 0, 0));
	wallURigidBodyCI.m_friction = 100;
	wallURigidBodyCI.m_rollingFriction = 100;
	wallURigidBodyCI.m_spinningFriction = 100;
	btRigidBody * rigidBodyWallU = new btRigidBody(wallURigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBodyWallU);

	btCollisionShape * wallD = new btStaticPlaneShape(btVector3(0, 0, -1), btScalar(-20));
	btDefaultMotionState * wallDMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo wallDRigidBodyCI(btScalar(0), wallDMotionState, wallD, btVector3(0, 0, 0));
	wallDRigidBodyCI.m_friction = 100;
	wallDRigidBodyCI.m_rollingFriction = 100;
	wallDRigidBodyCI.m_spinningFriction = 100;
	btRigidBody * rigidBodyWallD = new btRigidBody(wallDRigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBodyWallD);

	//top
	btCollisionShape * top = new btStaticPlaneShape(btVector3(0, -1, 0), btScalar(-15));
	btDefaultMotionState * topMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo topRigidBodyCI(btScalar(0), topMotionState, top, btVector3(0, 0, 0));
	topRigidBodyCI.m_friction = 100;
	topRigidBodyCI.m_rollingFriction = 100;
	topRigidBodyCI.m_spinningFriction = 100;
	btRigidBody * rigidBodyTop = new btRigidBody(topRigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBodyTop);

#endif
}
