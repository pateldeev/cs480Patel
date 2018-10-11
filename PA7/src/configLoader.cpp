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

	std::string tempLine;
	while (m_fileBuffer.peek() != 'V') {
		std::getline(m_fileBuffer, tempLine);

		if (m_fileBuffer.eof()) {
			printf("Configuration file '%s' is improperly formated /n", configFile.c_str());
			return;
		}
	}
}

ConfigFileParser::~ConfigFileParser(void) {

}

bool ConfigFileParser::getShaderFileNames(std::string & vertexShaderFile, std::string & fragmentShaderFile) {
	std::string varName;

	if (!parseLine < std::string > (varName, &vertexShaderFile) || varName.compare("VERTEX_SHADER_FILE")) {
		printf("Could not get vertex shader file from config file \n");
		return false;
	}

	if (!parseLine < std::string > (varName, &fragmentShaderFile) || varName.compare("FRAGMENT_SHADER_FILE")) {
		printf("Could not get fragment shader file from config file \n");
		return false;
	}
	return true;
}

bool ConfigFileParser::getMenuState(bool & menu) {
	std::string varName;
	if (!parseLine<bool>(varName, &menu) || varName.compare("ENABLE_MENU")) {
		printf("Could not get menu state from config file \n");
		return false;
	}
	return true;
}

bool ConfigFileParser::getWindowInfo(std::string & windowName, int & windowWidth, int & windowHeight) {
	std::string varName;
	if (!parseLine < std::string > (varName, &windowName) || varName.compare("WINDOW_NAME")) {
		printf("Could not get window name from config file \n");
		return false;
	}

	int windowSize[2];
	if (!parseLine<int, 2>(varName, windowSize) || varName.compare("WINDOW_SIZE_WIDTH_HEIGHT")) {
		printf("Could not get window size from config file \n");
		return false;
	}
	windowWidth = windowSize[0];
	windowHeight = windowSize[1];
	return true;
}

bool ConfigFileParser::getCameraInfo(glm::vec3 & eyePos, glm::vec3 & eyeLoc) {
	std::string varName;
	float values[3];

	if (!parseLine<float, 3>(varName, values) || varName.compare("CAMERA_EYEPOS")) {
		printf("Could not get eye position from config file \n");
		return false;
	}
	eyePos = glm::vec3(values[0], values[1], values[2]);

	if (!parseLine<float, 3>(varName, values) || varName.compare("CAMERA_FOCUSPOS")) {
		printf("Could not get eye focus position from config file \n");
		return false;
	}
	eyeLoc = glm::vec3(values[0], values[1], values[2]);

	return true;
}

bool ConfigFileParser::getObjFile(std::string & objFile) {
	std::string varName;
	if (!parseLine < std::string > (varName, &objFile) || varName.compare("OBJ_FILE")) {
		printf("Could not get obejct file name from config file \n");
		return false;
	}
	return true;
}
