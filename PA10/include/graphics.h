#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics_headers.h"

#include "camera.h"
#include "shader.h"
#include "objects/object.h"
#include "configLoader.hpp"

#include <SDL2/SDL.h>
#include <vector>

#include <btBulletDynamicsCommon.h>

class Graphics {
public:
	Graphics(void);
	~Graphics(void);

	//initalizes for various parts of graphics class
	bool Initialize(unsigned int windowWidth, unsigned int windowHeight, const glm::vec3 & eyePos, const glm::vec3 & focusPos);
	bool InitializeBt(const glm::vec3 & gravity);
	bool InitializeLighting(const glm::vec3 & ambientLevel, const float shininess);

	void Update(unsigned int dt);

	void AddObject(const objectModel & obj);
	bool VerifyObjects(void) const; //function to verify all pinball objects are in place
	void ResetBall(void);

	//to control impulse of ball
	void ApplyImpulse(const glm::vec3 & impulse, const glm::vec3 & spin = glm::vec3(1, 1, 1)); //apply impulse to ball
	void ApplyForce(const glm::vec3 & force, const glm::vec3 & spin = glm::vec3(1, 1, 1)); //apply force to ball
	void SetLinearVelocity(const glm::vec3 & vel, bool accumulate = false); //apply velocity to ball
	void SetAngularVelocity(const glm::vec3 & vel, bool accumulate = false); //apply velocity to ball

	void MovePaddleR(void);
	void SetResetFlagPaddleR(bool flag);
	void MovePaddleL(void);
	void SetResetFlagPaddleL(bool flag);

	//for movement for camera
	void IncreaseEyePosX(float moveAmount);
	void IncreaseEyePosZ(float moveAmount);
	void DecreaseEyePosX(float moveAmount);
	void DecreaseEyePosZ(float moveAmount);
	void ZoomIn(float moveAmount);
	void ZoomOut(float moveAmount);

	bool AddShaderSet(const std::string & setName, const std::string & vertexShaderSrc, const std::string & fragmentShaderSrc);
	bool UseShaderSet(const std::string & setName, bool hasLighting = true);

	bool UpdateCamera(const glm::vec3 & eyePos, const glm::vec3 & eyeFocus);

	void Render(void);

	glm::vec3 GetEyePos(void) const;
	glm::vec3 GetEyeLoc(void) const;

	//for changing lighting of ball
	void SetAmbientLight(const glm::vec3 & change);
	void SetDiffuseofBall(const glm::vec3 & change);
	void SetSpecularofBall(const glm::vec3 & change);
	void SetSpotlightHeight(float change);

	void AddSpotLight(const glm::vec3 & location);

	//for lives and scoring
	int GetScore(void) const;
	int GetNumLives(void) const;
	void StartLife(void);
	int GetLives(void);

private:
	std::string ErrorString(const GLenum error) const;

	Camera * m_camera;

	//variables to keep track of shaders
	std::vector<Shader *> m_shaders;
	std::vector<std::string> m_shaderNames;
	int m_currentShader;

	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;

	//for lighthing
	GLint m_lightPos;
	GLint m_cameraPos;
	GLint m_ambientProduct;
	GLint m_diffuseProduct;
	GLint m_specularProduct;
	GLint m_shininess;
	bool m_lightingStatus;
	glm::vec3 m_ambientLevel;
	float m_shininessConst;
	std::vector<glm::vec3> m_spotlightLocs;
	float m_spotLightHeight;

	void UpdateBallSpotlight(void); //update spotlight to be above following object

	std::vector<Object *> m_objects;
	int m_ball; //index of object being controlled by user input
	int m_paddleR; //index of right paddles object
	int m_paddleL; //index of left paddles object
	int m_board; //index of board object
	std::vector<unsigned int> m_renderOrder;
	std::vector<int> m_objectScores;
	int m_score; //total score
	btRigidBody * m_scoreLastObj; //last object that was hit for points
	int m_lives;

	std::vector<glm::vec3> m_startingLocs; //starting location of objects
	std::vector<glm::vec3> m_startingRotations; //starting locations of objects
	std::vector<glm::vec3> m_objectsDiffuseProducts;
	std::vector<glm::vec3> m_objectsSpecularProducts;

	//for bullet
	btBroadphaseInterface * mbt_broadphase;
	btDefaultCollisionConfiguration * mbt_collisionConfig;
	btCollisionDispatcher * mbt_dispatcher;
	btSequentialImpulseConstraintSolver * mbt_solver;
	btDiscreteDynamicsWorld * mbt_dynamicsWorld;

	void UpdateScore(void);
	void UpdateLives(void);
};

#endif /* GRAPHICS_H */
