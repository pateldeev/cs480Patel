#ifndef CUBE_H
#define CUBE_H

#include "object.h"

class Cube : Object {
public:
	Cube(const std::string & objFile);
	~Cube(void);
	void Update(unsigned int dt);
	void Render(void);
};

#endif /* CUBE_H */