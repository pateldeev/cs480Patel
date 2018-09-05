#include "engine.h"

int main(int argc, char * argv[]) {

	//default parameters
	std::string vertexFile = "shaders/vertShader.vert";
	std::string fragmentFile = "shaders/fragShader.frag";
	std::string winName = "PA01_Deev_Patel";
	int winWidth = 1600, winHeight = 1200;
	glm::vec3 eyePos(0.0, 8.0, -16.0);

	//get parameter values
	for (int i = 1; i < argc; ++i) {
		if (!std::strcmp(argv[i], "-v")) {
			if (++i < argc) {
				vertexFile = argv[i];
				vertexFile = "shaders/" + vertexFile;
			} else {
				std::cerr << "ERROR: value of -v arguemnt could not be read" << std::endl;
				return 0;
			}
		} else if (!std::strcmp(argv[i], "-f")) {
			if (++i < argc) {
				fragmentFile = argv[i];
				fragmentFile = "shaders/" + fragmentFile;
			} else {
				std::cerr << "ERROR: value of -f arguemnt could not be read" << std::endl;
				return 0;
			}
		} else if (!std::strcmp(argv[i], "-winW")) {
			if (++i < argc) {
				winWidth = std::stoi(argv[i]);
			} else {
				std::cerr << "ERROR: value of -winW arguemnt could not be read" << std::endl;
				return 0;
			}
		} else if (!std::strcmp(argv[i], "-winH")) {
			if (++i < argc) {
				winHeight = std::stoi(argv[i]);
			} else {
				std::cerr << "ERROR: value of -winH arguemnt could not be read" << std::endl;
				return 0;
			}
		} else if (!std::strcmp(argv[i], "-winN")) {
			if (++i < argc) {
				winName = argv[i];
			} else {
				std::cerr << "ERROR: value of -winN arguemnt could not be read" << std::endl;
				return 0;
			}
		} else if (!std::strcmp(argv[i], "-eyeX")) {
			if (++i < argc) {
				eyePos.x = std::atof(argv[i]);
			} else {
				std::cerr << "ERROR: value of -eyeX arguemnt could not be read" << std::endl;
				return 0;
			}
		} else if (!std::strcmp(argv[i], "-eyeY")) {
			if (++i < argc) {
				eyePos.y = std::atof(argv[i]);
			} else {
				std::cerr << "ERROR: value of -eyeY arguemnt could not be read" << std::endl;
				return 0;
			}
		} else if (!std::strcmp(argv[i], "-eyeZ")) {
			if (++i < argc) {
				eyePos.z = std::atof(argv[i]);
			} else {
				std::cerr << "ERROR: value of -eyeZ arguemnt could not be read" << std::endl;
				return 0;
			}
		} else {
			std::cerr << "ERROR: Could not understand command line argument: " << argv[i] << std::endl;
			return 0;
		}
	}

	//Set shader filenames
	Shader::SetVertexFile(vertexFile);
	Shader::SetFragmentFile(fragmentFile);

	// Start an engine and run it then cleanup after
	Engine *engine = new Engine(winName, winWidth, winHeight);
	if (!engine->Initialize(eyePos)) {
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
