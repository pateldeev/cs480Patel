#include "objLoader.h"

#include <fstream>
#include <sstream> 

//for private helper functions
namespace {

bool loadVertices(std::stringstream & ss_obj, std::vector<Vertex> & vertices, bool randomColors = true, const glm::vec3 & defaultColor = { 1.0, 1.0,
		1.0 }) {
	//remove extra starting info and find first line with vertex data
	bool foundData = false;
	std::string temp;
	while (!foundData) {
		int nextChar = ss_obj.peek();

		if (nextChar == EOF) {
			printf("ERROR got to end of obj file without finding vertex data! Check file formatting. Could not load object \n");
			return false;
		} else if (nextChar == 'v') {
			ss_obj >> temp;
			if (temp.size() == 1)
				foundData = true; //found first vertex data line
		} else {
			std::getline(ss_obj, temp); //skip line and move on
		}
	}

	//load verticies
	float x, y, z;
	std::srand(time(0));
	bool continueReading = true;
	while (continueReading) {
		ss_obj >> x >> y >> z; //get vertex location

		//push back data
		vertices.push_back(
				Vertex( { x, y, z },
						((randomColors) ?
								glm::vec3(std::rand() / float(RAND_MAX), std::rand() / float(RAND_MAX), std::rand() / float(RAND_MAX)) : defaultColor)));

		//go to next line if not there
		if (!std::isalpha(ss_obj.peek()))
			std::getline(ss_obj, temp);

		if (ss_obj.peek() == 'v') {
			ss_obj >> temp;

			if (temp.size() != 1)
				continueReading = false; //reached end of vertex data
		} else {
			continueReading = false; //reached end of vertex data
		}
	}

	return true;
}

//load a set of faces and returns material associated with set. Should be called multiple times until it returns false
bool loadFaces(std::stringstream & ss_obj, std::vector<unsigned int> & indices, std::string & material) {
	//remove extra starting info and get to data
	bool foundData = false;
	std::string temp;
	while (!foundData) {
		int nextChar = ss_obj.peek();

		if (nextChar == EOF) {
			return false;
		} else if (nextChar == 'f') {
			ss_obj >> temp;
			if (temp.size() == 1)
				foundData = true; //found first face data line
		} else {
			std::getline(ss_obj, temp);

			//check and record material line
			if (temp.find_first_of("usemtl") == 0) {
				std::stringstream ss;
				ss.str(temp);
				ss >> temp;
				if (temp == "usemtl")
					ss >> material;
			}
		}
	}

	unsigned int i1, i2, i3;
	bool continueReading = true;
	while (continueReading) {

		ss_obj >> i1;
		if (ss_obj.peek() == '/') {
			std::string extraInfo;
			ss_obj >> extraInfo;
		}

		ss_obj >> i2;
		if (ss_obj.peek() == '/') {
			std::string extraInfo;
			ss_obj >> extraInfo;
		}

		ss_obj >> i3;

		//push face data - account for blendar starting at index 1
		indices.push_back(--i1);
		indices.push_back(--i2);
		indices.push_back(--i3);

		//go to next line if not there
		if (!std::isalpha(ss_obj.peek()))
			std::getline(ss_obj, temp);

		if (ss_obj.peek() == 'f') {
			ss_obj >> temp;

			if (temp.size() != 1)
				continueReading = false; //reached end of current face data set
		} else {
			continueReading = false; //reached end of vertex data
		}
	}

	return !ss_obj.eof();
}

glm::vec3 GetMaterialColor(std::ifstream & mtlFile, const std::string & materialName) {
	glm::vec3 color(0.0, 0.0, 0.0); //default color in case nothing is found

	mtlFile.clear();
	mtlFile.seekg(0, std::ios::beg);

	//remove extra starting info
	std::string temp = "";
	std::string searchFor = "newmtl " + materialName;
	while (temp != searchFor) {
		std::getline(mtlFile, temp);
		if (mtlFile.eof()) {
			printf("ERROR: Got to end of material file with finding material: %s \n   Returning default color (black) for that material \n",
					materialName.c_str());
			return color;
		}
	}

	//get to Kd value
	while (temp.substr(0, 2) != "Kd")
		std::getline(mtlFile, temp);

	std::stringstream ss;
	ss.str(temp.substr(2));

	float r, g, b;
	ss >> color.r >> color.g >> color.b;

	return color;
}

}

void ObjLoader::loadObjectRandColor(const std::string & objFileName, std::vector<Vertex> & vertices, std::vector<unsigned int> & indices) {
	std::ifstream objFile;
	objFile.open(objFileName.c_str());

	if (!objFile.is_open()) {
		printf("ERR: Could not find obj file: %s \n", objFileName.c_str());
		return;
	}

	std::stringstream ss_obj;
	ss_obj << objFile.rdbuf();
	objFile.close();

	loadVertices(ss_obj, vertices);

	std::string material;

	while (loadFaces(ss_obj, indices, material))
		; //load all face sets (per material) until there are no more
}

void ObjLoader::loadObjectWithColor(const std::string objFileName, const std::string mtlFileName, std::vector<Vertex> & verticies,
		std::vector<unsigned int> & indicies) {
	std::ifstream objFile, mtlFile;
	objFile.open(objFileName.c_str());
	mtlFile.open(mtlFileName.c_str());

	if (!objFile.is_open()) {
		printf("ERR: Could not find obj file: %s \n", objFileName.c_str());
		return;
	}
	if (!mtlFile.is_open()) {
		printf("ERR: Could not load mtl file: %s \n", mtlFileName.c_str());
		printf("Loading object with random colors \n");
		objFile.close();
		loadObjectRandColor(objFileName, verticies, indicies);
		return;
	}

	std::stringstream ss_obj;
	ss_obj << objFile.rdbuf();
	objFile.close();

	//store data in secondary location until color information can be added correctly
	std::vector < Vertex > rawVerticies;
	std::vector<unsigned int> rawIndicies;

	//load raw verticies data
	loadVertices(ss_obj, rawVerticies, false);

	bool continueReading = true;
	std::string material;
	while (continueReading) {
		rawIndicies.clear();
		continueReading = loadFaces(ss_obj, rawIndicies, material);

		glm::vec3 color = GetMaterialColor(mtlFile, material);

		//add each vertex and face with color - each new material must have new face
		for (unsigned int index : rawIndicies) {
			verticies.push_back(Vertex(rawVerticies[index].vertex, color));
			indicies.push_back(verticies.size() - 1);
		}
	}

	mtlFile.close();
}
