#include "board.h"

Board::Board(const boardInfo & board) :
		m_shaderCurrent(nullptr), m_boardSize(board.m_size), m_changeRow(board.m_directionRow), m_changeCol(board.m_directionCol), m_ambientLevel(
				board.m_ambientLevel), m_diffuseLevel(board.m_object.m_diffuseLevel), m_specularLevel(board.m_object.m_specularLevel), m_shininessConst(
				board.m_object.m_shininess), m_spotlightNum(board.m_spotlightNum) {
	//add spotlights
	m_spotlightLocs.resize(board.m_spotlightNum);
	for (unsigned int i = 0; i < board.m_spotlightNum; ++i)
		m_spotlightLocs[i] = board.m_spotlightLocs[i];

	m_obj = new Object(board.m_object.m_objFile, board.m_size, board.m_startingLoc);
	m_obj->LoadTexture(board.m_textureDead, ObjType::DEAD);
	m_obj->LoadTexture(board.m_texturesP1[0], ObjType::P1_ALIVE);
	m_obj->LoadTexture(board.m_texturesP2[0], ObjType::P2_ALIVE);
	m_obj->LoadTexture(board.m_texturesP1[1], ObjType::P1_DYING);
	m_obj->LoadTexture(board.m_texturesP2[1], ObjType::P2_DYING);
	m_obj->LoadTexture(board.m_texturesP1[2], ObjType::P1_MARKED);
	m_obj->LoadTexture(board.m_texturesP2[2], ObjType::P2_MARKED);
}

Board::~Board(void) {
	for (std::pair<std::string, Shader *> temp : m_shaders)
		delete temp.second;

	delete m_obj;
}

void Board::AddShaderSet(const std::string & setName, const std::string & vertexShaderSrc, const std::string & fragmentShaderSrc) {
	m_shaders.push_back(std::pair<std::string, Shader *>(setName, new Shader())); //Set up the shader
	m_shaders.back().second->AddShader(GL_VERTEX_SHADER, vertexShaderSrc); //Add the vertex shader
	m_shaders.back().second->AddShader(GL_FRAGMENT_SHADER, fragmentShaderSrc); //Add the fragment shader
	m_shaders.back().second->Finalize(); //Connect the program
}

void Board::UseShaderSet(const std::string & setName) {
	//find shader set
	unsigned int i;
	for (i = 0; i < m_shaders.size(); ++i) {
		if (m_shaders[i].first == setName) {
			m_shaderCurrent = m_shaders[i].second;
			break;
		}
	}
	if (i == m_shaders.size())
		throw std::string("Could not find shader set: " + std::string(setName));

	//helper function to locate uniform and bind it to variable
	auto bindUniform = [this](GLint & uniformBinding, const std::string & uniformName)->void {
		uniformBinding = this->m_shaderCurrent->GetUniformLocation(uniformName);
		if (uniformBinding == -1)
		throw std::string(std::string(uniformName) + " not found");
	};

	//find MVP matricies
	bindUniform(m_projectionMatrix, "projectionMatrix");
	bindUniform(m_viewMatrix, "viewMatrix");
	bindUniform(m_modelMatrix, "modelMatrix");

	//find lighting uniforms
	bindUniform(m_lightPos, "lightPos");
	bindUniform(m_cameraPos, "eyePos");
	bindUniform(m_ambientProduct, "ambientP");
	bindUniform(m_diffuseProduct, "diffuseP");
	bindUniform(m_specularProduct, "specularP");
	bindUniform(m_shininess, "shininess");

	//find instancing uniforms
	bindUniform(m_instanceChangeRow, "changeRow");
	bindUniform(m_instanceChangeCol, "changeCol");
	bindUniform(m_instanceNumPerRow, "numPerRow");
	bindUniform(m_samplerDead, "samplerDead");

	bindUniform(m_samplersP1[0], "samplerP1_Alive");
	bindUniform(m_samplersP2[0], "samplerP2_Alive");
	bindUniform(m_samplersP1[1], "samplerP1_Dying");
	bindUniform(m_samplersP2[1], "samplerP2_Dying");
	bindUniform(m_samplersP1[2], "samplerP1_Marked");
	bindUniform(m_samplersP2[2], "samplerP2_Marked");

	bindUniform(m_sampleTypes, "sampleType");

	m_shaderCurrent->Enable();

	glUniform3f(m_instanceChangeRow, m_changeRow.x, m_changeRow.y, m_changeRow.z);
	glUniform3f(m_instanceChangeCol, m_changeCol.x, m_changeCol.y, m_changeCol.z);
	glUniform1i(m_instanceNumPerRow, (int) m_boardSize.x);

	glUniform1i(m_samplerDead, 0);
	glUniform1i(m_samplersP1[0], 1);
	glUniform1i(m_samplersP2[0], 2);
	glUniform1i(m_samplersP1[1], 3);
	glUniform1i(m_samplersP2[1], 4);
	glUniform1i(m_samplersP1[2], 5);
	glUniform1i(m_samplersP2[2], 6);

	UpdateTypeBindings();
}

void Board::Update(void) {
	m_obj->SetType(ObjType::P1_DYING, 0, 0);
	m_obj->SetType(ObjType::P1_MARKED, 2, 3);
	m_obj->SetType(ObjType::P1_ALIVE, 7, 3);
	m_obj->SetType(ObjType::P2_MARKED, 9, 9);
	m_obj->SetType(ObjType::P2_MARKED, 14, 8);
	UpdateTypeBindings();
	m_obj->Update();
}

void Board::Render(void) {
	if (!m_shaderCurrent) //Ensure shader is enabled
		throw std::string("No shader has been enabled!");

	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_obj->GetModel()));
	m_obj->Render();
}

void Board::ChangeAmbientLight(const glm::vec3 & change) {
	m_ambientLevel += change;
	EnforceBounds (m_ambientLevel);
	UpdateLightBindings();
}

void Board::ChangeDiffuseLight(const glm::vec3 & change) {
	m_diffuseLevel += change;
	EnforceBounds (m_diffuseLevel);
	UpdateLightBindings();
}

void Board::ChangeSpecularLight(const glm::vec3 & change) {
	m_specularLevel += change;
	EnforceBounds (m_specularLevel);
	UpdateLightBindings();
}

//updates bindings for camera in shader
void Board::UpdateCameraBindings(const glm::mat4 & viewMat, const glm::mat4 & projectionMat, const glm::vec3 & cameraPos) {
	if (!m_shaderCurrent) //Ensure shader is enabled
		throw std::string("No shader has been enabled!");

	//Bind shader uniforms
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMat));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniform3f(m_cameraPos, cameraPos.x, cameraPos.y, cameraPos.z);
}

//updates lighting binding in shader
void Board::UpdateLightBindings(void) {
	if (!m_shaderCurrent) //Ensure shader is enabled
		throw std::string("No shader has been enabled!");

	//add lighting varibles/uniforms to shaders if needed
	glUniform3fv(m_lightPos, 3, glm::value_ptr(m_spotlightLocs[0]));

	glUniform3f(m_ambientProduct, m_ambientLevel.x, m_ambientLevel.y, m_ambientLevel.z);
	glUniform3f(m_diffuseProduct, m_diffuseLevel.x, m_diffuseLevel.y, m_diffuseLevel.z);
	glUniform3f(m_specularProduct, m_specularLevel.x, m_specularLevel.y, m_specularLevel.z);
	glUniform1f(m_shininess, m_shininessConst);
}

//updates bindings for type in shader
void Board::UpdateTypeBindings(void) {
	std::vector < ObjType > types = m_obj->GetTypesList();
	glUniform1iv(m_sampleTypes, types.size(), (reinterpret_cast<int*>(&types[0])));
}

//rounds everything to be in range [min, max]
void Board::EnforceBounds(glm::vec3 & v, float min, float max) {
	if (v.x < min)
		v.x = min;
	else if (v.x > max)
		v.x = max;

	if (v.y < min)
		v.y = min;
	else if (v.y > max)
		v.y = max;

	if (v.z < min)
		v.z = min;
	else if (v.z > max)
		v.z = max;
}

