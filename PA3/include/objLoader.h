#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>

#include "graphics_headers.h"

namespace ObjLoader {

void loadFile(const char * fileName, std::vector<Vertex> & Vertices, std::vector<unsigned int> & Indices, const glm::vec3 & color);

};

#endif /* OBJLOADER_H */
