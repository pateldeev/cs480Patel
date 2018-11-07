#version 330

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture;

smooth out vec2 texture;

smooth out vec3 pos;
smooth out vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void){
    pos = vec3(modelMatrix * vec4(v_pos, 1.0));
    gl_Position = projectionMatrix * viewMatrix * vec4(pos, 1.0);

    normal = mat3(transpose(inverse(modelMatrix))) * v_normal;  

    texture = v_texture;
}
