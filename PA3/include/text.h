#ifndef TEXT_H
#define TEXT_H

#include "graphics_headers.h"
#include "objLoader.h"

class Text {
public:
	Text(const char * fileName, const glm::vec3 & color);
	~Text(void);
	void Update(const glm::vec3 & translation, const glm::vec3 & scale);
	void Render(void);
	glm::mat4 GetModel(void);

private:
	glm::mat4 m_model;
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	GLuint VB;
	GLuint IB;
};

#endif /* TEXT_H */
