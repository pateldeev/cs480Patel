#include "graphics.h"

Graphics::Graphics(unsigned int windowWidth, unsigned int windowHeight, const glm::vec3 & eyePos, const glm::vec3 & eyeFocus, const boardInfo & board) :
		m_camera(windowWidth, windowHeight, eyePos, eyeFocus), m_board(nullptr) {

// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
	glewExperimental = GL_TRUE;

	const GLenum status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK) {
		std::string errorMsg = "Graphics could not be created! GLEW Error: ";
		errorMsg += ((char*) glewGetErrorString(status));
		throw errorMsg;
	}
#endif

	// For OpenGL 3
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//enable depth testing
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);

	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_board = new Board(board);
        
        if (!InitializeBullet() || m_dynamicsWorld == nullptr) {
           printf("Bullet Discrete Dynamics World failed to initialize!"); 
        }
        GetCubePositions(board);
}

Graphics::~Graphics(void) {
	delete m_board;
        delete m_dynamicsWorld;
}

void Graphics::AddShaderSet(const std::string & setName, const std::string & vertexShaderSrc, const std::string & fragmentShaderSrc) {
	m_board->AddShaderSet(setName, vertexShaderSrc, fragmentShaderSrc);
}

void Graphics::UseShaderSet(const std::string & setName) {
	m_board->UseShaderSet(setName);
	m_board->UpdateLightBindings();
	UpdateCameraBindings();
}

void Graphics::Update(unsigned int dt) {
	m_board->Update();
}

void Graphics::Render(void) {
	//Clear the screen
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_board->Render();

	//Get any errors from OpenGL
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::string errMsg = std::string("Error initializing OpenGL! ") + std::to_string(error) + std::string(" : ") + ErrorString(error);
		throw errMsg;
	}
}

void Graphics::IncreaseEyePosX(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.x += moveAmount;
	UpdateCamera(newEyePos, GetEyeLoc());

}

void Graphics::DecreaseEyePosX(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.x -= moveAmount;
	UpdateCamera(newEyePos, GetEyeLoc());
}

void Graphics::IncreaseEyePosZ(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.z -= moveAmount;
	UpdateCamera(newEyePos, GetEyeLoc());
}

void Graphics::DecreaseEyePosZ(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	newEyePos.z += moveAmount;
	UpdateCamera(newEyePos, GetEyeLoc());
}

void Graphics::ZoomIn(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - newEyePos);
	newEyePos += moveVector;
	UpdateCamera(newEyePos, GetEyeLoc());
}

void Graphics::ZoomOut(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - newEyePos);
	newEyePos -= moveVector;
	UpdateCamera(newEyePos, GetEyeLoc());
}

void Graphics::MoveForward(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	glm::vec3 newEyeLoc = GetEyeLoc();
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - newEyePos);
	newEyePos += moveVector * moveAmount;
	newEyeLoc += moveVector * moveAmount;
	UpdateCamera(newEyePos, newEyeLoc);
}

void Graphics::MoveBackward(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	glm::vec3 newEyeLoc = GetEyeLoc();
	glm::vec3 moveVector = glm::normalize(GetEyeLoc() - newEyePos);
	newEyePos -= moveVector * moveAmount;
	newEyeLoc -= moveVector * moveAmount;
	UpdateCamera(newEyePos, newEyeLoc);
}

void Graphics::MoveRight(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	glm::vec3 newEyeLoc = GetEyeLoc();
	glm::vec3 pointVector = newEyeLoc - newEyePos;
	glm::vec3 upVector = glm::vec3(0, 1, 0);
	glm::vec3 moveVector = glm::normalize(glm::cross(pointVector, upVector));
	newEyePos += moveVector * moveAmount;
	newEyeLoc += moveVector * moveAmount;
	UpdateCamera(newEyePos, newEyeLoc);
}

void Graphics::MoveLeft(float moveAmount) {
	glm::vec3 newEyePos = GetEyePos();
	glm::vec3 newEyeLoc = GetEyeLoc();
	glm::vec3 pointVector = newEyeLoc - newEyePos;
	glm::vec3 upVector = glm::vec3(0, 1, 0);
	glm::vec3 moveVector = glm::normalize(glm::cross(pointVector, upVector));
	newEyePos -= moveVector * moveAmount;
	newEyeLoc -= moveVector * moveAmount;
	UpdateCamera(newEyePos, newEyeLoc);
}

void Graphics::UpdateCamera(const glm::vec3 & eyePos, const glm::vec3 & eyeFocus) {
	m_camera.UpdateCamera(eyePos, eyeFocus);
	UpdateCameraBindings();
}

glm::vec3 Graphics::GetEyePos(void) const {
	return m_camera.GetEyePos();
}

glm::vec3 Graphics::GetEyeLoc(void) const {
	return m_camera.GetEyeFocus();
}

void Graphics::SetScreenWidth(float width) {
    m_screenWidth = width;
}

void Graphics::SetScreenHeight(float height) {
    m_screenHeight = height;
}

void Graphics::ChangeAmbientLight(const glm::vec3 & change) {
	m_board->ChangeAmbientLight(change);
}

void Graphics::ChangeDiffuseLight(const glm::vec3 & change) {
	m_board->ChangeDiffuseLight(change);
}

void Graphics::ChangeSpecularLight(const glm::vec3 & change) {
	m_board->ChangeSpecularLight(change);
}

void Graphics::LeftClick(glm::vec2 mousePosition) {
    //cube position will be {-1, -1, -1} if no object was hit, otherwise it's {board number, row, column}
    glm::vec3 cubePosition = GetCubeUnderPosition(mousePosition);
    
    if (cubePosition != glm::vec3(-1, -1, -1)) {
        //pick the right board
        //then for that board do this
        //(this however is just a proof of concept to show it works)
        ObjType type = m_board->m_obj->GetType(cubePosition.y, cubePosition.z);
        type = static_cast<ObjType>((static_cast<int>(type) + 2) % static_cast<int>(ObjType::NUM_OBJ_TYPES));
        m_board->m_obj->SetType(cubePosition.y, cubePosition.z, type);//every other click should cycle the texture on the cube
    }
#ifdef DEBUG
    else {
        printf("There was no cube located under mouse position {%f, %f}\n", mousePosition.x, mousePosition.y);
    }
#endif
}

//creates physics world to handle raycasting for clicking
bool Graphics::InitializeBullet() {
#ifdef DEBUG 
    printf("Initializing Bullet!\n");
#endif
    m_broadphase = new btDbvtBroadphase();
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver();
    
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, 0, 0));
    return true;
}

void Graphics::AddCubeColliderToWorld(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    scale = scale / 2.0f;
    btCollisionShape *collider = new btBoxShape(btVector3(scale.x, scale.y, scale.z));//should be half extents
    btQuaternion rotQuat = btQuaternion::getIdentity();
    rotQuat.setEuler(rotation.y, rotation.x, rotation.z);
    btDefaultMotionState *startTransform = new btDefaultMotionState(btTransform(rotQuat, {position.x, position.y, position.z}));//rotation and position
    btScalar mass = 0;
    btVector3 inertia(0, 0, 0);
    collider->calculateLocalInertia(mass, inertia);
    btRigidBody *rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(
            mass,
            startTransform,
            collider,
            inertia));//and thus the rigidbody was born
    //rigidBody->setGravity({0, 0, 0});
    //rigidBody->setActivationState(0);//ensuring it doesn't move no matter what, sleeping objects should still be hit with a raycast
    rigidBody->setSleepingThresholds(0, 0);
    m_rigidBodies.push_back(rigidBody);
    m_dynamicsWorld->addRigidBody(rigidBody);//I might be able to improve this by simply using dynamicsWorld->addCollisionObject(), but i'm a tad skeptical, and this should work fine
#ifdef DEBUG
    printf("Collider added!\n");
#endif
}

void Graphics::GetCubePositions(boardInfo board) {
#ifdef DEBUG
    printf("Setting cube positions...\n");
#endif
    glm::vec3 scale = board.m_object.m_scale;
    //for each wall do this
    //get wllID starting location
    glm::vec3 start = board.m_startingLoc;
    glm::vec3 rotation = board.m_object.m_rotation;
    glm::vec3 colTranslation = {0, 0, 0};
    glm::vec3 rowTranslation = {0, 0, 0};
    for (int i = 0; i < (int) board.m_size.x; i++) {//row
        for (int j = 0; j < (int) board.m_size.y; j++) {//col
            rowTranslation = (float) i * board.m_directionRow;
            colTranslation = (float) j * board.m_directionCol;
            glm::vec3 newPos = start + rowTranslation + colTranslation;
#ifdef DEBUG
            printf("Placing Cube Collider at location {%f, %f, %f} with scale {%f, %f, %f}\n",
                    newPos.x, newPos.y, newPos.z,
                    scale.x, scale.y, scale.z);
#endif
            AddCubeColliderToWorld(start + rowTranslation + colTranslation, rotation, scale);
        }
    }
}

glm::vec3 Graphics::GetCubeUnderPosition(glm::vec2 mousePosition) {//set up from and to for raycast direction
    float mouseScreenX = (2.0f * mousePosition.x) / m_screenWidth - 1.0f;
    float mouseScreenY = 1.0f - (2.0f * mousePosition.y) / m_screenHeight;
    glm::vec4 mouseRayStart(mouseScreenX, mouseScreenY, -1.0f, 1.0f);
    glm::vec4 mouseRayEnd(mouseScreenX, mouseScreenY, 0, 1.0f);

    glm::mat4 inverseProjectionMatrix = glm::inverse(m_camera.GetProjection());
    glm::mat4 inverseViewMatrix = glm::inverse(m_camera.GetView());
    
    glm::vec4 cameraRayStart = inverseProjectionMatrix * mouseRayStart;
    cameraRayStart /= cameraRayStart.w;
    glm::vec4 cameraRayEnd = inverseProjectionMatrix * mouseRayEnd;
    cameraRayEnd /= cameraRayEnd.w;
    
    glm::vec4 worldRayStart = inverseViewMatrix * cameraRayStart;
    worldRayStart /= worldRayStart.w;
    glm::vec4 worldRayEnd = inverseViewMatrix * cameraRayEnd;
    worldRayEnd /= worldRayEnd.w;
    
    glm::vec3 worldRayDirection(worldRayEnd - worldRayStart);
    worldRayDirection = glm::normalize(worldRayDirection);
    glm::vec3 worldRayMax = glm::vec3(worldRayStart.x, worldRayStart.y, worldRayStart.z) + worldRayDirection * 1000.0f; 
    
    
    btVector3 Start, End;
    Start = btVector3(worldRayStart.x, worldRayStart.y, worldRayStart.z);
    End = btVector3(worldRayMax.x, worldRayMax.y, worldRayMax.z);
    
    //get the raycast callback ready
    btCollisionWorld::ClosestRayResultCallback closestResults(Start, End);
    closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
    
    m_dynamicsWorld->rayTest(Start, End, closestResults);
    
    //if it hit, grab the position of the collider, otherwise return {0, 0, 0}
    glm::vec3 cubePosition;
    if (closestResults.hasHit()) {
        btVector3 hitResults = closestResults.m_collisionObject->getWorldTransform().getOrigin();
        cubePosition = glm::vec3(hitResults.x(), hitResults.y(), hitResults.z());
#ifdef DEBUG
        printf("World position for cube was {%f, %f, %f}\n", cubePosition.x, cubePosition.y, cubePosition.z);
#endif
        //figure out which board that is by checking if the cube lies on the same plane use the board (cross products and such are necessary in case someone decides to make weird offsets like what's currently in the config)
        int board = 0;//the only board we have right now
        int row, col;
        glm::vec3 start = m_board->m_obj->GetTranslation();//m_obj translation is the first position for instancing
        glm::vec3 rowOffset = m_board->GetChangePerRow();
        glm::vec3 colOffset = m_board->GetChangePerCol();
        glm::vec3 tempPos = {0, 0, 0};
        unsigned int i, j;
        for (i = 0; i < m_board->m_obj->GetNumInstances().x && tempPos != cubePosition; i++) {
            for (j = 0; j < m_board->m_obj->GetNumInstances().y && tempPos != cubePosition; j++) {
                tempPos = start + ((float) i * rowOffset) + ((float) j * colOffset);
            }
        }
        row = i - 1;
        col = j - 1;
        cubePosition = glm::vec3(board, col, row);//i guess this just has to be backwards
    } else {
        cubePosition = {-1, -1, -1};
    }
    
#ifdef DEBUG
    printf("Game board position for cube is {%i, %i, %i}\n", cubePosition.x, cubePosition.y, cubePosition.z);
#endif
    
    return cubePosition;
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

//updates bindings for camera in shader - need to call for camera change to take effect
void Graphics::UpdateCameraBindings(void) {
	m_board->UpdateCameraBindings(m_camera.GetView(), m_camera.GetProjection(), m_camera.GetEyePos());
}
