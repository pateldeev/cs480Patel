#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "graphics_headers.h"

class Object {
public:
	Object(float orbitalRadius = 4.0, float rotationSpeed = 0.001, float orbitSpeed = 0.0004);
	~Object(void);
	void Update(unsigned int dt);
	void Render(void);
	glm::mat4 GetModel(void);

	void SetRotationSpeed(float rotationSpeed);
	float GetRotationSpeed(void) const;
	void SetOrbitSpeed(float orbitSpeed);
	float GetOrbitSpeed(void) const;
	
	void ToggleRotation(bool rotate);
	bool IsRotating(void) const;
	void ToggleOrbit(bool orbit);
	bool IsOrbiting(void) const;
	
private:
	glm::mat4 model;
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	GLuint VB;
	GLuint IB;

	float angleRotation;
	float angleOrbit;
	float orbitRadius;
	float speedRotation;
	float speedOrbit;
	bool pauseRotation;
	bool pauseOrbit;
};

#endif /* OBJECT_H */
