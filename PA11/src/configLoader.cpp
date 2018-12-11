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

void ConfigFileParser::GetGameInfo(GameInfo & game) {
	std::string varName;
	float valuesfl[3];
	unsigned valuesui[2];

	//get object file
	if (!ParseLine < std::string > (varName, &game.m_object.m_objFile) || varName.compare("OBJ_FILE"))
		throw std::string("Could not get object file from config file");

	//get scale
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("OBJ_SCALE"))
		throw std::string("Could not get object scale from config file");
	game.m_object.m_scale = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get rotation
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("OBJ_ROTATION"))
		throw std::string("Could not get object rotation from config file");
	game.m_object.m_rotation = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get diffuse product for lighting
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("OBJ_DIFFUSE"))
		throw std::string("Could not get diffuse products from config file ");
	game.m_object.m_diffuseLevel = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get specular product for lighting
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("OBJ_SPECULAR"))
		throw std::string("Could not get specular products from config file");
	game.m_object.m_specularLevel = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get shininess friction
	if (!ParseLine<float>(varName, &game.m_object.m_shininess) || varName.compare("OBJ_SHININESS"))
		throw std::string("Could not get shiniess from config file");

	//get ambient level
	if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("AMBIENT_LEVEL"))
		throw std::string("Could not get ambient level from config file");
	game.m_ambientLevel = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);

	//get textures
	if (!ParseLine < std::string > (varName, &game.m_textures[0]) || varName.compare("TEXTURE_DEAD"))
		throw std::string("Could not get texture 0 for dead from config file");
	if (!ParseLine < std::string > (varName, &game.m_textures[1]) || varName.compare("TEXTURE_P1_ALIVE_FUTURE"))
		throw std::string("Could not get texture 1 for dead from config file");
	if (!ParseLine < std::string > (varName, &game.m_textures[2]) || varName.compare("TEXTURE_P2_ALIVE_FUTURE"))
		throw std::string("Could not get texture 2 for dead from config file");
	if (!ParseLine < std::string > (varName, &game.m_textures[3]) || varName.compare("TEXTURE_P1_ALIVE"))
		throw std::string("Could not get texture 3 for dead from config file");
	if (!ParseLine < std::string > (varName, &game.m_textures[4]) || varName.compare("TEXTURE_P2_ALIVE"))
		throw std::string("Could not get texture 4 for dead from config file");
	if (!ParseLine < std::string > (varName, &game.m_textures[5]) || varName.compare("TEXTURE_P1_DEAD_FUTURE"))
		throw std::string("Could not get texture 5 for dead from config file");
	if (!ParseLine < std::string > (varName, &game.m_textures[6]) || varName.compare("TEXTURE_P2_DEAD_FUTURE"))
		throw std::string("Could not get texture 6 for dead from config file");
	if (!ParseLine < std::string > (varName, &game.m_textures[7]) || varName.compare("TEXTURE_P1_ALIVE_MARKED"))
		throw std::string("Could not get texture 7 for dead from config file");
	if (!ParseLine < std::string > (varName, &game.m_textures[8]) || varName.compare("TEXTURE_P2_ALIVE_MARKED"))
		throw std::string("Could not get texture 8 for dead from config file");
	if (!ParseLine < std::string > (varName, &game.m_textures[9]) || varName.compare("TEXTURE_P1_DEAD_MARKED"))
		throw std::string("Could not get texture 9 for dead from config file");
	if (!ParseLine < std::string > (varName, &game.m_textures[10]) || varName.compare("TEXTURE_P2_DEAD_MARKED"))
		throw std::string("Could not get texture 10 for dead from config file");

	//get various sides of board
	for (unsigned int i = 0; i < BoardSides::NUM_SIDES; ++i) {

		//get board name
		if (!ParseLine<unsigned int>(varName, valuesui) || varName.compare("BOARD_SIDE") || valuesui[0] != i)
			throw std::string("Could not get board side" + std::to_string(i) + "from config file");

		//get board size
		if (!ParseLine<unsigned int, 2>(varName, valuesui) || varName.compare("BOARD_SIZE"))
			throw std::string("Could not get board size from config file");
		game.m_sides[i].m_size = glm::uvec2(valuesui[0], valuesui[1]);

		//get board starting point and direction
		if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("BOARD_START_POINT"))
			throw std::string("Could not get starting point of board from config file");
		game.m_sides[i].m_startingLoc = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);
		if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("BOARD_CHANGE_ROW"))
			throw std::string("Could not get board row direction from config file");
		game.m_sides[i].m_changeRow = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);
		if (!ParseLine<float, 3>(varName, valuesfl) || varName.compare("BOARD_CHANGE_COL"))
			throw std::string("Could not get board column direction from config file");
		game.m_sides[i].m_changeCol = glm::vec3(valuesfl[0], valuesfl[1], valuesfl[2]);
	}
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
