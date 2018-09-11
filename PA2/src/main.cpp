#include "engine.h"

struct params {
	std::string vertexFile, fragmentFile, winName;
	bool menu;
	int winWidth, winHeight;
	glm::vec3 eyePos;

	//default parameters
	params() :
			vertexFile("shaders/vertShader.vert"), fragmentFile("shaders/fragShader.frag"), winName("PA02_Deev_Patel"), menu(false), winWidth(1600), winHeight(
					1200), eyePos(0.0, 8.0, -16.0) {
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

	//Set shader filenames
	Shader::SetVertexFile(paramVals.vertexFile);
	Shader::SetFragmentFile(paramVals.fragmentFile);

	// Start an engine and run it then cleanup after
	Engine *engine = new Engine(paramVals.winName, paramVals.winWidth, paramVals.winHeight);
	if (!engine->Initialize(paramVals.eyePos, paramVals.menu)) {
		printf("The engine failed to start.\n");
		delete engine;
		engine = NULL;
		return 1;
	}

	engine->Run();

	delete engine;
	engine = NULL;

	return 0;
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
		} else if (!std::strcmp(argv[i], "-winW")) {
			if (++i < argc) {
				parameters.winWidth = std::stoi(argv[i]);
			} else {
				std::cerr << "ERROR: value of -winW arguemnt could not be read" << std::endl;
				return false;
			}
		} else if (!std::strcmp(argv[i], "-winH")) {
			if (++i < argc) {
				parameters.winHeight = std::stoi(argv[i]);
			} else {
				std::cerr << "ERROR: value of -winH arguemnt could not be read" << std::endl;
				return false;
			}
		} else if (!std::strcmp(argv[i], "-winN")) {
			if (++i < argc) {
				parameters.winName = argv[i];
			} else {
				std::cerr << "ERROR: value of -winN arguemnt could not be read" << std::endl;
				return false;
			}
		} else if (!std::strcmp(argv[i], "-eyeX")) {
			if (++i < argc) {
				parameters.eyePos.x = std::atof(argv[i]);
			} else {
				std::cerr << "ERROR: value of -eyeX arguemnt could not be read" << std::endl;
				return false;
			}
		} else if (!std::strcmp(argv[i], "-eyeY")) {
			if (++i < argc) {
				parameters.eyePos.y = std::atof(argv[i]);
			} else {
				std::cerr << "ERROR: value of -eyeY arguemnt could not be read" << std::endl;
				return false;
			}
		} else if (!std::strcmp(argv[i], "-eyeZ")) {
			if (++i < argc) {
				parameters.eyePos.z = std::atof(argv[i]);
			} else {
				std::cerr << "ERROR: value of -eyeZ arguemnt could not be read" << std::endl;
				return false;
			}
		} else if (!std::strcmp(argv[i], "-m")) {
			parameters.menu = true;
		}
		else {
			std::cerr << "ERROR: Could not understand command line argument: " << argv[i] << std::endl;
			return false;
		}
	}
	return true;
}
