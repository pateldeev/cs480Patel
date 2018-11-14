#ifndef SPHERE_H
#define SPHERE_H

#include "objects/object.h"

class Sphere: public Object {
public:

	Sphere(const std::string & objFile, const glm::vec3 & traslation = glm::vec3(0, 0, 0), const glm::vec3 & rotation = glm::vec3(0, 0, 0),
			const glm::vec3 & scale = glm::vec3(1, 1, 1));
	~Sphere(void);

	void EnableBt(btDiscreteDynamicsWorld * dynamicsWorld, unsigned int mass, float restitution, float friction) override final;
private:

#if DEBUG
        void DrawDebug(void); 
#endif
};

#endif /* SPHERE_H */
