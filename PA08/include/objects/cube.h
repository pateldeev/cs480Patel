#ifndef CUBE_H
#define CUBE_H

#include "objects/object.h"

class Cube: public Object {
public:
	Cube(const std::string & objFile, const glm::vec3 & traslation = glm::vec3(0, 0, 0), const glm::vec3 & rotation = glm::vec3(0, 0, 0),
			const glm::vec3 & scale = glm::vec3(1, 1, 1));
	~Cube(void);

	void EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass = 0) override final;
};

#endif /* CUBE_H */
