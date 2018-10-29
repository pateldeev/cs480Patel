#include "graphics.h"

Graphics::Graphics(void) :
		m_camera(nullptr), m_currentShader(-1), mbt_broadphase(nullptr), mbt_collisionConfig(nullptr), mbt_dispatcher(nullptr), mbt_solver(nullptr), mbt_dynamicsWorld(
				nullptr) {
}

Graphics::~Graphics(void) {
	m_objects.clear();

	delete m_camera;

	delete mbt_broadphase;
	delete mbt_collisionConfig;
	delete mbt_dispatcher;
	delete mbt_solver;
	delete mbt_dynamicsWorld;
}

bool Graphics::Initialize(unsigned int windowWidth, unsigned int windowHeight, const glm::vec3 & eyePos, const glm::vec3 & focusPos) {
    printf("Initializing Graphics");
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

	//create bullet world
	mbt_broadphase = new btDbvtBroadphase();
	mbt_collisionConfig = new btDefaultCollisionConfiguration();
	mbt_dispatcher = new btCollisionDispatcher(mbt_collisionConfig);
	mbt_solver = new btSequentialImpulseConstraintSolver;

	mbt_dynamicsWorld = new btDiscreteDynamicsWorld(mbt_dispatcher, mbt_broadphase, mbt_solver, mbt_collisionConfig);
	mbt_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	return true;
}

void Graphics::AddObject(const objectModel & obj) {
    printf("Adding Object to Graphics");
    Object* newObject;
    if (obj.name == "Sphere") {
        *newObject = Sphere(obj.objFile);
    } else if (obj.name == "Board") {
        *newObject = Board(obj.objFile);
    } else if (obj.name == "Cube") {
        *newObject = Cube(obj.objFile);
    }
	m_objects.push_back(newObject);
	
	//set default properties
        m_objects.back()->SetName(obj.name);
	m_objects.back()->SetCurrentLocation(obj.startingLoc);
	m_objects.back()->SetScale(obj.scale);
	m_objects.back()->SetRotationAngles(obj.rotation);
}

bool Graphics::AddShaderSet(const std::string & setName, const std::string & vertexShaderSrc, const std::string & fragmentShaderSrc) {
	//Set up the shader
	m_shaders.push_back(Shader());
	if (!m_shaders.back().Initialize()) {
		printf("Shader set %s Failed to Initialize\n", setName.c_str());
		m_shaders.pop_back();
		return false;
	}

	// Add the vertex shader
	if (!m_shaders.back().AddShader(GL_VERTEX_SHADER, vertexShaderSrc)) {
		printf("Vertex Shader failed to Initialize\n");
		m_shaders.pop_back();
		return false;
	}

	// Add the fragment shader
	if (!m_shaders.back().AddShader(GL_FRAGMENT_SHADER, fragmentShaderSrc)) {
		printf("Fragment Shader failed to Initialize\n");
		m_shaders.pop_back();
		return false;
	}

	// Connect the program
	if (!m_shaders.back().Finalize()) {
		printf("Program to Finalize\n");
		m_shaders.pop_back();
		return false;
	}

	m_shaderNames.push_back(setName);
	return true;
}

bool Graphics::UseShaderSet(const std::string & setName) {
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
	m_projectionMatrix = m_shaders[i].GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) {
		printf("m_projectionMatrix not found\n");
		return false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shaders[i].GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION) {
		printf("m_viewMatrix not found\n");
		return false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shaders[i].GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION) {
		printf("m_modelMatrix not found\n");
		return false;
	}

	//update current shader
	m_currentShader = i;
	return true;

}

void Graphics::Update(unsigned int dt) {
	for (Object * obj : m_objects)
		obj->Update(dt);
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
	m_shaders.at(m_currentShader).Enable();

	//Send in the projection and view to the shader
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	//sort objects so that furthest objects render first
        glm::vec3 cameraPosition = m_camera->GetEyePos();
        printf("Sorting m_objects vector");
        sort(m_objects.begin(), m_objects.end(), [&cameraPosition](Object* a, Object* b) {
                return a->GetDistanceFromPoint(cameraPosition) > b->GetDistanceFromPoint(cameraPosition);
        });
        //Render each planet and its moons
	for (Object* obj : m_objects) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(obj->GetModel()));
		obj->Render();
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
