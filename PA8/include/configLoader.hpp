#ifndef CONFIGLOADER_HPP
#define CONFIGLOADER_HPP

#include <glm/glm.hpp>

#include <sstream>

struct Planet {
	std::string name;
	std::string objFile;
	glm::vec3 modelScale;
	unsigned int numMoons;
	float moonMinDistance;
	glm::vec2 radius;
	float orbitSpeed;
	float rotationSpeed;
	glm::vec3 axialTilt;
};

//class to parse configuration file of paramters
class ConfigFileParser {

public:
	ConfigFileParser(const std::string & configFile);
	~ConfigFileParser(void);

	bool getShaderFileNames(std::string & vertexShaderFile, std::string & fragmentShaderFile);

	bool getMenuState(bool & menu);

	bool getWindowInfo(std::string & windowName);

	bool getCameraInfo(glm::vec3 & eyePos, glm::vec3 & eyeLoc);

	bool getMoonObjFile(std::string & objFile);

	bool getPlanetInfo(Planet & planet);

private:
	std::stringstream m_fileBuffer;

	//internal function to get variable name and value in a line. Returns false if file is at end and line could not be read 
	template<typename T, unsigned int NUMVALUES = 1, char SEPERATOR = ','>
	inline bool parseLine(std::string & variableName, T * variableValue) {
		if (m_fileBuffer.eof())
			return false;

		//use = sign to differeniate
		std::getline(m_fileBuffer, variableName, '=');

		char temp;
		if (NUMVALUES == 1) {
			m_fileBuffer >> variableValue[0];
		} else {
			for (unsigned int i = 0; i < NUMVALUES; ++i) {
				m_fileBuffer >> variableValue[i];

				if (i != NUMVALUES - 1) {
					if (m_fileBuffer.peek() != SEPERATOR)
						return false;

					m_fileBuffer >> temp;
				}
			}
		}

		//skip to next valid line or go to end of file
		std::string tempLine;
		while (!m_fileBuffer.eof() && !std::isalpha(m_fileBuffer.peek()))
			std::getline(m_fileBuffer, tempLine);

		return true;
	}
};

#endif /* CONFIGLOADER_HPP */
