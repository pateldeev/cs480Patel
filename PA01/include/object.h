#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "graphics_headers.h"

class Object {
public:
	Object(float orbitalRadius = 4.0);
	~Object(void);
	void Update(unsigned int dt);
	void Render(void);

	glm::mat4 GetModel(void);

private:
	glm::mat4 model;
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	GLuint VB;
	GLuint IB;

	float angleYrot;
	float angleOrbit;
	float orbitRadius;
};

#endif /* OBJECT_H */
