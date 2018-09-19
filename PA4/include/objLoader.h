#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>

#include "graphics_headers.h"

namespace ObjLoader {

void loadObject(const std::string & objFileName, std::vector<Vertex> & vertices, std::vector<unsigned int> & indices);

};

#endif /* OBJLOADER_H */
