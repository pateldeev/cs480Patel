#include "text.h"

Text::Text(const char * fileName, const glm::vec3 & color) :
		m_model(1.0f) {

	ObjLoader::loadFile(fileName, Vertices, Indices, color);

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

Text::~Text(void) {
	Vertices.clear();
	Indices.clear();
}

void Text::Update(const glm::vec3 & translation, const glm::vec3 & scale) {
	m_model = glm::translate(translation) * glm::rotate((float) (M_PI), glm::vec3(0.0, 1.0, 0.0)) * glm::scale(scale);
}

void Text::Render(void) {

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

glm::mat4 Text::GetModel(void) {
	return m_model;
}
