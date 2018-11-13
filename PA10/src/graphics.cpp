#include "graphics.h"

Graphics::Graphics(void) :
		m_camera(nullptr), m_currentShader(-1), mbt_broadphase(nullptr), mbt_collisionConfig(nullptr), mbt_dispatcher(nullptr), mbt_solver(nullptr), mbt_dynamicsWorld(
				nullptr), m_lightingStatus(false), m_ambientLevel(0.0, 0.0, 0.0), m_shininessConst(0), m_spotLightHeight(6), m_ball(-1), m_paddleR(
				-1), m_paddleL(-1) {
	m_spotlightLocs.resize(1);
}

Graphics::~Graphics(void) {
	//remove the rigidbodies from the dynamics world and delete them
	for (int i = mbt_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; --i) {
		btCollisionObject * obj = mbt_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody * body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
			delete body->getMotionState();
		mbt_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	for (Object * obj : m_objects)
		delete obj;

	delete mbt_dynamicsWorld;
	delete mbt_solver;
	delete mbt_dispatcher;
	delete mbt_collisionConfig;
	delete mbt_broadphase;

	for (Shader * shader : m_shaders)
		delete shader;

	delete m_camera;
}

bool Graphics::Initialize(unsigned int windowWidth, unsigned int windowHeight, const glm::vec3 & eyePos, const glm::vec3 & focusPos) {
// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
	// std::cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK) {
		printf("GLEW Error: %s \n", glewGetErrorString(status));
		return false;
	}
#endif

	// For OpenGL 3
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Init Camera
	m_camera = new Camera(eyePos, focusPos);
	if (!m_camera->Initialize(windowWidth, windowHeight)) {
		printf("Camera Failed to Initialize\n");
		return false;
	}

	//enable depth testing
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);

	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

bool Graphics::InitializeBt(const glm::vec3 & gravity) {
	//create bullet world
	mbt_broadphase = new btDbvtBroadphase();
	mbt_collisionConfig = new btDefaultCollisionConfiguration();
	mbt_dispatcher = new btCollisionDispatcher(mbt_collisionConfig);
	mbt_solver = new btSequentialImpulseConstraintSolver();

	mbt_dynamicsWorld = new btDiscreteDynamicsWorld(mbt_dispatcher, mbt_broadphase, mbt_solver, mbt_collisionConfig);
	mbt_dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));

	return true;
}

bool Graphics::InitializeLighting(const glm::vec3 & ambientLevel, const float shininess) {
	m_ambientLevel = ambientLevel;
	m_shininessConst = shininess;

	return true;
}

void Graphics::AddObject(const objectModel & obj) {
	//create appropriate type of object
	if (obj.btType == "SphereDynamic") {
		m_objects.push_back(new Sphere(obj.objFile, obj.startingLoc, obj.rotation, obj.scale));
	} else if (obj.btType == "CubeDynamic") {
		m_objects.push_back(new Cube(obj.objFile, obj.startingLoc, obj.rotation, obj.scale));
	} else if (obj.btType == "Board") {
		m_objects.push_back(new Board(obj.objFile, obj.startingLoc, obj.rotation, obj.scale));
	} else if (obj.btType == "CylinderStatic") {
		m_objects.push_back(new Cylinder(obj.objFile, obj.startingLoc, obj.rotation, obj.scale));
	} else if (obj.btType == "Paddle") {
		m_objects.push_back(new Paddle(obj.objFile, obj.startingLoc, obj.rotation, obj.scale));
	} else if (obj.btType == "Complex") {
		m_objects.push_back(new Complex(obj.objFile, obj.startingLoc, obj.rotation, obj.scale));
	} else {
		printf("Unknown bt object type: %s", obj.btType.c_str());
		return;
	}

	//store object properties
	m_renderOrder.push_back(m_objects.size() - 1);
	m_startingLocs.push_back(obj.startingLoc);
	m_startingRotations.push_back(obj.rotation);
	m_objectsDiffuseProducts.push_back(obj.diffuseProduct);
	m_objectsSpecularProducts.push_back(obj.specularProduct);

	//enable bullet on object
	m_objects.back()->EnableBt(mbt_dynamicsWorld, obj.mass);

	if (obj.name == "Ball") {
		m_ball = m_objects.size() - 1;
		m_spotlightLocs[0] = obj.startingLoc;
	} else if (obj.name == "Paddle_Right") {
		m_paddleR = m_objects.size() - 1;
	} else if (obj.name == "Paddle_Left") {
		m_paddleL = m_objects.size() - 1;
	}
}

bool Graphics::VerifyObjects(void) const {
	//check if ball is present
	if (m_ball < 0) {
		printf("No Ball object found!");
		return false;
	}
	//check if paddles are present
	if (m_paddleR < 0) {
		printf("No Paddle_Right object found!");
		return false;
	}
	if (m_paddleL < 0) {
		printf("No Paddle_Left object found!");
		return false;
	}

	return true;
}

void Graphics::ResetBall(void) {
	m_objects[m_ball]->ResetBt(m_startingLocs[m_ball], m_startingRotations[m_ball]);
}

void Graphics::ApplyImpulse(const glm::vec3 & impulse, const glm::vec3 & spin) {
	m_objects[m_ball]->applyImpulse(impulse, spin);
}

void Graphics::ApplyForce(const glm::vec3 & force, const glm::vec3 & spin) {
	m_objects[m_ball]->applyForce(force, spin);
}

void Graphics::SetLinearVelocity(const glm::vec3 & vel, bool accumulate) {
	m_objects[m_ball]->setLinearVelocity(vel, accumulate);
}

void Graphics::SetAngularVelocity(const glm::vec3 & vel, bool accumulate) {
	m_objects[m_ball]->setAngularVelocity(vel, accumulate);
}

void Graphics::MovePaddleR(void) {
	static_cast<Paddle *>(m_objects[m_paddleR])->MoveUpR();
}

void Graphics::SetResetFlagPaddleR(bool flag) {
	static_cast<Paddle *>(m_objects[m_paddleR])->SetResetFlag(flag);
}

bool Graphics::AddShaderSet(const std::string & setName, const std::string & vertexShaderSrc, const std::string & fragmentShaderSrc) {
	//Set up the shader
	m_shaders.push_back(new Shader());
	if (!m_shaders.back()->Initialize()) {
		printf("Shader set %s Failed to Initialize\n", setName.c_str());
		m_shaders.pop_back();
		return false;
	}

	// Add the vertex shader
	if (!m_shaders.back()->AddShader(GL_VERTEX_SHADER, vertexShaderSrc)) {
		printf("Vertex Shader failed to Initialize\n");
		m_shaders.pop_back();
		return false;
	}

	// Add the fragment shader
	if (!m_shaders.back()->AddShader(GL_FRAGMENT_SHADER, fragmentShaderSrc)) {
		printf("Fragment Shader failed to Initialize\n");
		m_shaders.pop_back();
		return false;
	}

	// Connect the program
	if (!m_shaders.back()->Finalize()) {
		printf("Program failed to Finalize\n");
		m_shaders.pop_back();
		return false;
	}

	m_shaderNames.push_back(setName);
	return true;
}

bool Graphics::UseShaderSet(const std::string & setName, bool hasLighting) {
	//find shader set
	unsigned int i;
	for (i = 0; i < m_shaderNames.size(); ++i)
		if (m_shaderNames[i] == setName)
			break;

	if (i == m_shaderNames.size()) {
		printf("Could not find shader set %s", setName.c_str());
		return false;
	}

	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shaders[i]->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == -1) {
		printf("m_projectionMatrix not found\n");
		return false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shaders[i]->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == -1) {
		printf("m_viewMatrix not found\n");
		return false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shaders[i]->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == -1) {
		printf("m_modelMatrix not found\n");
		return false;
	}

	//find lighting uniforms
	if (hasLighting) {
		m_lightingStatus = true;
		m_lightPos = m_shaders[i]->GetUniformLocation("lightPos");
		if (m_lightPos == -1) {
			printf("lightPos not found\n");
			return false;
		}

		m_cameraPos = m_shaders[i]->GetUniformLocation("eyePos");
		if (m_cameraPos == -1) {
			printf("eyePos not found\n");
			return false;
		}

		m_ambientProduct = m_shaders[i]->GetUniformLocation("ambientP");
		if (m_ambientProduct == -1) {
			printf("ambient product not found\n");
			return false;
		}

		m_diffuseProduct = m_shaders[i]->GetUniformLocation("diffuseP");
		if (m_diffuseProduct == -1) {
			printf("diffuse product not found\n");
			return false;
		}

		m_specularProduct = m_shaders[i]->GetUniformLocation("specularP");
		if (m_specularProduct == -1) {
			printf("specular product not found\n");
			return false;
		}

		m_shininess = m_shaders[i]->GetUniformLocation("shininess");
		if (m_shininess == -1) {
			printf("shininess not found\n");
			return false;
		}
	}

	//update current shader
	m_currentShader = i;
	return true;
}

void Graphics::Update(unsigned int dt) {
	mbt_dynamicsWorld->stepSimulation(dt / 1000.f, 1000, btScalar(1.) / btScalar(100.));

	if (static_cast<Paddle *>(m_objects[m_paddleR])->GetResetFlag())
		static_cast<Paddle *>(m_objects[m_paddleR])->ResetPaddleR();

	int numManifolds = mbt_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; ++i) {
		btPersistentManifold * contactManifold = mbt_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject * obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject * obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() < 0.f) {
				if ((m_objects[m_ball]->GetRigidBody() == obA && m_objects[m_paddleR]->GetRigidBody() == obB)
						|| (m_objects[m_ball]->GetRigidBody() == obB && m_objects[m_paddleR]->GetRigidBody() == obA)) {
					ApplyImpulse(glm::vec3(0, 0, -30), glm::vec3(0, 0, 0));
				}

				if ((m_objects[m_ball]->GetRigidBody() == obA && m_objects[m_paddleL]->GetRigidBody() == obB)
						|| (m_objects[m_ball]->GetRigidBody() == obB && m_objects[m_paddleL]->GetRigidBody() == obA)) {
					ApplyImpulse(glm::vec3(0, 0, -30), glm::vec3(0, 0, 0));
				}
			}
		}
	}

	//update location and rotation of each object from bullet to openGL
	for (Object * obj : m_objects) {
		if (obj->GetRigidBody()) {
			btTransform trans;
			if (obj->GetRigidBody()->getMotionState())
				obj->GetRigidBody()->getMotionState()->getWorldTransform(trans);
			else
				printf("Error - body has not motionstate!\n");

			obj->SetTranslation(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
			glm::vec3 newRotations;
			trans.getRotation().getEulerZYX(newRotations.z, newRotations.y, newRotations.x);
			obj->SetRotationAngles(newRotations);
		}
		obj->Update();
	}
}

bool Graphics::UpdateCamera(const glm::vec3 & eyePos, const glm::vec3 & eyeFocus) {
	m_camera->UpdatePosition(eyePos, eyeFocus);
	return true;
}

void Graphics::Render(void) {
//Clear the screen
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//Start the correct program
	if (m_currentShader < 0)
		printf("No shader has been enabled!\n");
	m_shaders.at(m_currentShader)->Enable();

//Send in the projection and view to the shader
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

//update render order based on distance
	glm::vec3 cameraPos = m_camera->GetEyePos();
	std::sort(std::begin(m_renderOrder), std::end(m_renderOrder), [this, &cameraPos](unsigned int a, unsigned int b) {
		return m_objects[a]->GetDistanceFromPoint(cameraPos) > m_objects[b]->GetDistanceFromPoint(cameraPos);});

//Render each object
	for (unsigned int i = 0; i < m_renderOrder.size(); ++i) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_objects[m_renderOrder[i]]->GetModel()));

		//add lighting varibles/uniforms to shaders if needed
		if (m_lightingStatus) {
			UpdateBallSpotlight();
			glm::vec3 cameraPos = m_camera->GetEyePos();
			glm::vec3 kD = m_objectsDiffuseProducts[m_renderOrder[i]];
			glm::vec3 kS = m_objectsSpecularProducts[m_renderOrder[i]];

			glm::vec3 pointLightPositions[] = { m_spotlightLocs.at(0), m_spotlightLocs.at(1), m_spotlightLocs.at(2) };

			glUniform3fv(m_lightPos, 3, glm::value_ptr(pointLightPositions[0]));
			glUniform3f(m_cameraPos, cameraPos.x, cameraPos.y, cameraPos.z);
			glUniform3f(m_ambientProduct, m_ambientLevel.x, m_ambientLevel.y, m_ambientLevel.z);
			glUniform3f(m_diffuseProduct, kD.x, kD.y, kD.z);
			glUniform3f(m_specularProduct, kS.x, kS.y, kS.z);
			glUniform1f(m_shininess, m_shininessConst);
		}

		m_objects[m_renderOrder[i]]->Render(); //render object
	}

//Get any errors from OpenGL
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::string val = ErrorString(error);
		printf("Error initializing OpenGL! %d: %s \n", error, val.c_str());
	}
}

glm::vec3 Graphics::GetEyePos(void) const {
	return m_camera->GetEyePos();
}

glm::vec3 Graphics::GetEyeLoc(void) const {
	return m_camera->GetFocusPos();
}

void Graphics::SetAmbientLight(const glm::vec3 & change) {
	m_ambientLevel += change;

//check bounds
	if (m_ambientLevel.x < 0)
		m_ambientLevel.x = 0;
	else if (m_ambientLevel.x > 1)
		m_ambientLevel.x = 1;

	if (m_ambientLevel.y < 0)
		m_ambientLevel.y = 0;
	else if (m_ambientLevel.y > 1)
		m_ambientLevel.y = 1;

	if (m_ambientLevel.z < 0)
		m_ambientLevel.z = 0;
	else if (m_ambientLevel.z > 1)
		m_ambientLevel.z = 1;
}

void Graphics::SetDiffuseofBall(const glm::vec3 & change) {
	glm::vec3 temp = m_objectsDiffuseProducts[m_ball] + change;

//check bounds
	if (temp.x < 0)
		temp.x = 0;
	else if (temp.x > 1)
		temp.x = 1;

	if (temp.y < 0)
		temp.y = 0;
	else if (temp.y > 1)
		temp.y = 1;

	if (temp.z < 0)
		temp.z = 0;
	else if (temp.z > 1)
		temp.z = 1;

	m_objectsDiffuseProducts[m_ball] = temp;
}

void Graphics::SetSpecularofBall(const glm::vec3 & change) {
	glm::vec3 temp = m_objectsSpecularProducts[m_ball] + change;

//check bounds
	if (temp.x < 0)
		temp.x = 0;
	else if (temp.x > 1)
		temp.x = 1;

	if (temp.y < 0)
		temp.y = 0;
	else if (temp.y > 1)
		temp.y = 1;

	if (temp.z < 0)
		temp.z = 0;
	else if (temp.z > 1)
		temp.z = 1;
	m_objectsSpecularProducts[m_ball] = temp;
}

void Graphics::SetSpotlightHeight(float change) {
	m_spotLightHeight += change;
}

void Graphics::AddSpotLight(const glm::vec3 & location) {
	m_spotlightLocs.push_back(location);
	m_spotlightLocs.back().y += m_spotLightHeight;
}

std::string Graphics::ErrorString(const GLenum error) const {
	if (error == GL_INVALID_ENUM)
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	else if (error == GL_INVALID_VALUE)
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	else if (error == GL_INVALID_OPERATION)
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	else if (error == GL_OUT_OF_MEMORY)
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	else
		return "None";
}

void Graphics::UpdateBallSpotlight(void) {
	m_spotlightLocs[0] = m_objects[m_ball]->GetTranslation();
	m_spotlightLocs[0].y += m_spotLightHeight;
}

void Graphics::IncreaseEyePosX(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.x += moveAmount;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());

}

void Graphics::DecreaseEyePosX(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.x -= moveAmount;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());
}

void Graphics::IncreaseEyePosZ(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.z -= moveAmount;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());
}

void Graphics::DecreaseEyePosZ(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.z += moveAmount;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());
}

void Graphics::ZoomIn(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - newEyePos);
	newEyePos += moveVector;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());
}

void Graphics::ZoomOut(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - newEyePos);
	newEyePos -= moveVector;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());
}
