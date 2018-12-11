#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "camera.h"
#include "board.h"

#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>

class Graphics {
public:
	Graphics(void) = delete;
	Graphics(const glm::uvec2 & windowSize, const glm::vec3 & eyePos, const glm::vec3 & eyeFocus, const GameInfo & game);
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
	void MoveUp(float moveAmount);
	void MoveDown(float moveAmount);
	void RotateCamera(float newX, float newY);

	void UpdateCamera(const glm::vec3 & eyePos, const glm::vec3 & eyeFocus);
	glm::vec3 GetEyePos(void) const;
	glm::vec3 GetEyeLoc(void) const;

	//for changing lighting
	void ChangeAmbientLight(const glm::vec3 & change);
	void ChangeDiffuseLight(const glm::vec3 & change);
	void ChangeSpecularLight(const glm::vec3 & change);

	void LeftClick(const glm::vec2 & mousePosition); //call this from engine, and it should successfully click on object

private:
	std::string ErrorString(const GLenum error) const;

	void UpdateCameraBindings(void); //updates bindings for camera in shader - need to call for camera change to take effect

	glm::vec3 GetPositionUnder(const glm::vec2 & mousePosition); //raycast and find position mouse is pointing at

	Camera m_camera;

	float m_yaw;
	float m_pitch;

	Board * m_board;

	glm::uvec2 m_screenSize; //required for calculating mouse position in 3d space
};

#endif /* GRAPHICS_H */
