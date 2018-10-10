#include "engine.h"

#include <iostream>

//struct for changable parameters
struct params {
	std::string objFile;
	std::string vertexFile, fragmentFile;
	std::string winName;
	int winWidth, winHeight;
	glm::vec3 eyePos;
	bool menu;

	//default parameters
	params() :
			objFile("objFiles/buddha/buddha.obj"), vertexFile("shaders/vertShader.vert"), fragmentFile("shaders/fragShader.frag"), winName(
					"PA06"), winWidth(1600), winHeight(1200), eyePos(0.0, 10.0, -25.0), menu(true) {
	}
};

//function to parse command line arguments and get parameter values
bool ParseCLArgs(int argc, char * argv[], params & parameters);

//initializes engine with give parameter values
Engine * StartEngine(const params & parameters);

int main(int argc, char * argv[]) {

	params paramVals;

	if (!ParseCLArgs(argc, argv, paramVals)) {
		std::cerr << "Error getting paramter values " << std::endl;
		return 0;
	}

	Engine * engine = StartEngine(paramVals);

	if (!engine) {
		std::cerr << "Error starting engine! " << std::endl;
		return 0;
	}

	engine->Run();

	delete engine;
	engine = nullptr;

	return 0;
}

//initializes engine with give parameter values
Engine * StartEngine(const params & parameters) {

	//Set shader filenames
	Shader::SetVertexFile(parameters.vertexFile);
	Shader::SetFragmentFile(parameters.fragmentFile);

	// Start an engine and run it then cleanup after
	Engine *engine = new Engine(parameters.winName, parameters.winWidth, parameters.winHeight);
	if (!engine->Initialize(parameters.eyePos, parameters.objFile, parameters.menu)) {
		printf("The engine failed to start.\n");
		delete engine;
		engine = nullptr;
	}

	return engine;
}

bool ParseCLArgs(int argc, char * argv[], params & parameters) {
	//get parameter values
	for (int i = 1; i < argc; ++i) {
		if (!std::strcmp(argv[i], "-v")) {
			if (++i < argc) {
				parameters.vertexFile = argv[i];
				parameters.vertexFile = "shaders/" + parameters.vertexFile;
			} else {
				std::cerr << "ERROR: value of -v arguemnt could not be read" << std::endl;
				return false;
			}
		} else if (!std::strcmp(argv[i], "-f")) {
			if (++i < argc) {
				parameters.fragmentFile = argv[i];
				parameters.fragmentFile = "shaders/" + parameters.fragmentFile;
			} else {
				std::cerr << "ERROR: value of -f arguemnt could not be read" << std::endl;
				return false;
			}
		} else if (!std::strcmp(argv[i], "-l")) {
			if (++i < argc) {
				parameters.objFile = argv[i];
			} else {
				std::cerr << "ERROR: value of -l arguemnt could not be read" << std::endl;
				return false;
			}
		} else if (!std::strcmp(argv[i], "-m")) {
			if (++i < argc) {
				parameters.menu = std::atof(argv[i]) != 0;
			} else {
				std::cerr << "ERROR: value of -m arguemnt could not be read" << std::endl;
				return false;
			}
		} else {
			std::cerr << "ERROR: Could not understand command line argument: " << argv[i] << std::endl;
			return false;
		}
	}
	return true;
}
