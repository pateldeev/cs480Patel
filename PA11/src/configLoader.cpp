#include "configLoader.hpp"

#include <fstream>

ConfigFileParser::ConfigFileParser(const std::string & configFile) {
	std::ifstream inputFile(configFile);

	if (!inputFile.is_open())
		throw(std::string("Could not open configuration file: ") + configFile);

	m_fileBuffer << inputFile.rdbuf(); //load file data into buffer
	inputFile.close();

	//progress to first line with data
	std::string tempLine;
	while (!m_fileBuffer.eof() && !std::isalpha(m_fileBuffer.peek()))
		std::getline(m_fileBuffer, tempLine);

}

ConfigFileParser::~ConfigFileParser(void) {

}

void ConfigFileParser::GetWindowInfo(std::string & windowName, glm::uvec2 & size) {
	std::string varName;
	if (!ParseLine < std::string > (varName, &windowName) || varName.compare("WINDOW_NAME"))
		throw std::string("Could not get window name from config file!");

	bool fullscreen;
	if (!ParseLine<bool>(varName, &fullscreen) || varName.compare("WINDOW_FULLSCREEN"))
		throw std::string("Could not get window fullscreen state from config file");

	//get window size
	unsigned int winSize[2];
	if (!ParseLine<unsigned int, 2>(varName, winSize) || varName.compare("WINDOW_SIZE"))
		throw std::string("Could not get window size from config file");

	size = (fullscreen) ? glm::uvec2(0, 0) : glm::uvec2(winSize[0], winSize[1]); //set windowsize to 0 if in fullscreen
}

void ConfigFileParser::GetCameraInfo(glm::vec3 & eyePos, glm::vec3 & eyeLoc) {
	std::string varName;
	float values[3];

	//get eye position
	if (!ParseLine<float, 3>(varName, values) || varName.compare("CAMERA_EYEPOS"))
		std::string("Could not get eye position from config file");
	eyePos = glm::vec3(values[0], values[1], values[2]);

	//get eye focus
	if (!ParseLine<float, 3>(varName, values) || varName.compare("CAMERA_FOCUSPOS"))
		throw std::string("Could not get eye focus position from config file");
	eyeLoc = glm::vec3(values[0], values[1], values[2]);
}

void ConfigFileParser::GetMenuState(bool & menu, glm::uvec2 & size) {
	std::string varName;
	if (!ParseLine<bool>(varName, &menu) || varName.compare("ENABLE_MENU"))
		throw std::string("Could not get menu state from config file");

	//get windowsize of menu
	unsigned int winSize[2];
	if (!ParseLine<unsigned int, 2>(varName, winSize) || varName.compare("MENU_SIZE"))
		throw std::string("Could not get menu size from config file");
	size = glm::uvec2(winSize[0], winSize[1]);
}

boardInfo* ConfigFileParser::GetBoardInfo(void) {
	std::string varName;
	float valuesfl[3];
	unsigned valuesui[2];
	objectInfo objInfo;
	boardInfo * board = nullptr;

	//get object file
	if (!ParseLine < std::string > (varName, &objInfo.m_objFile) || varName.compare("OBJ_FILE"))
		throw std::string("Could not get object file from config file");

	//get scale
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("OBJ_SCALE"))
		throw std::string("Could not get object scale from config file");
	objInfo.m_scale = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get rotation
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("OBJ_ROTATION"))
		throw std::string("Could not get object rotation from config file");
	objInfo.m_rotation = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get diffuse product for lighting
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("OBJ_DIFFUSE"))
		throw std::string("Could not get diffuse products from config file ");
	objInfo.m_diffuseLevel = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get specular product for lighting
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("OBJ_SPECULAR"))
		throw std::string("Could not get specular products from config file");
	objInfo.m_specularLevel = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get shininess friction
	if (!ParseLine<float>(varName, &objInfo.m_shininess) || varName.compare("OBJ_SHININESS"))
		throw std::string("Could not get shiniess from config file");

	//get number of spotlights
	if (!ParseLine<unsigned int>(varName, valuesui) || varName.compare("NUM_SPOTLIGHTS"))
		throw std::string("Could not get number of spotlights from config file");
	board = new boardInfo(valuesui[0]);
	board->m_object = objInfo;

	//get spotlight locations
	for (unsigned int i = 0; i < valuesui[0]; ++i) {
		if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("SPOTLIGHT_LOC"))
			throw std::string("Could not get spotlight location from config file");
		board->m_spotlightLocs[i] = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);
	}

	//get ambient level
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("AMBIENT_LEVEL"))
		throw std::string("Could not get ambient level from config file");
	board->m_ambientLevel = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get board size
	if (!ParseLine<unsigned int, 2>(varName, valuesui) || varName.compare("BOARD_SIZE"))
		throw std::string("Could not get board size from config file");
	board->m_size = glm::uvec2(valuesui[0], valuesui[1]);

	//get board starting point and direction
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("STARTING_POINT"))
		throw std::string("Could not get starting point of board from config file");
	board->m_startingLoc = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("BOARD_DIRECTION_ROW"))
		throw std::string("Could not get board row direction from config file");
	board->m_directionRow = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("BOARD_DIRECTION_COL"))
		throw std::string("Could not get board column direction from config file");
	board->m_directionCol = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get texture files
	if (!ParseLine < std::string > (varName, &board->m_textureDead) || varName.compare("TEXTURE_DEAD"))
		throw std::string("Could not get texture for dead from config file");
	//get Player 1 textures
	if (!ParseLine < std::string > (varName, &board->m_texturesP1[0]) || varName.compare("TEXTURE_P1"))
		throw std::string("Could not get texture for P1 alive from config file");
	if (!ParseLine < std::string > (varName, &board->m_texturesP1[1]) || varName.compare("TEXTURE_P1_DYING"))
		throw std::string("Could not get texture for P1 dying from config file");
	if (!ParseLine < std::string > (varName, &board->m_texturesP1[2]) || varName.compare("TEXTURE_P1_MARKED"))
		throw std::string("Could not get texture for P1 marked from config file");
	//get Player 2 texures
	if (!ParseLine < std::string > (varName, &board->m_texturesP2[0]) || varName.compare("TEXTURE_P2"))
		throw std::string("Could not get texture for P2 alive from config file");
	if (!ParseLine < std::string > (varName, &board->m_texturesP2[1]) || varName.compare("TEXTURE_P2_DYING"))
		throw std::string("Could not get texture for P2 dying from config file");
	if (!ParseLine < std::string > (varName, &board->m_texturesP2[2]) || varName.compare("TEXTURE_P2_MARKED"))
		throw std::string("Could not get texture for P2 marked from config file");

	return board;
}

void ConfigFileParser::GetShaderSet(std::string & shaderSetName, std::string & vertexShaderFile, std::string & fragmentShaderFile) {
	if (m_fileBuffer.eof() || m_fileBuffer.peek() != 'N')
		throw false;

	std::string varName;

	//get shader set name
	if (!ParseLine < std::string > (varName, &shaderSetName) || varName.compare("NAME_SET"))
		throw std::string("Could not get shader set name from config file");

	//get vertex shader name
	if (!ParseLine < std::string > (varName, &vertexShaderFile) || varName.compare("VERTEX_SHADER"))
		throw std::string("Could not get vertex shader file from config file");

	//get fragment shader name
	if (!ParseLine < std::string > (varName, &fragmentShaderFile) || varName.compare("FRAGMENT_SHADER"))
		throw std::string("Could not get fragment shader file from config file");
}

void ConfigFileParser::GetShaderSetActive(std::string & set) {
	std::string varName;
	//get shader set name
	if (!ParseLine < std::string > (varName, &set) || varName.compare("DEFAULT_SET"))
		throw std::string("Could not get active shader set from config file");
}
