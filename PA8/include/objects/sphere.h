#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere : Object {
public:
	Sphere(const std::string & objFile);
	~Sphere(void);
	void Update(unsigned int dt);
	void Render(void);
};

#endif /* SPHERE_H */