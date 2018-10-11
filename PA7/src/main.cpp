#include "engine.h"

#include <iostream>

//function to parse command line arguments and get name of launch file
bool GetLaunchFile(int argc, char * argv[], std::string & launchFile);

//initializes engine with give parameter values
Engine * StartEngine(const std::string & launchFile);

int main(int argc, char * argv[]) {

	std::string launchFile;

	if (!GetLaunchFile(argc, argv, launchFile)) {
		std::cerr << "Error getting paramter values " << std::endl;
		return 0;
	}

	Engine * engine = StartEngine(launchFile);

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
Engine * StartEngine(const std::string & launchFile) {

	// Start engine
	Engine * engine = new Engine(launchFile);
	if (!engine->Initialize()) {
		std::cout << std::endl << "The engine failed to start." << std::endl;
		delete engine;
		engine = nullptr;
	}

	return engine;
}

bool GetLaunchFile(int argc, char * argv[], std::string & launchFile) {
	if (argc == 1) {
		launchFile = "launch/DefaultConfig.txt"; //default launch file 
	} else if (argc == 3 && *argv[1] == '-' && *(argv[1] + 1) == 'l') {
		launchFile = argv[2];
	} else {
		std::cout << std::endl << "Could not understand command line arguments!" << std::endl
				<< "You can run the program without any flags or provide a configuration file with the -l flag. See REAME.MD" << std::endl;
		return false;
	}

	return true;
}
