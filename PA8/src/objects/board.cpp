#include "objects/board.h"

Board::Board(const std::string & objFile, const glm::vec3 & translation, const glm::vec3 & rotationAngles, const glm::vec3 & scale) :
		Object(objFile, translation, rotationAngles, scale, true), mbt_rigidBody(nullptr) {
	SetMass(0);
}

Board::~Board(void) {
	m_vertices.clear();

	for (std::vector<unsigned int> & temp : m_indices)
		temp.clear();
	m_indices.clear();

	m_textures.clear();
}

void Board::IntializeBt(btDiscreteDynamicsWorld * dynamicsWorld) {
#if 1
	btBvhTriangleMeshShape * shape = new btBvhTriangleMeshShape(mbt_mesh, true);

	btQuaternion startRotations;
	startRotations.setEulerZYX(m_rotationAngles.z, m_rotationAngles.y, m_rotationAngles.x);
	btTransform startTransform(startRotations, btVector3(m_translation.x, m_translation.y, m_translation.z));
	btDefaultMotionState * shapeMotionState = new btDefaultMotionState(startTransform);
	btCollisionShape * shapeScaled = new btScaledBvhTriangleMeshShape(shape, btVector3(m_scale.x, m_scale.y, m_scale.z));

	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(m_mass, shapeMotionState, shapeScaled);

	mbt_rigidBody = new btRigidBody(shapeRigidBodyCI);
	dynamicsWorld->addRigidBody(mbt_rigidBody);
#else
	btCollisionShape * shape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btQuaternion startRotations;
	btTransform startTransform(startRotations, btVector3(0, 0, 0));
	btDefaultMotionState * shapeMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(0, shapeMotionState, shape);

	mbt_rigidBody = new btRigidBody(shapeRigidBodyCI);
	dynamicsWorld->addRigidBody(mbt_rigidBody);

#endif
	
	btTransform trans;
	btScalar m[16];

	mbt_rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	m_model = glm::make_mat4(m) * glm::scale(m_scale);
}

void Board::Update(void) {

}
