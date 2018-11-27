#version 330

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture;

smooth out vec2 texture;

flat out int instanceID;

smooth out vec3 v_posWorld;
smooth out vec3 v_normalWorld;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec3 changeRow, changeCol;
uniform int numPerRow;

void main(void){
  vec3 translationRow = changeRow*(gl_InstanceID % numPerRow);
  vec3 translationCol = changeCol*(gl_InstanceID/numPerRow);
  vec3 v_posInstance = v_pos + translationRow + translationCol;
  v_posWorld = (modelMatrix * vec4(v_posInstance, 1.0)).xyz;

  gl_Position = projectionMatrix * viewMatrix * vec4(v_posWorld, 1.0);
  instanceID = gl_InstanceID;

  v_normalWorld = mat3(transpose(inverse(modelMatrix))) * v_normal;  

  texture = v_texture;
}
