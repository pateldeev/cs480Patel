#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics_headers.h"

#include "camera.h"
#include "shader.h"

#include "object.h"
#include "objects/board.h"
#include "objects/sphere.h"
#include "objects/cube.h"

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

	void Update(unsigned int dt);

	void AddObject(const objectModel & obj);

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
	//std::vector<std::string> m_objectNames; names now stored in objects
	
	//for bullet
	btBroadphaseInterface * mbt_broadphase;
	btDefaultCollisionConfiguration * mbt_collisionConfig;
	btCollisionDispatcher * mbt_dispatcher;
	btSequentialImpulseConstraintSolver * mbt_solver;
	btDiscreteDynamicsWorld * mbt_dynamicsWorld;
};

#endif /* GRAPHICS_H */
