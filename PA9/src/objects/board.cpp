#include "objects/board.h"

Board::Board(const std::string & objFile, const glm::vec3 & translation, const glm::vec3 & rotationAngles, const glm::vec3 & scale) :
		Object(objFile, translation, rotationAngles, scale) {
}

Board::~Board(void) {

}

void Board::EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass) {
#if USE_COMPLEX_BOARD_MESH //use mesh
	mbt_shape = new btScaledBvhTriangleMeshShape(new btBvhTriangleMeshShape(mbt_mesh, true, true), btVector3(m_scale.x, m_scale.y, m_scale.z));
	//mbt_shape->setLocalScaling();

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

//#if DEBUG
class GlDrawcallback : public btTriangleCallback
{
public:
	bool m_wireframe;

	GlDrawcallback()
		: m_wireframe(true)
	{
	}

	virtual void processTriangle(btVector3* triangle, int partId, int triangleIndex)
	{
		(void)triangleIndex;
		(void)partId;
                //this thing needs a speedy shader that just assigns the color 
                GLuint IB, VB;
                glGenBuffers(1, &VB);
                glBindBuffer(GL_ARRAY_BUFFER, VB);
                glm::vec3 verts[3];
                verts[0] = glm::vec3(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
                verts[1] = glm::vec3(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
                verts[2] = glm::vec3(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 3, &verts[0], GL_STATIC_DRAW);
                glGenBuffers(1, &IB);
                glBindBuffer(GL_ARRAY_BUFFER, IB);
                unsigned int indices[3] = {1, 2, 3};
                glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * 3, &indices[0], GL_STATIC_DRAW);
                
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, VB);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
                
                glDrawElements(GL_LINE_STRIP, 3, GL_UNSIGNED_INT, 0);
                
                glDisableVertexAttribArray(0);
                
	}
};
//#endif

void Board::DrawDebug() {
#if USE_COMPLEX_BOARD_MESH
    btConcaveShape* concaveMesh = (btConcaveShape*)mbt_shape;
    GlDrawcallback drawCallback;
    drawCallback.m_wireframe = false;
    btTransform trans;
    btVector3 aabbMin, aabbMax;
    mbt_shape->getAabb(trans, aabbMin, aabbMax);
    concaveMesh->processAllTriangles(&drawCallback, aabbMin, aabbMax);
#endif
}
