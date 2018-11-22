#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl3.h>
#include <OpenGL/GLU.h>
#else //linux as default
#include <GL/glew.h>
//#include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <string>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

typedef struct Vertex{
	glm::vec3 m_vertex;
	glm::vec3 m_normal;
	glm::vec2 m_texture;

	Vertex(const glm::vec3 & vertex, const glm::vec3 & normal, const glm::vec2 & texture) :
			m_vertex(vertex), m_normal(normal), m_texture(texture) {
	}
} Vertex;

typedef struct objectInfo {
		std::string m_objFile;
		glm::vec3 m_scale;
		glm::vec3 m_rotation;
		glm::vec3 m_diffuseLevel;
		glm::vec3 m_specularLevel;
		float m_shininess;
} objectInfo;


typedef struct boardInfo {
	objectInfo m_object;
	glm::uvec2 m_size;
	float m_objectDistance;
	std::string m_textureDead;
	std::string m_texturesP1[3]; //order: alive, dead, marked
	std::string m_texturesP2[3]; //order: alive, dead, marked
	glm::vec3 m_ambientLevel;
	unsigned int m_spotlightNum;
	glm::vec3 * m_spotlightLocs;

	boardInfo(unsigned int spotlightNum): m_spotlightNum(spotlightNum){
		m_spotlightLocs = new glm::vec3[m_spotlightNum];
	}

	~boardInfo(void){
		delete[] m_spotlightLocs;
	}
} boardInfo;

#endif /* GRAPHICS_HEADERS_H */
