#version 330

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture;

flat out int obj_row, obj_col;

smooth out vec3 v_posWorld, v_normalWorld;
smooth out vec2 texture;

uniform mat4 model, view, projection;

uniform vec3 changeRow, changeCol;
uniform int numPerRow;

void main(void){
  obj_row = gl_InstanceID / numPerRow;
  obj_col = gl_InstanceID % numPerRow;
  vec3 v_posInstance = v_pos + (changeRow * obj_row) + (changeCol * obj_col);
  
  v_posWorld = (model * vec4(v_posInstance, 1.0)).xyz;
  v_normalWorld = mat3(transpose(inverse(model))) * v_normal;  
  
  texture = v_texture;
  
  gl_Position = projection * view * vec4(v_posWorld, 1.0);
}
