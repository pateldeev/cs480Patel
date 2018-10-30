#ifndef BOARD_H
#define BOARD_H

#include "object.h"

class Board: public Object {
public:
	Board(const std::string & objFile, const glm::vec3 & translation = glm::vec3(0, 0, 0), const glm::vec3 & rotationAngles = glm::vec3(0, 0, 0),
			const glm::vec3 & scale = glm::vec3(1, 1, 1));
	~Board(void);

	void IntializeBt(btDiscreteDynamicsWorld * dynamicsWorld) override final;

	void Update(void) override final;

private:
	btRigidBody * mbt_rigidBody;
};

#endif /* BOARD_H */
