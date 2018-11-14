#ifndef COMPLEX_H
#define COMPLEX_H

#include "objects/object.h"
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h>

class Complex: public Object {
public:
	Complex(const std::string & objFile, const glm::vec3 & traslation = glm::vec3(0, 0, 0), const glm::vec3 & rotation = glm::vec3(0, 0, 0),
			const glm::vec3 & scale = glm::vec3(1, 1, 1));
	~Complex(void);

	void EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass, float restitution, float friction) override final;

private:
#if DEBUG
	void DrawDebug(void);
#endif
};

#endif /* COMPLEX_H */
