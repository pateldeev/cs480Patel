#ifndef PADDLE_H
#define PADDLE_H

#include "objects/object.h"
#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h>

class Paddle: public Object {
public:
	Paddle(const std::string & objFile, const glm::vec3 & traslation = glm::vec3(0, 0, 0), const glm::vec3 & rotation = glm::vec3(0, 0, 0),
			const glm::vec3 & scale = glm::vec3(1, 1, 1));
	~Paddle(void);

	void EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass = 0) override final;

	void MoveUpR(void);
	void MoveUpL(void);
	void ResetPaddleR(void);
	void ResetPaddleL(void);
	void SetResetFlag(bool flag);

	bool GetResetFlag(void) const;

private:
	bool m_resetFlag;

#if DEBUG
	void DrawDebug(void);
#endif
};

#endif /* PADDLE_H */
