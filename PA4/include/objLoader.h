#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>

#include "graphics_headers.h"

namespace ObjLoader {

void loadObjectRandColor(const std::string & objFileName, std::vector<Vertex> & vertices, std::vector<unsigned int> & indices);

void loadObjectWithColor(const std::string objFileName, const std::string mtlFileName, std::vector<Vertex> & verticies,
		std::vector<unsigned int> & indicies);

};

#endif /* OBJLOADER_H */
