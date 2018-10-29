#include "objects/cube.h"

Cube::Cube(const std::string & objFile) : Object(objFile) {
      
}

Cube::~Cube(void) {
	m_vertices.clear();

	for (std::vector<unsigned int> & temp : m_indices)
		temp.clear();
	m_indices.clear();

	m_textures.clear();
}

void Cube::Update(unsigned int dt) {
	glm::mat4 rotationMat = glm::rotate((m_rotationAngles.x), glm::vec3(1.0, 0.0, 0.0)) * glm::rotate((m_rotationAngles.z), glm::vec3(0.0, 0.0, 1.0))
			* glm::rotate((m_rotationAngles.y), glm::vec3(0.0, 1.0, 0.0));

	m_model = glm::translate(m_translation) * rotationMat * glm::scale(m_scale);
}

void Cube::Render(void) {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, m_texture));

	for (int i = 0; i < IB.size(); ++i) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB[i]);

		glActiveTexture (GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		glDrawElements(GL_TRIANGLES, m_indices[i].size(), GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

