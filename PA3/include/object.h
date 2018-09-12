#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "graphics_headers.h"

class Object {
public:
	Object(float orbitalRadius, float rotationSpeed, float orbitSpeed);
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
	
	void SetOrbitCenter(const glm::vec3 & center);
	glm::vec3 GetOrbitCenter(void) const;
	glm::vec3 GetOrbitLoc(void) const;
	
	void SetScale(const glm::vec3 & scale);
	glm::vec3 GetScale(void) const;
	
private:
	glm::mat4 model;
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	GLuint VB;
	GLuint IB;

	float angleRotation;
	float angleOrbit;
	float orbitRadius;
	
	glm::vec3 orbitCenter; //center of object's orbital
	glm::vec3 orbitLoc; //position of object on the orbit 
	glm::vec3 objectScale; //scaling vector applied to object
	
	float speedRotation;
	float speedOrbit;
	bool pauseRotation;
	bool pauseOrbit;
};

#endif /* OBJECT_H */
