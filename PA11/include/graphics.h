#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "camera.h"
#include "board.h"

#include <SDL2/SDL.h>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>

class Graphics {
public:
	Graphics(void) = delete;
	Graphics(unsigned int windowWidth, unsigned int windowHeight, const glm::vec3 & eyePos, const glm::vec3 & eyeFocus, const boardInfo & board);
	~Graphics(void);

	//Graphics is not meant to be copied or moved
	Graphics(const Graphics &) = delete;
	Graphics(Graphics &&) = delete;
	Graphics& operator=(const Graphics &) = delete;
	Graphics& operator=(Graphics&&) = delete;
        
	//to add and change shaders
	void AddShaderSet(const std::string & setName, const std::string & vertexShaderSrc, const std::string & fragmentShaderSrc);
	void UseShaderSet(const std::string & setName);

	void Update(unsigned int dt);
	void Render(void);

	//for movement for camera
	void IncreaseEyePosX(float moveAmount);
	void IncreaseEyePosZ(float moveAmount);
	void DecreaseEyePosX(float moveAmount);
	void DecreaseEyePosZ(float moveAmount);
	void ZoomIn(float moveAmount);
	void ZoomOut(float moveAmount);
	void MoveForward(float moveAmount);
	void MoveBackward(float moveAmount);
	void MoveRight(float moveAmount);
	void MoveLeft(float moveAmount);

	void UpdateCamera(const glm::vec3 & eyePos, const glm::vec3 & eyeFocus);
	glm::vec3 GetEyePos(void) const;
	glm::vec3 GetEyeLoc(void) const;
        
        //change screen width/height in graphics (needed for calculating mouse in 3d space)
        void SetScreenWidth(float width);
        void SetScreenHeight(float height);

	//for changing lighting
	void ChangeAmbientLight(const glm::vec3 & change);
	void ChangeDiffuseLight(const glm::vec3 & change);
	void ChangeSpecularLight(const glm::vec3 & change);
       
        //Dat click doe
        void LeftClick(glm::vec2 mousePosition);//call this from engine, and it should successfully do everything 

private:
        //bullet for raycasting
        bool InitializeBullet();//create the dynamics world (called by the constructor)
        void AddCubeColliderToWorld(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);//need one for every cube in game
        void GetCubePositions(boardInfo board);//generates cube positions and rotations based on boardInfo from config
        glm::vec3 GetCubeUnderPosition(glm::vec2 mousePosition);//returns position of the object hit by the raycast
        
        btBroadphaseInterface *m_broadphase;
        btDefaultCollisionConfiguration *m_collisionConfiguration;
        btCollisionDispatcher *m_dispatcher;
        btSequentialImpulseConstraintSolver *m_solver;
        btDiscreteDynamicsWorld *m_dynamicsWorld;
        std::vector<btRigidBody*> m_rigidBodies;
	
        std::string ErrorString(const GLenum error) const;
	void UpdateCameraBindings(void); //updates bindings for camera in shader - need to call for camera change to take effect

	Camera m_camera;
        
        //required for calculating mouse position in 3d space, could be stored in camera (it'd clean this up and make more sense organizationally)
        float m_screenWidth;
        float m_screenHeight;
        
	Board * m_board;      
};

#endif /* GRAPHICS_H */
