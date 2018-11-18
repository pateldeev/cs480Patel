#include "engine.h"

#include <iostream>

//function to parse command line arguments and get name of launch file
bool GetLaunchFile(int argc, char * argv[], std::string & launchFile);

int main(int argc, char * argv[]) {

	std::string launchFile;

	if (!GetLaunchFile(argc, argv, launchFile)) {
		std::cerr << "Error getting paramter values " << std::endl;
		return -1;
	}

	try {
		Engine engine(launchFile);
		std::cout << std::endl << "Running Engine! " << std::endl;
		engine.Run();
		std::cout << std::endl << "Engine Done Running. Now exiting! " << std::endl;
	} catch (std::string & err) {
		std::cerr << std::endl << "The engine crashed: " << err << std::endl;
		return -1;
	} catch (...) {
		std::cerr << std::endl << "The engine crashed! Error is unknown " << std::endl;
		return -1;
	}

	return 0;
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
