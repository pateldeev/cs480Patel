#ifndef BOARD_H
#define BOARD_H

#include "objects/object.h"
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h>

class Board: public Object {
public:
	Board(const std::string & objFile, const glm::vec3 & traslation = glm::vec3(0, 0, 0), const glm::vec3 & rotation = glm::vec3(0, 0, 0),
			const glm::vec3 & scale = glm::vec3(1, 1, 1));
	~Board(void);

	void EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass = 0) override final;

private:
	btRigidBody * AddPlane(btDiscreteDynamicsWorld * dynamicsWorld, const btVector3 & normal, const btScalar & offset, const float friction = 100);

#if DEBUG
	void DrawDebug(void);
#endif
};

#endif /* BOARD_H */
