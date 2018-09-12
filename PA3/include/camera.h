#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera(void);
    ~Camera(void);
    bool Initialize(int w, int h, const glm::vec3 & eyePos );
    glm::mat4 GetProjection(void);
    glm::mat4 GetView(void);
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
