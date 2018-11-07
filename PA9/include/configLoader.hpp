#ifndef CONFIGLOADER_HPP
#define CONFIGLOADER_HPP

#include <glm/glm.hpp>

#include <sstream>

//holds data related to each object
struct objectModel {
	std::string name;
	std::string btType;
	std::string objFile;
	glm::vec3 startingLoc;
	glm::vec3 scale;
	glm::vec3 rotation;
	unsigned int mass;
};

//class to parse configuration file of paramters
class ConfigFileParser {

public:
	ConfigFileParser(const std::string & configFile);
	~ConfigFileParser(void);

	bool getWindowInfo(std::string & windowName, glm::uvec2 & size);

	bool getCameraInfo(glm::vec3 & eyePos, glm::vec3 & eyeLoc);

	bool getShaderSet(std::string & shaderSetName, std::string & vertexShaderFile, std::string & fragmentShaderFile);

	bool getShaderSetActive(std::string & set);

	bool getMenuState(bool & menu, glm::uvec2 & size);
	
	bool getWorldGravity(glm::vec3 & gravity);

	bool getObject(objectModel & obj);

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
