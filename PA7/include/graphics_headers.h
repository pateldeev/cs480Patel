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

#define INVALID_UNIFORM_LOCATION 0x7fffffff

struct Vertex {
	glm::vec3 m_vertex;
	glm::vec2 m_texture;

	Vertex(const glm::vec3 & vertex, const glm::vec2 & texture) :
			m_vertex(vertex), m_texture(texture) {
	}
};

#endif /* GRAPHICS_HEADERS_H */
