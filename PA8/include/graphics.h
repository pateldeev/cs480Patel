#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics_headers.h"

#include "camera.h"
#include "shader.h"

#include "object.h"

#include "configLoader.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

#include <btBulletDynamicsCommon.h>

class Graphics {
public:
	Graphics(void);
	~Graphics(void);

	bool Initialize(unsigned int windowWidth, unsigned int windowHeight, const glm::vec3 & eyePos, const glm::vec3 & focusPos);
	bool InitializeBt(const glm::vec3 & gravity);

	void Update(unsigned int dt);

	void AddObject(const objectModel & obj);
	
	void moveSphere(const glm::vec3 & impulse);

	bool AddShaderSet(const std::string & setName, const std::string & vertexShaderSrc, const std::string & fragmentShaderSrc);
	bool UseShaderSet(const std::string & setName);

	bool UpdateCamera(const glm::vec3 & eyePos, const glm::vec3 & eyeFocus);

	void Render(void);

	glm::vec3 GetEyePos(void) const;
	glm::vec3 GetEyeLoc(void) const;

private:
	std::string ErrorString(const GLenum error) const;

	Camera * m_camera;

	//variables to keep track of shaders
	std::vector<Shader> m_shaders;
	std::vector<std::string> m_shaderNames;
	int m_currentShader;

	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;

	std::vector<std::unique_ptr<Object>> m_objects;
	int m_movingObject; //points to object that can be moved via user keys
	
	//for bullet
	btBroadphaseInterface * mbt_broadphase;
	btDefaultCollisionConfiguration * mbt_collisionConfig;
	btCollisionDispatcher * mbt_dispatcher;
	btSequentialImpulseConstraintSolver * mbt_solver;
	btDiscreteDynamicsWorld * mbt_dynamicsWorld;
};

#endif /* GRAPHICS_H */
