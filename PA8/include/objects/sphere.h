#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere: public Object {
public:

	Sphere(const std::string & objFile, unsigned int mass, const glm::vec3 & translation = glm::vec3(0, 0, 0), const glm::vec3 & rotationAngles =
			glm::vec3(0, 0, 0), const glm::vec3 & scale = glm::vec3(1, 1, 1));
	~Sphere(void);

	void IntializeBt(btDiscreteDynamicsWorld * dynamicsWorld) override final;

	void Update(void) override final;

	void applyImpulse(const glm::vec3 & impulse, const glm::vec3 & spin = glm::vec3(0, 1, 1));

private:
	btRigidBody * mbt_rigidBody;
};

#endif /* SPHERE_H */
