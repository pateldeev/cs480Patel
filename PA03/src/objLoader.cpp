#include "objLoader.h"

#include <fstream>
#include <sstream> 
#include <iostream>

void ObjLoader::loadFile(const char * fileName, std::vector<Vertex> & Vertices, std::vector<unsigned int> & Indices, const glm::vec3 & color) {

	std::ifstream file;
	file.open(fileName);

	if (!file.is_open()) {
		printf("ERR: Could not find obj file: %s \n", fileName);
		return;
	}

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	std::string temp;
	char tempChar;

	//remove extra starting info
	temp = ss.str();
	ss.str(temp.substr(temp.find("v ")));

	//read vertecies data
	float f1, f2, f3;
	ss >> tempChar;
	while (tempChar == 'v') {
		ss >> f1 >> f2 >> f3;
		Vertices.push_back(Vertex( { f1, f2, f3 }, color));
		ss >> tempChar;
	}

	//remove extra info between vertex and face data
	temp = ss.str();
	ss.str(temp.substr(temp.find("f ")));

	//read face data
	int i1, i2, i3;
	ss >> tempChar;
	while (tempChar == 'f') {
		ss >> i1 >> i2 >> i3;
		Indices.push_back(i1);
		Indices.push_back(i2);
		Indices.push_back(i3);

		if (ss.eof())
			break;

		ss >> tempChar;
	}
}
