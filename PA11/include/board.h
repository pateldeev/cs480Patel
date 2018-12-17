#ifndef BOARD_H
#define BOARD_H

#include "object.h"
#include "shader.h"
#include <iostream>

#include <btBulletDynamicsCommon.h>

class Board {

public:
	Board(void) = delete;
	Board(const GameInfo & game);
	~Board(void);

	//Board is not meant to be copied or moved
	Board(const Board &) = delete;
	Board(Board &&) = delete;
	Board& operator=(const Board &) = delete;
	Board& operator=(Board&&) = delete;

	//to add and change shaders
	void AddShaderSet(const std::string & setName, const std::string & vertexShaderSrc, const std::string & fragmentShaderSrc);
	void UseShaderSet(const std::string & setName);

	void Update(void);
	void Render(void);

	//for changing lighting
	void ChangeAmbientLight(const glm::vec3 & change);
	void ChangeDiffuseLight(const glm::vec3 & change);
	void ChangeSpecularLight(const glm::vec3 & change);
	void UpdateSpotlightLoc(const glm::vec3 & location);

	//for updating the bindings of uniforms
	void UpdateCameraBindings(const glm::mat4 & viewMat, const glm::mat4 & projectionMat, const glm::vec3 & cameraPos);
	void UpdateLightBindings(void);
	void UpdateInstanceBindings(Object * obj);

	const btDiscreteDynamicsWorld * GetBulletWorld(void) const;

	//functions to interface with game elements
	//uvec3: {face - BoardSides enumeration, row, column}
	glm::uvec3 GetGameElementByPosition(const glm::vec3 & position) const;
	std::vector<glm::uvec3> GetGameElementNeighbors(const glm::uvec3 & element) const;
	ObjType GetGameElementType(const glm::uvec3 & element) const;
	void SetGameElementType(const glm::uvec3 & element, const ObjType type = ObjType::DEAD);
	glm::uvec3 GetNextGameElement(const glm::uvec3 & currentElement) const;

	glm::uvec2 GetScore(void) const;

private:
	void EnforceBounds(glm::vec3 & v, float min = 0.f, float max = 1.f); //rounds everything to be in range [min, max]

	void InitializeBullet(void); //start bullet world
	void LoadColliders(void); //loads one side of game board
	void AddCubeColliderToWorld(const glm::vec3 & position, const glm::vec3 & rotation, const glm::vec3 & scale); //adds cube collider to bullet world

	void FindElementNeighborsInFace(std::vector<glm::uvec3> & neighbors, const glm::uvec3 & element) const; //helper function to get neighbors in same side as object

	Object * m_sides[BoardSides::NUM_SIDES];

	//variable to keep track of shaders
	std::vector<std::pair<std::string, Shader *>> m_shaders;
	Shader * m_shaderCurrent;

	//for unifroms in shaders for translation and lighting
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;
	GLint m_lightPos;
	GLint m_cameraPos;
	GLint m_ambientProduct;
	GLint m_diffuseProduct;
	GLint m_specularProduct;
	GLint m_shininess;
	glm::vec3 m_ambientLevel;
	glm::vec3 m_diffuseLevel;
	glm::vec3 m_specularLevel;
	float m_shininessConst;
	glm::vec3 m_spotlightLoc;

	//for instancing
	GLint m_instanceChangeRow;
	GLint m_instanceChangeCol;
	GLint m_instanceNumPerRow;
	GLint m_samplers;
	GLint m_sampleTypes; //type of texture to use on each object

	//bullet variables - needed for ray casting
	btBroadphaseInterface * m_broadphase;
	btDefaultCollisionConfiguration * m_collisionConfiguration;
	btCollisionDispatcher * m_dispatcher;
	btSequentialImpulseConstraintSolver * m_solver;
	btDiscreteDynamicsWorld * m_dynamicsWorld;
	std::vector<btRigidBody*> m_rigidBodies;
};

#endif /* BOARD_H */
