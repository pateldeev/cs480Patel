#include "object.h"

Object::Object(float orbitalRadius, float rotationSpeed, float orbitSpeed) :
		angleRotation(0.0f), angleOrbit(0.0f), orbitRadius(orbitalRadius), orbitCenter(glm::vec3(0.0f, 0.0f, 0.0f)), orbitLoc(
				glm::vec3(0.0f, 0.0f, 0.0f)), objectScale(glm::vec3(1.0f, 1.0f, 1.0f)), speedRotation(rotationSpeed), speedOrbit(orbitSpeed), pauseRotation(
				false), pauseOrbit(false) {
	/*
	 # Blender File for a Cube
	 o Cube
	 v 1.000000 -1.000000 -1.000000
	 v 1.000000 -1.000000 1.000000
	 v -1.000000 -1.000000 1.000000
	 v -1.000000 -1.000000 -1.000000
	 v 1.000000 1.000000 -0.999999
	 v 0.999999 1.000000 1.000001
	 v -1.000000 1.000000 1.000000
	 v -1.000000 1.000000 -1.000000
	 s off
	 f 2 3 4
	 f 8 7 6
	 f 1 5 6
	 f 2 6 7
	 f 7 8 4
	 f 1 4 8
	 f 1 2 4
	 f 5 8 6
	 f 2 1 6
	 f 3 2 7
	 f 3 7 4
	 f 5 1 8
	 */

	Vertices = {
		{	{	1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
		{	{	1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{	{	-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{	{	-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
		{	{	1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
		{	{	1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
		{	{	-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
		{	{	-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
	};

	Indices = {
		2, 3, 4,
		8, 7, 6,
		1, 5, 6,
		2, 6, 7,
		7, 8, 4,
		1, 4, 8,
		1, 2, 4,
		5, 8, 6,
		2, 1, 6,
		3, 2, 7,
		3, 7, 4,
		5, 1, 8
	};

	// The index works at a 0th index
	for(unsigned int i = 0; i < Indices.size(); ++i) {
		--Indices[i];
	}

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object(void) {
	Vertices.clear();
	Indices.clear();
}

void Object::Update(unsigned int dt) {

	if (!pauseRotation)
		angleRotation += dt * M_PI * speedRotation;

	if (!pauseOrbit)
		angleOrbit += dt * M_PI * speedOrbit;

	if (!pauseRotation || !pauseOrbit) {
		orbitLoc = orbitCenter + glm::vec3(orbitRadius * std::cos(angleOrbit), 0.0 , orbitRadius * std::sin(angleOrbit));

		//model = glm::scale(objectScale); 
		//model = glm::rotate(model, (angleRotation), glm::vec3(0.0, 1.0, 0.0)); 
		//model = glm::translate(model, orbitLoc);
		
		model =  glm::translate(orbitLoc) * glm::rotate((angleRotation), glm::vec3(0.0, 1.0, 0.0)) * glm::scale(objectScale);
	}
}

void Object::Render(void) {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

glm::mat4 Object::GetModel(void) {
	return model;
}

void Object::SetRotationSpeed(float rotationSpeed) {
	speedRotation = rotationSpeed;
}

float Object::GetRotationSpeed(void) const {
	return speedRotation;
}

void Object::SetOrbitSpeed(float orbitSpeed) {
	speedOrbit = orbitSpeed;
}

float Object::GetOrbitSpeed(void) const {
	return speedOrbit;
}

void Object::ToggleRotation(bool rotate) {
	pauseRotation = !rotate;
}

bool Object::IsRotating(void) const {
	return !pauseRotation;
}

void Object::ToggleOrbit(bool orbit) {
	pauseOrbit = !orbit;
}

bool Object::IsOrbiting(void) const {
	return !pauseOrbit;
}

void Object::SetOrbitCenter(const glm::vec3 & center) {
	orbitCenter = center;
}

glm::vec3 Object::GetOrbitCenter(void) const {
	return orbitCenter;
}

glm::vec3 Object::GetOrbitLoc(void) const {
	return orbitLoc;
}

void Object::SetScale(const glm::vec3 & scale) {
	objectScale = scale;
}

glm::vec3 Object::GetScale(void) const {
	return objectScale;
}
