#version 330

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture;

flat out int instanceID;

smooth out vec3 v_posWorld;
smooth out vec3 v_normalWorld;
smooth out vec2 texture;

uniform mat4 model, view, projection;

uniform vec3 changeRow, changeCol;
uniform int numPerRow;

void main(void){
  vec3 translationRow = changeRow*(gl_InstanceID % numPerRow);
  vec3 translationCol = changeCol*(gl_InstanceID/numPerRow);
  vec3 v_posInstance = v_pos + translationRow + translationCol;
  
  instanceID = gl_InstanceID;
  
  v_posWorld = (model * vec4(v_posInstance, 1.0)).xyz;
  v_normalWorld = mat3(transpose(inverse(model))) * v_normal;  
  
  texture = v_texture;
  
  gl_Position = projection * view * vec4(v_posWorld, 1.0);
}
