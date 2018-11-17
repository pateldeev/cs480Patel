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

bool ConfigFileParser::getWindowInfo(std::string & windowName) {
	std::string varName;
	if (!parseLine < std::string > (varName, &windowName) || varName.compare("WINDOW_NAME")) {
		printf("Could not get window name from config file \n");
		return false;
	}
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

bool ConfigFileParser::getMoonObjFile(std::string & objFile) {
	std::string varName;
	if (!parseLine < std::string > (varName, &objFile) || varName.compare("MOON_OBJ_FILE")) {
		printf("Could not get moon obj file name from information config file \n");
		return false;
	}
	return true;

}

bool ConfigFileParser::getPlanetInfo(Planet & planet) {
	std::string varName;

	//get planet name
	if (!parseLine < std::string > (varName, &planet.name) || varName.compare("PLANET_NAME"))
		return false;

	//get object file for planet
	if (!parseLine < std::string > (varName, &planet.objFile) || varName.compare("OBJ_FILE")) {
		printf("Could not get planet information config file \n");
		return false;
	}

	//get scale for model
	float values[3];
	if (!parseLine<float, 3>(varName, values) || varName.compare("MODEL_SCALE")) {
		printf("Could not get planet information config file \n");
		return false;
	}
	planet.modelScale = glm::vec3(values[0], values[1], values[2]);

	//get number of moons
	if (!parseLine<unsigned int>(varName, &planet.numMoons) || varName.compare("NUM_MOONS")) {
		printf("Could not get planet information config file \n");
		return false;
	}

	//get minimum distance of moon if needed
	if (planet.numMoons) {
		if (!parseLine<float>(varName, &planet.moonMinDistance) || varName.compare("MOON_MIN_DISTANCE")) {
			printf("Could not get planet information config file \n");
			return false;
		}
	}

	//get planet orbit information
	if (!parseLine<float, 2>(varName, values) || varName.compare("ORBIT_RADIUS_X_Z")) {
		printf("Could not get planet information config file \n");
		return false;
	}
	planet.radius = glm::vec2(values[0], values[1]);

	//get planet orbit speed
	if (!parseLine<float>(varName, &planet.orbitSpeed) || varName.compare("ORBIT_SPEED")) {
		printf("Could not get planet information config file \n");
		return false;
	}

	//get planet rotation speed
	if (!parseLine<float>(varName, &planet.rotationSpeed) || varName.compare("ROTATION_SPEED")) {
		printf("Could not get planet information config file \n");
		return false;
	}

	//get planet axial tilt
	if (!parseLine<float, 2>(varName, values) || varName.compare("AXIAL_TILT")) {
		printf("Could not get planet information config file \n");
		return false;
	}
	planet.axialTilt = glm::vec3(values[0], 0, values[1]);

	return true;
}
