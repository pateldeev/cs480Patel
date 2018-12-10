#ifndef CONFIGLOADER_HPP
#define CONFIGLOADER_HPP

#include "graphics_headers.h"

#include <sstream>
#include <vector>

//class to parse configuration file of paramters
class ConfigFileParser {

public:
	ConfigFileParser(const std::string & configFile);
	~ConfigFileParser(void);

	//ConfigFileParser is not meant to be copied or moved
	ConfigFileParser(const ConfigFileParser &) = delete;
	ConfigFileParser(ConfigFileParser &&) = delete;
	ConfigFileParser& operator=(const ConfigFileParser &) = delete;
	ConfigFileParser& operator=(ConfigFileParser &&) = delete;

	void GetWindowInfo(std::string & windowName, glm::uvec2 & size);

	void GetCameraInfo(glm::vec3 & eyePos, glm::vec3 & eyeLoc);

	void GetMenuState(bool & menu, glm::uvec2 & size);

	void GetGameInfo(GameInfo & game);

	void GetShaderSet(std::string & shaderSetName, std::string & vertexShaderFile, std::string & fragmentShaderFile);

	void GetShaderSetActive(std::string & set);

private:
	std::stringstream m_fileBuffer;

	//internal function to get variable name and value in a line. Returns false if file is at end and line could not be read 
	template<typename T, unsigned int NUMVALUES = 1, char SEPERATOR = ','>
	inline bool ParseLine(std::string & variableName, T * variableValue) {
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
