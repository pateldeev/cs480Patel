#ifndef CONFIGLOADER_HPP
#define CONFIGLOADER_HPP

#include <glm/glm.hpp>

#include <sstream>

class ConfigFileParser {

public:
	ConfigFileParser(const std::string & configFile);
	~ConfigFileParser(void);

	bool getShaderFileNames(std::string & vertexShaderFile, std::string & fragmentShaderFile);

	bool getMenuState(bool & menu);

	bool getWindowInfo(std::string & windowName, int & windowWidth, int & windowHeight);

	bool getCameraInfo(glm::vec3 & eyePos, glm::vec3 & eyeLoc);

	bool getObjFile(std::string & objFile);

private:
	std::stringstream m_fileBuffer;

	//internal function to get variable name and value in a line. Returns false if file is at end and line could not be read 
	template<typename T, unsigned int NUMVALUES = 1, char SEPERATOR = ','>
	bool parseLine(std::string & variableName, T * variableValue) {
		if (m_fileBuffer.eof())
			return false;

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

		std::string tempLine;
		while (!m_fileBuffer.eof() && !std::isalpha(m_fileBuffer.peek()))
			std::getline(m_fileBuffer, tempLine);

		return true;
	}
};

#endif /* CONFIGLOADER_HPP */
