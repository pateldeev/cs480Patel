#include "graphics.h"

Graphics::Graphics(void) :
		m_camera(nullptr), m_currentShader(-1), mbt_broadphase(nullptr), mbt_collisionConfig(nullptr), mbt_dispatcher(nullptr), mbt_solver(nullptr), mbt_dynamicsWorld(
				nullptr), m_lightingStatus(false), m_ambientLevel(0.1, 0.1, 0.1), m_spotlightLoc(0, 0, 0), m_spotLightHeight(6), m_objCtr(-1) {
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

void Graphics::AddObject(const objectModel & obj, bool control) {
	//create appropriate type of object
	if (obj.btType == "SphereDynamic") {
		m_objects.push_back(new Sphere(obj.objFile, obj.startingLoc, obj.rotation, obj.scale));
	} else if (obj.btType == "CubeDynamic") {
		m_objects.push_back(new Cube(obj.objFile, obj.startingLoc, obj.rotation, obj.scale));
	} else if (obj.btType == "MeshStatic") {
		m_objects.push_back(new Board(obj.objFile, obj.startingLoc, obj.rotation, obj.scale));
	} else if (obj.btType == "CylinderStatic") {
		m_objects.push_back(new Cylinder(obj.objFile, obj.startingLoc, obj.rotation, obj.scale));
	} else {
		printf("Unknown bt object type: %s", obj.btType.c_str());
		return;
	}
	m_renderOrder.push_back(m_objects.size() - 1);
	m_startingLocs.push_back(obj.startingLoc);

	m_objectsDiffuseProducts.push_back(glm::vec3(0.8, 0.8, 0.8));
	m_objectsSpecularProducts.push_back(glm::vec3(0.7, 0.7, 0.7));

	m_objects.back()->EnableBt(mbt_dynamicsWorld, obj.mass);

	if (control)
		m_objCtr = m_objects.size() - 1;
}

void Graphics::ResetObjects(void) {
	for (unsigned int i = 0; i < m_objects.size(); ++i) {
		m_objects[i]->ResetBt(m_startingLocs[i]);
		m_objects[i]->SetTranslation(m_startingLocs[i]);
	}
}

void Graphics::ApplyImpulse(const glm::vec3 & impulse, const glm::vec3 & spin) {
	m_objects[m_objCtr]->applyImpulse(impulse, spin);
}

void Graphics::SetLinearVelocity(const glm::vec3 & vel, bool accumulate) {
	m_objects[m_objCtr]->setLinearVelocity(vel, accumulate);
}

void Graphics::SetAngularVelocity(const glm::vec3 & vel, bool accumulate) {
	m_objects[m_objCtr]->setAngularVelocity(vel, accumulate);
}

void Graphics::ApplyForce(const glm::vec3 & force, const glm::vec3 & spin) {
	m_objects[m_objCtr]->applyForce(force, spin);
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
	mbt_dynamicsWorld->stepSimulation(dt / 1000.f, 500);

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

		if (m_lightingStatus) {
			UpdateSpotlightLoc();
			glm::vec3 cameraPos = m_camera->GetEyePos();
			glm::vec3 kD = m_objectsDiffuseProducts[m_renderOrder[i]];
			glm::vec3 kS = m_objectsSpecularProducts[m_renderOrder[i]];

			glUniform3f(m_lightPos, m_spotlightLoc.x, m_spotlightLoc.y, m_spotlightLoc.z);
			glUniform3f(m_cameraPos, cameraPos.x, cameraPos.y, cameraPos.z);
			glUniform3f(m_ambientProduct, m_ambientLevel.x, m_ambientLevel.y, m_ambientLevel.z);
			glUniform3f(m_diffuseProduct, kD.x, kD.y, kD.z);
			glUniform3f(m_specularProduct, kS.x, kS.y, kS.z);
			glUniform1f(m_shininess, 32);
		}

		m_objects[m_renderOrder[i]]->Render();
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
	glm::vec3 temp = m_objectsDiffuseProducts[m_objCtr] + change;

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

	m_objectsDiffuseProducts[m_objCtr] = temp;
}

void Graphics::SetSpecularofBall(const glm::vec3 & change) {
	glm::vec3 temp = m_objectsSpecularProducts[m_objCtr] + change;

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
	m_objectsSpecularProducts[m_objCtr] = temp;
}

void Graphics::SetSpotlightHeight(float change) {
	m_spotLightHeight += change;
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

void Graphics::UpdateSpotlightLoc(void) {
	m_spotlightLoc = m_objects[m_objCtr]->GetTranslation();
	m_spotlightLoc.y += m_spotLightHeight;
}

void Graphics::IncreaseEyePosX(float moveAmount) {
	glm::vec3 newEyePos;
	newEyePos = GetEyePos();
	newEyePos.x = newEyePos.x + moveAmount;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());

}

void Graphics::DecreaseEyePosX(float moveAmount) {
	glm::vec3 newEyePos;
	newEyePos = GetEyePos();
	newEyePos.x = newEyePos.x - moveAmount;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());
}

void Graphics::IncreaseEyePosZ(float moveAmount) {
	glm::vec3 newEyePos;
	newEyePos = GetEyePos();
	newEyePos.z = newEyePos.z - moveAmount;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());
}

void Graphics::DecreaseEyePosZ(float moveAmount) {
	glm::vec3 newEyePos;
	newEyePos = GetEyePos();
	newEyePos.z = newEyePos.z + moveAmount;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());
}

void Graphics::ZoomIn(float moveAmount) {
	glm::vec3 newEyePos;
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - GetEyePos());
	newEyePos = GetEyePos() + moveVector;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());
}

void Graphics::ZoomOut(float moveAmount) {
	glm::vec3 newEyePos;
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - GetEyePos());
	newEyePos = GetEyePos() - moveVector;
	m_camera->UpdatePosition(newEyePos, GetEyeLoc());
}
