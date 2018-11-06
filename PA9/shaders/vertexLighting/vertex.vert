#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture;

smooth out vec2 texture;

smooth out vec3 fN;
smooth out vec3 fE;
smooth out vec3 fL;

uniform vec3 lightPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void){
  fN = v_normal;
  fE = v_position;
  fL = lightPosition;

  texture = v_texture;

  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(v_position, 1.0);
}
