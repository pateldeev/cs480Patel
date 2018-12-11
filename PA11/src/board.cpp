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

void Board::Update() {
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
	//implimented exhaustive search for now
	//will need to use normals in future to narrow down search to correct face
	for (unsigned int i = 0; i < BoardSides::NUM_SIDES; ++i) {
<<<<<<< HEAD
		if (m_sides[i]) {
                    glm::vec3 testVector(position + m_sides[i]->GetChangeRow());
                    //glm::vec3 tempNormal(position + m_sides[i]->GetChangeRow());
                    //tempNormal = glm::cross(m_sides[i]->GetChangeRow(), tempNormal);
                    //tempNormal = glm::normalize(tempNormal);
#ifdef DEBUG
                    printf("Normal of current board: {%f, %f, %f}\n", m_sides[i]->GetNormal().x, m_sides[i]->GetNormal().y, m_sides[i]->GetNormal().z);
                    printf("Vector to test against normal: {%f, %f, %f}\n", testVector.x, testVector.y, testVector.z);
                    //printf("Normal generated from point: {%f, %f, %f}\n", tempNormal.x, tempNormal.y, tempNormal.z);
#endif
                    if (glm::dot(testVector, m_sides[i]->GetNormal()) == 0) {//p lies on the plane defined by m_side[i]
                    //if (tempNormal == m_sides[i]->GetNormal()) {
			try {
				glm::uvec2 elementPos = m_sides[i]->GetCubeByPosition(position);
				return glm::uvec3(i, elementPos);
			} catch (const std::string &) { //not this one - go to next one
			}
                    }
=======
		try {
			glm::uvec2 elementPos = m_sides[i]->GetCubeByPosition(position);
			return glm::uvec3(i, elementPos);
		} catch (const std::string &) { //not this one - go to next one
>>>>>>> e5a60e5c7bb62d9f1cb210231a5065d1cc25932c
		}
	}

	//did not find game element. Throw exception
	throw std::string("Position " + std::to_string(position.x) + "," + std::to_string(position.y) + "," + std::to_string(position.z) + " not found!");
}

//get current status of game element. element = {face - BoardSides enumeration, row, column}
ObjType Board::GetGameElementType(const glm::uvec3 & element) const {
	return m_sides[element.x]->GetType(element.y, element.z);
}

//set currents status of game element. element = {face - BoardSides enumeration, row, column}
void Board::SetGameElementType(const glm::uvec3 & element, const ObjType type) {
	m_sides[element.x]->SetType(element.y, element.z, type);
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
	printf("Cube Collider placed at location {%f, %f, %f} with scale {%f, %f, %f}\n",
			position.x, position.y, position.z,
			scale.x, scale.y, scale.z);
#endif
}

