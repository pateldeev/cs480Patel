#ifndef CUBE_H
#define CUBE_H

#include "object.h"

class Cube: public Object {
public:
	Cube(const std::string & objFile, unsigned int mass, const glm::vec3 & translation = glm::vec3(0, 0, 0), const glm::vec3 & rotationAngles =
			glm::vec3(0, 0, 0), const glm::vec3 & scale = glm::vec3(1, 1, 1));
	~Cube(void);

	void IntializeBt(btDiscreteDynamicsWorld * dynamicsWorld) override final;

	void Update(void) override final;

private:
	btRigidBody * mbt_rigidBody;
};

#endif /* CUBE_H */
