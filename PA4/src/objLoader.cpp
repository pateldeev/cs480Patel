#include "objLoader.h"

#include <fstream>
#include <sstream> 

namespace ObjLoader {

	//for private helper functions
	namespace {
	
		bool loadVertices(std::ifstream & objFile, std::vector<Vertex> & vertices) {		
			//remove extra starting info
			std::string tempLine;
			while(std::getline(objFile, tempLine)){
				
				if(objFile.eof()){
					printf("ERROR got to end of obj file without finding necessasry data! \n");
					return false;
				}
				
				if(tempLine.size() > 2 && tempLine[0] == 'v' && tempLine[1] == ' '){
					break;
				}
			}
						
			//load verticies
			float x, y, z;
			while(tempLine.size() > 2 && tempLine[0] == 'v' && tempLine[1] == ' '){
				std::istringstream data(tempLine.substr(1));
				data >> x >> y >> z;
				vertices.push_back(Vertex( { x, y, z }, {std::rand() / float(RAND_MAX), std::rand() / float(RAND_MAX), std::rand() / float(RAND_MAX)}));
				std::getline(objFile, tempLine);
			}
			return true;
		}
		
		bool loadFaces(std::ifstream & objFile, std::vector<unsigned int> & indices) {		
			//remove extra starting info
			std::string tempLine;
			while(std::getline(objFile, tempLine)){
					
				if(objFile.eof()){
					return false;
				}
					
				if(tempLine.size() > 2 && tempLine[0] == 'f' && tempLine[1] == ' '){
						break;
				}						
			}
								
			//load faces
			unsigned int i1, i2, i3;
			while(tempLine.size() > 2 && tempLine[0] == 'f' && tempLine[1] == ' '){
				std::istringstream data(tempLine.substr(1));

				data >> i1;
				if(data.peek() == '/'){
					std::string extraInfo;
					data >> extraInfo;								
				}
									
				data >> i2;
				if(data.peek() == '/'){
					std::string extraInfo;
					data >> extraInfo;
				}
													
				data >> i3;
				
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);

				std::getline(objFile, tempLine);
			}
			
			if(objFile.eof()){
				return false;
			}
			
			return true;
		}
	}
	
	void loadObject(const std::string & objFileName, std::vector<Vertex> & vertices, std::vector<unsigned int> & indices){
		std::ifstream objFile;
		objFile.open(objFileName.c_str());

		if (!objFile.is_open()) {
				printf("ERR: Could not find obj file: %s \n", objFileName.c_str());
				return;
		}
		
		loadVertices(objFile, vertices);
		
		//load all face data
		while(loadFaces(objFile, indices));
	}
	
	
#if 0
	void loadObject(const std::string & objFileName, const std::string & matFileName, std::vector<Vertex> & vertices,
			std::vector<unsigned int> & indices) {

		std::ifstream objFile;
		objFile.open(objFileName.c_str());

		if (!objFile.is_open()) {
			printf("ERR: Could not find obj file: %s \n", objFile);
			return;
		}

	
		
		ssObj.str(temp.substr(temp.find_first_of("v ")));

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
#endif

};
