#include <iostream>

#include "engine.h"

int main(int argc, char **argv) {

	std::string vShaderFile = "shaders/vertShader.vert", fShaderFile = "shaders/fragShader.frag";

	for (int i = 1; i < argc; ++i) {

		std::string argument = argv[i];

		if (argument.find("vertexFile=") == 0) {
			vShaderFile = "shaders/" + argument.substr(argument.find('=') + 1);
		} else if (argument.find("fragmentFile=") == 0) {
			fShaderFile = "shaders/" + argument.substr(argument.find('=') + 1);
		} else {
			std::cerr << "could not understanding meaning command line argument: " << std::endl;
		}
	}

	// Start an engine and run it then cleanup after
	Engine *engine = new Engine("PA01 Deev Patel", 1600, 1200);
	if (!engine->Initialize(vShaderFile, fShaderFile)) {
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
