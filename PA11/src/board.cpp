#include "board.h"

Board::Board(const GameInfo & game) :
		m_shaderCurrent(nullptr), m_ambientLevel(game.m_ambientLevel), m_diffuseLevel(game.m_object.m_diffuseLevel), m_specularLevel(
				game.m_object.m_specularLevel), m_shininessConst(game.m_object.m_shininess), m_spotlightLoc(0.0, 0.0, 0.0), m_broadphase(nullptr), m_collisionConfiguration(
				nullptr), m_dispatcher(nullptr), m_solver(nullptr), m_dynamicsWorld(nullptr) {

	for (unsigned int i = 0; i < BoardSides::NUM_SIDES; ++i)
		m_sides[i] = new Object(game.m_object.m_objFile, game.m_sides[i].m_size, game.m_sides[i].m_changeRow, game.m_sides[i].m_changeCol,
				game.m_sides[i].m_startingLoc);

	//load textures
	for (int i = 0; i < ObjType::NUM_TYPES; ++i)
		m_sides[0]->LoadTexture(game.m_textures[i], static_cast<ObjType>(i));
	m_sides[0]->BindTextures();

	InitializeBullet(); //start bullet for raycasting
}

Board::~Board(void) {
	for (std::pair<std::string, Shader *> temp : m_shaders)
		delete temp.second;

	for (unsigned int i = 0; i < BoardSides::NUM_SIDES; ++i)
		delete m_sides[i];

	//clean up bullet world
	delete m_dynamicsWorld;
	delete m_solver;
	delete m_dispatcher;
	delete m_collisionConfiguration;
	delete m_broadphase;
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
	bindUniform(m_projectionMatrix, "projection");
	bindUniform(m_viewMatrix, "view");
	bindUniform(m_modelMatrix, "model");

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
	bindUniform(m_samplers, "samplers");
	bindUniform(m_sampleTypes, "sampleType");

	m_shaderCurrent->Enable();

	const int samplerNums[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	glUniform1iv(m_samplers, 11, samplerNums);
}

void Board::Update(void) {
	for (unsigned int i = 0; i < BoardSides::NUM_SIDES; ++i)
		m_sides[i]->Update();
}

void Board::Render(void) {
	if (!m_shaderCurrent) //Ensure shader is enabled
		throw std::string("No shader has been enabled!");

	for (unsigned int i = 0; i < BoardSides::NUM_SIDES; ++i) {
		UpdateInstanceBindings (m_sides[i]);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sides[i]->GetModel()));
		m_sides[i]->Render();
	}
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

void Board::UpdateSpotlightLoc(const glm::vec3 & location) {
	m_spotlightLoc = location;
	if (!m_shaderCurrent) //Ensure shader is enabled
		throw std::string("No shader has been enabled!");

	//update location
	glUniform3f(m_lightPos, m_spotlightLoc.x, m_spotlightLoc.y, m_spotlightLoc.z);
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
	glUniform3f(m_lightPos, m_spotlightLoc.x, m_spotlightLoc.y, m_spotlightLoc.z);

	glUniform3f(m_ambientProduct, m_ambientLevel.x, m_ambientLevel.y, m_ambientLevel.z);
	glUniform3f(m_diffuseProduct, m_diffuseLevel.x, m_diffuseLevel.y, m_diffuseLevel.z);
	glUniform3f(m_specularProduct, m_specularLevel.x, m_specularLevel.y, m_specularLevel.z);
	glUniform1f(m_shininess, m_shininessConst);
}

void Board::UpdateInstanceBindings(Object * obj) {
	glm::vec3 changeRow = obj->GetChangeRow();
	glm::vec3 changeCol = obj->GetChangeCol();
	const std::vector<ObjType> types = obj->GetTypesList();

	glUniform1iv(m_sampleTypes, types.size(), reinterpret_cast<const int*>(&types[0]));
	glUniform3f(m_instanceChangeRow, changeRow.x, changeRow.y, changeRow.z);
	glUniform3f(m_instanceChangeCol, changeCol.x, changeCol.y, changeCol.z);
	glUniform1i(m_instanceNumPerRow, (int) obj->GetSize().y);
}

const btDiscreteDynamicsWorld * Board::GetBulletWorld(void) const {
	return m_dynamicsWorld;
}

//finds specific element at position. returns {face - BoardSides enumeration, row, column}
glm::uvec3 Board::GetGameElementByPosition(const glm::vec3 & position) const {
#ifdef DEBUG
	printf("\nSearching board for postion: |%s|\n", glm::to_string(position).c_str());
#endif

	for (unsigned int i = 0; i < BoardSides::NUM_SIDES; ++i) {
		glm::vec3 testVector = (position - m_sides[i]->GetTranslation()); //vector between point and main cube and board - should be orthogonal to normal if in plane

#ifdef DEBUG
		printf("%i\n", i);
		printf("Normal of current board: |%s|\n", glm::to_string(m_sides[i]->GetNormal()).c_str());
		printf("Vector to test against normal: |%s|\n", glm::to_string(testVector).c_str());
#endif
		if (glm::length2(testVector) < 0.001 || glm::dot(testVector, m_sides[i]->GetNormal()) == 0) { //p lies on a plane defined by m_side[i]
			try { //search for position
				glm::uvec3 elementPos(i, m_sides[i]->GetCubeByPosition(position));
#ifdef DEBUG
				printf("Element found: |%s|\n", glm::to_string(elementPos).c_str());
#endif
				return elementPos;
			} catch (const std::string &) { //not found in this side. Keep searching cause it could still be in same plane but be part of another, perpendicular side
			}
		}
	}
	throw std::string("Position |" + glm::to_string(position) + "| not found!");
}

//finds position of all the neighbors. element = {face - BoardSides enumeration, row, column}
std::vector<glm::uvec3> Board::GetGameElementNeighbors(const glm::uvec3 & element) const {
#ifdef DEBUG
	printf("\nGetting neighbors of: |%s|\n", glm::to_string(element).c_str());
#endif
	std::vector < glm::uvec3 > neighbors;

	FindElementNeighborsInFace(neighbors, element); //get neighbors in same face

#ifdef DEBUG
	printf("Got neighbors on the same face. Checking if element has neighbors on another face\n");
#endif

	//for getting neighbors in different faces
	//Note: some of this had to be hard coded. So changing the configuration file/board dimensions may cause errors here
	glm::uvec2 faceSize = m_sides[element.x]->GetSize();
	BoardSides tempSide;

	//helper function to add neighbor if possible
	auto addNeighbor = [this, &tempSide, &neighbors](unsigned int r, unsigned int c)->void {
		if (m_sides[tempSide]->IsValidElement(glm::uvec2(r, c))) {
			neighbors.emplace_back(tempSide, r, c);
		}
	};

	if (element.y == 0) { //element is in first row

		switch ((BoardSides) element.x) {

		case BoardSides::FLOOR: //0
			tempSide = BoardSides::NORTH;

			addNeighbor(0, element.z + 1);
			addNeighbor(0, element.z);
			addNeighbor(0, element.z + 2);
			break;

		case BoardSides::ROOF: //1
			tempSide = BoardSides::NORTH;

			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z - 1);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z + 1);
			break;

		case BoardSides::NORTH: //2
			tempSide = BoardSides::FLOOR;

			addNeighbor(0, element.z - 1);
			addNeighbor(0, element.z - 2);
			addNeighbor(0, element.z);
			break;

		case BoardSides::SOUTH: //3
			tempSide = BoardSides::FLOOR;

			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z - 1);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z - 2);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z);
			break;

		case BoardSides::EAST: //4
			tempSide = BoardSides::FLOOR;

			addNeighbor(element.z - 1, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.z - 2, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.z, m_sides[tempSide]->GetSize().y - 1);
			break;

		case BoardSides::WEST: //5
			tempSide = BoardSides::FLOOR;

			addNeighbor(element.z, 0);
			addNeighbor(element.z - 1, 0);
			addNeighbor(element.z + 1, 0);
			break;

		default:
			throw std::string("Invalid element face number:" + glm::to_string(element));
		}

	} else if (element.y == faceSize.x - 1) { //element is in last row

		switch ((BoardSides) element.x) {

		case BoardSides::FLOOR: //0
			tempSide = BoardSides::SOUTH;

			addNeighbor(0, element.z + 1);
			addNeighbor(0, element.z);
			addNeighbor(0, element.z + 2);
			break;

		case BoardSides::ROOF: //1
			tempSide = BoardSides::SOUTH;

			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z - 1);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z + 1);
			break;

		case BoardSides::NORTH: //2
			tempSide = BoardSides::ROOF;

			addNeighbor(0, element.z);
			addNeighbor(0, element.z - 1);
			addNeighbor(0, element.z + 1);
			break;

		case BoardSides::SOUTH: //3
			tempSide = BoardSides::ROOF;

			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z - 1);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.z + 1);
			break;

		case BoardSides::EAST: //4
			tempSide = BoardSides::ROOF;

			addNeighbor(element.z, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.z - 1, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.z + 1, m_sides[tempSide]->GetSize().y - 1);
			break;

		case BoardSides::WEST: //5
			tempSide = BoardSides::ROOF;

			addNeighbor(element.z + 1, 0);
			addNeighbor(element.z, 0);
			addNeighbor(element.z + 2, 0);
			break;

		default:
			throw std::string("Invalid element face number:" + glm::to_string(element));
		}

	}

	if (element.z == 0) { //element is in first column

		switch ((BoardSides) element.x) {

		case BoardSides::FLOOR: //0
			tempSide = BoardSides::WEST;

			addNeighbor(element.z, element.y);
			addNeighbor(element.z, element.y - 1);
			addNeighbor(element.z, element.y + 1);
			break;

		case BoardSides::ROOF: //1
			tempSide = BoardSides::WEST;

			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.y - 1);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.y - 2);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.y);
			break;

		case BoardSides::NORTH: //2
			tempSide = BoardSides::WEST;

			addNeighbor(element.y, 0);
			addNeighbor(element.y - 1, 0);
			addNeighbor(element.y + 1, 0);
			break;

		case BoardSides::SOUTH: //3
			tempSide = BoardSides::WEST;

			addNeighbor(element.y, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.y - 1, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.y + 1, m_sides[tempSide]->GetSize().y - 1);
			break;

		case BoardSides::EAST: //4
			tempSide = BoardSides::NORTH;

			addNeighbor(element.y, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.y - 1, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.y + 1, m_sides[tempSide]->GetSize().y - 1);
			break;

		case BoardSides::WEST: //5
			tempSide = BoardSides::NORTH;

			addNeighbor(element.y, 0);
			addNeighbor(element.y - 1, 0);
			addNeighbor(element.y + 1, 0);
			break;

		default:
			throw std::string("Invalid element face number:" + glm::to_string(element));
		}

	} else if (element.z == faceSize.y - 1) { //element is in last column

		switch ((BoardSides) element.x) {

		case BoardSides::FLOOR: //0
			tempSide = BoardSides::EAST;

			addNeighbor(0, element.y + 1);
			addNeighbor(0, element.y);
			addNeighbor(0, element.y + 2);
			break;

		case BoardSides::ROOF: //1
			tempSide = BoardSides::EAST;

			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.y);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.y - 1);
			addNeighbor(m_sides[tempSide]->GetSize().x - 1, element.y + 1);
			break;

		case BoardSides::NORTH: //2
			tempSide = BoardSides::EAST;

			addNeighbor(element.y, 0);
			addNeighbor(element.y - 1, 0);
			addNeighbor(element.y + 1, 0);
			break;

		case BoardSides::SOUTH: //3
			tempSide = BoardSides::EAST;

			addNeighbor(element.y, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.y - 1, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.y + 1, m_sides[tempSide]->GetSize().y - 1);
			break;

		case BoardSides::EAST: //4
			tempSide = BoardSides::SOUTH;

			addNeighbor(element.y, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.y - 1, m_sides[tempSide]->GetSize().y - 1);
			addNeighbor(element.y + 1, m_sides[tempSide]->GetSize().y - 1);
			break;

		case BoardSides::WEST: //5
			tempSide = BoardSides::SOUTH;

			addNeighbor(element.y, 0);
			addNeighbor(element.y - 1, 0);
			addNeighbor(element.y + 1, 0);
			break;

		default:
			throw std::string("Invalid element face number:" + glm::to_string(element));
		}
	}

	//weird corner cases
	if (element == glm::uvec3(1, 19, 0))
		neighbors.emplace_back(3, 19, 0);
	else if (element == glm::uvec3(1, 1, 0))
		neighbors.emplace_back(2, 19, 0);
	else if (element == glm::uvec3(1, 0, 19))
		neighbors.emplace_back(4, 19, 0);
	else if (element == glm::uvec3(1, 20, 19))
		neighbors.emplace_back(4, 19, 20);
	else if (element == glm::uvec3(2, 19, 0))
		neighbors.emplace_back(1, 1, 0);
	else if (element == glm::uvec3(2, 0, 19))
		neighbors.emplace_back(4, 0, 1);
	else if (element == glm::uvec3(3, 0, 1))
		neighbors.emplace_back(5, 0, 18);
	else if (element == glm::uvec3(3, 19, 0))
		neighbors.emplace_back(1, 19, 0);
	else if (element == glm::uvec3(4, 19, 0))
		neighbors.emplace_back(1, 0, 19);
	else if (element == glm::uvec3(4, 19, 20))
		neighbors.emplace_back(1, 20, 19);
	else if (element == glm::uvec3(5, 0, 18))
		neighbors.emplace_back(3, 0, 18);

	return neighbors;
}

//get current status of game element. element = {face - BoardSides enumeration, row, column}
ObjType Board::GetGameElementType(const glm::uvec3 & element) const {
#ifdef DEBUG
	printf("\nGetting element type for element: |%s|\n", glm::to_string(element).c_str());
#endif
	return m_sides[element.x]->GetType(element.y, element.z);
}

//set currents status of game element. element = {face - BoardSides enumeration, row, column}
void Board::SetGameElementType(const glm::uvec3 & element, const ObjType type) {
#ifdef DEBUG
	printf("\nSetting element |%s| to type: %i\n", glm::to_string(element).c_str(), type);
#endif
	m_sides[element.x]->SetType(element.y, element.z, type);
}

glm::uvec3 Board::GetNextGameElement(const glm::uvec3 & currentElement) const {
	if (m_sides[currentElement.x]->IsValidElement(glm::uvec2(currentElement.y, currentElement.z + 1))) //try next element in row
		return glm::uvec3(currentElement.x, currentElement.y, currentElement.z + 1);
	else if (m_sides[currentElement.x]->IsValidElement(glm::uvec2(currentElement.y + 1, 0))) //try next row
		return glm::uvec3(currentElement.x, currentElement.y + 1, 0);
	else if (currentElement.x + 1 < BoardSides::NUM_SIDES) //try next face
		return glm::uvec3(currentElement.x + 1, 0, 0);
	else
		return glm::uvec3(0, 0, 0); //done go back to first element
}

glm::uvec2 Board::GetScore(void) const {
	glm::uvec3 tempElement(0, 0, 0);
	glm::uvec2 score(0, 0);

	do { //go through all the elements
		ObjType tempElementType = GetGameElementType(tempElement);

		if (tempElementType == P1_ALIVE || tempElementType == P1_DEAD_FUTURE)
			++score.x;
		else if (tempElementType == P2_ALIVE || tempElementType == P2_DEAD_FUTURE)
			++score.y;

		tempElement = GetNextGameElement(tempElement); //go to next element
	} while (tempElement != glm::uvec3(0, 0, 0)); //check if all have been iterated through

	return score;
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

void Board::InitializeBullet(void) {
#ifdef DEBUG
	printf("Initializing Bullet!\n");
#endif

	//initialize bullet world
	m_broadphase = new btDbvtBroadphase();
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_solver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, 0, 0));

	LoadColliders(); //load colliders
}

//for loading each face of game
void Board::LoadColliders(void) {

#ifdef DEBUG
	printf("Setting cube positions...\n");
#endif

	for (unsigned int i = 0; i < BoardSides::NUM_SIDES; ++i) {

#ifdef DEBUG
		printf("Setting side %i...\n", i);
#endif
		std::vector < glm::vec3 > positions = m_sides[i]->GetInstancePositions();

		for (const glm::vec3 & pos : positions)
			AddCubeColliderToWorld(pos, m_sides[i]->GetRotation(), m_sides[i]->GetScale());
	}
}

//for adding cube collider to bullet world
void Board::AddCubeColliderToWorld(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale) {
	btCollisionShape * collider = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
	btQuaternion rotQuat = btQuaternion::getIdentity();
	rotQuat.setEuler(rotation.y, rotation.x, rotation.z);
	btDefaultMotionState * startTransform = new btDefaultMotionState(btTransform(rotQuat, { position.x, position.y, position.z })); //rotation and position
	btScalar mass = 0;
	btVector3 inertia(0, 0, 0);
	collider->calculateLocalInertia(mass, inertia);
	btRigidBody * rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(mass, startTransform, collider, inertia)); //and thus the rigidbody was born
//rigidBody->setGravity({0, 0, 0});
//rigidBody->setActivationState(0);//ensuring it doesn't move no matter what, sleeping objects should still be hit with a raycast
	rigidBody->setSleepingThresholds(0, 0);
	m_dynamicsWorld->addRigidBody(rigidBody); //I might be able to improve this by simply using dynamicsWorld->addCollisionObject(), but i'm a tad skeptical, and this should work fine
	m_rigidBodies.push_back(rigidBody);

#ifdef DEBUG
	printf("Cube Collider placed at location |%s| with scale |%s|\n", glm::to_string(position).c_str(), glm::to_string(scale).c_str());
#endif
}

//helper function to get neighbors in same side as object
void Board::FindElementNeighborsInFace(std::vector<glm::uvec3> & neighbors, const glm::uvec3 & element) const {
	glm::uvec2 faceSize = m_sides[element.x]->GetSize();

	//get neighbors in previous row
	if (element.y > 0) {
		neighbors.emplace_back(element.x, element.y - 1, element.z);
		if (element.z > 0)
			neighbors.emplace_back(element.x, element.y - 1, element.z - 1);
		if (element.z < faceSize.y - 1)
			neighbors.emplace_back(element.x, element.y - 1, element.z + 1);
	}

	//get neighbors in next row
	if (element.y < faceSize.x - 1) {
		neighbors.emplace_back(element.x, element.y + 1, element.z);
		if (element.z > 0)
			neighbors.emplace_back(element.x, element.y + 1, element.z - 1);
		if (element.z < faceSize.y - 1)
			neighbors.emplace_back(element.x, element.y + 1, element.z + 1);
	}

	//get neighbors in previous column
	if (element.z > 0)
		neighbors.emplace_back(element.x, element.y, element.z - 1);

	//get neighbors in next column
	if (element.z < faceSize.y - 1)
		neighbors.emplace_back(element.x, element.y, element.z + 1);
}
