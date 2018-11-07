#include "configLoader.hpp"

#include <fstream>

ConfigFileParser::ConfigFileParser(const std::string & configFile) {
	std::ifstream inputFile(configFile);

	if (!inputFile.is_open()) {
		printf("Could not open configuration file: %s \n", configFile.c_str());
		return;
	}

	m_fileBuffer << inputFile.rdbuf(); //load file data into buffer
	inputFile.close();

	//progress to first line with data
	std::string tempLine;
	while (!m_fileBuffer.eof() && !std::isalpha(m_fileBuffer.peek()))
		std::getline(m_fileBuffer, tempLine);

}

ConfigFileParser::~ConfigFileParser(void) {

}

bool ConfigFileParser::getWindowInfo(std::string & windowName, glm::uvec2 & size) {
	std::string varName;
	if (!parseLine < std::string > (varName, &windowName) || varName.compare("WINDOW_NAME")) {
		printf("Could not get window name from config file \n");
		return false;
	}

	bool fullscreen;
	if (!parseLine<bool>(varName, &fullscreen) || varName.compare("WINDOW_FULLSCREEN")) {
		printf("Could not get window fullscreen state from config file \n");
		return false;
	}

	//get window size
	unsigned int winSize[2];
	if (!parseLine<unsigned int, 2>(varName, winSize) || varName.compare("WINDOW_SIZE")) {
		printf("Could not get window size from config file \n");
		return false;
	}

	size = (fullscreen) ? glm::uvec2(0, 0) : glm::uvec2(winSize[0], winSize[1]); //set windowsize to 0 if in fullscreen

	return true;
}

bool ConfigFileParser::getCameraInfo(glm::vec3 & eyePos, glm::vec3 & eyeLoc) {
	std::string varName;
	float values[3];

	//get eye position
	if (!parseLine<float, 3>(varName, values) || varName.compare("CAMERA_EYEPOS")) {
		printf("Could not get eye position from config file \n");
		return false;
	}
	eyePos = glm::vec3(values[0], values[1], values[2]);

	//get eye focus
	if (!parseLine<float, 3>(varName, values) || varName.compare("CAMERA_FOCUSPOS")) {
		printf("Could not get eye focus position from config file \n");
		return false;
	}
	eyeLoc = glm::vec3(values[0], values[1], values[2]);

	return true;
}

bool ConfigFileParser::getShaderSet(std::string & shaderSetName, std::string & vertexShaderFile, std::string & fragmentShaderFile) {
	if (m_fileBuffer.eof() || m_fileBuffer.peek() != 'S')
		return false; //return false if there is no shader set to parse

	std::string varName;

	//get shader set name
	if (!parseLine < std::string > (varName, &shaderSetName) || varName.compare("SHADER_NAMES")) {
		printf("Could not get shader set name from config file \n");
		return false;
	}

	//get vertex shader name
	if (!parseLine < std::string > (varName, &vertexShaderFile) || varName.compare("VERTEX_SHADER")) {
		printf("Could not get vertex shader file from config file \n");
		return false;
	}

	//get fragment shader name
	if (!parseLine < std::string > (varName, &fragmentShaderFile) || varName.compare("FRAGMENT_SHADER")) {
		printf("Could not get fragment shader file from config file \n");
		return false;
	}
	return true;
}

bool ConfigFileParser::getShaderSetActive(std::string & set) {
	std::string varName;
	//get shader set name
	if (!parseLine < std::string > (varName, &set) || varName.compare("DEFAULT_SET")) {
		printf("Could not get active shader set from config file \n");
		return false;
	}
	return true;
}

bool ConfigFileParser::getMenuState(bool & menu, glm::uvec2 & size) {
	std::string varName;
	if (!parseLine<bool>(varName, &menu) || varName.compare("ENABLE_MENU")) {
		printf("Could not get menu state from config file \n");
		return false;
	}

	//get windowsize of menu
	unsigned int winSize[2];
	if (!parseLine<unsigned int, 2>(varName, winSize) || varName.compare("MENU_SIZE")) {
		printf("Could not get menu size from config file \n");
		return false;
	}
	size = glm::uvec2(winSize[0], winSize[1]);

	return true;
}

bool ConfigFileParser::getWorldGravity(glm::vec3 & gravity) {
	std::string varName;
	float values[3];

	//gravity
	if (!parseLine<float, 3>(varName, values) || varName.compare("WORLD_GRAVITY")) {
		printf("Could not get gravity from config file \n");
		return false;
	}
	gravity = glm::vec3(values[0], values[1], values[2]);
	return true;
}

bool ConfigFileParser::getObject(objectModel & obj) {
	if (m_fileBuffer.eof() || m_fileBuffer.peek() != 'O')
		return false; //return false if there is no object to parse

	std::string varName;
	float values[3];

	//get name
	if (!parseLine < std::string > (varName, &obj.name) || varName.compare("OBJ_NAME")) {
		printf("Could not get object name from config file \n");
		return false;
	}

	//get type
	if (!parseLine < std::string > (varName, &obj.btType) || varName.compare("BT_TYPE")) {
		printf("Could not get object type from config file \n");
		return false;
	}

	//get obj file name
	if (!parseLine < std::string > (varName, &obj.objFile) || varName.compare("OBJ_FILE")) {
		printf("Could not get object name from config file \n");
		return false;
	}

	//get starting location
	if (!parseLine<float, 3>(varName, values) || varName.compare("OBJ_STARTING_LOC")) {
		printf("Could not get object starting location from config file \n");
		return false;
	}
	obj.startingLoc = glm::vec3(values[0], values[1], values[2]);

	//get scale
	if (!parseLine<float, 3>(varName, values) || varName.compare("OBJ_SCALE")) {
		printf("Could not get object scale from config file \n");
		return false;
	}
	obj.scale = glm::vec3(values[0], values[1], values[2]);

	//get rotation
	if (!parseLine<float, 3>(varName, values) || varName.compare("OBJ_ROTATION")) {
		printf("Could not get object rotation from config file \n");
		return false;
	}
	obj.rotation = glm::vec3(values[0], values[1], values[2]);

	//get mass
	if (!parseLine<unsigned int>(varName, &obj.mass) || varName.compare("OBJ_MASS")) {
		printf("Could not get bullet object mass from config file \n");
		return false;
	}
	return true;
}
