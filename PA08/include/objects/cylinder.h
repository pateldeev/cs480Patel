#ifndef CYLINDER_H
#define CYLINDER_H

#include "objects/object.h"

class Cylinder: public Object {
public:
	Cylinder(const std::string & objFile, const glm::vec3 & traslation = glm::vec3(0, 0, 0), const glm::vec3 & rotation = glm::vec3(0, 0, 0),
			const glm::vec3 & scale = glm::vec3(1, 1, 1));
	~Cylinder(void);

	void EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass = 0) override final;
};

#endif /* CYLINDER_H */
