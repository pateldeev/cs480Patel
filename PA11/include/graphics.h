#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "camera.h"
#include "board.h"

#include <SDL2/SDL.h>

#include <btBulletDynamicsCommon.h>

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

	//for changing lighting
	void ChangeAmbientLight(const glm::vec3 & change);
	void ChangeDiffuseLight(const glm::vec3 & change);
	void ChangeSpecularLight(const glm::vec3 & change);

private:
	std::string ErrorString(const GLenum error) const;
	void UpdateCameraBindings(void); //updates bindings for camera in shader - need to call for camera change to take effect

	Camera m_camera;

	Board * m_board;
};

#endif /* GRAPHICS_H */
