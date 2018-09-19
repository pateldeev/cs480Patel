#include "object.h"

Object::Object(const std::string & objFile) :
		m_model(1.0), m_translation(0.0, 0.0, 0.0), m_scale(1.0, 1.0, 1.0), m_rotationAngles(0.0, 0.0, 0.0) {

	//load object data
	ObjLoader::loadObject(objFile, Vertices, Indices);

	// The index works at a 0th index
	for (unsigned int i = 0; i < Indices.size(); ++i) {
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

void Object::Update(const glm::vec3 & translation, const glm::vec3 & scale, const glm::vec3 & rotationAngles) {
	m_translation = translation;
	m_scale = scale;
	m_rotationAngles = rotationAngles;

	glm::mat4 rotationMat = glm::rotate((rotationAngles.x), glm::vec3(1.0, 0.0, 0.0)) * glm::rotate((rotationAngles.z), glm::vec3(0.0, 0.0, 1.0))
			* glm::rotate((rotationAngles.y), glm::vec3(0.0, 1.0, 0.0));

	m_model = glm::translate(m_translation) * rotationMat * glm::scale(m_scale);
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
	return m_model;
}

glm::vec3 Object::GetTranslation(void) const {
	return m_translation;
}

glm::vec3 Object::GetScale(void) const {
	return m_scale;
}

glm::vec3 Object::GetRotationAngles(void) const {
	return m_rotationAngles;
}
