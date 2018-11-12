#version 330

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture;

smooth out vec3 color;

smooth out vec2 texture;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec3 ambientP;
uniform vec3 diffuseP;
uniform vec3 specularP;
uniform float shininess;

uniform vec3 lightPos[3];
uniform vec3 eyePos;

//function to add contribution of one light
vec3 addLight(vec3 baseColor, vec3 lightPosition, vec3 v_posWorld, vec3 v_normalWorld);

void main(void){
  vec3 v_posWorld = (modelMatrix * vec4(v_pos, 1.0)).xyz;
  gl_Position = projectionMatrix * viewMatrix * vec4(v_posWorld, 1.0);
  vec3 v_normalWorld = mat3(transpose(inverse(modelMatrix))) * v_normal;

  vec3 baseColor = vec3(1.0,1.0,1.0);

  //add contribution of each light
  for(int i = 0; i <3; ++i)
    baseColor = addLight(baseColor, lightPos[i], v_posWorld, v_normalWorld);

  color = baseColor;

  texture = v_texture;
}

vec3 addLight(vec3 baseColor, vec3 lightPosition, vec3 v_posWorld, vec3 v_normalWorld){

  vec3 E = normalize(eyePos - v_posWorld);
  vec3 L = normalize(lightPosition - v_posWorld);
  vec3 H = normalize(L + E);
  vec3 N = normalize(v_normalWorld);
  
  vec3 ambient = ambientP;

  float Kd = max(dot(L, N), 0.0);
  vec3 diffuse = Kd * diffuseP;

  float Ks = pow(max(dot(N, H), 0.0), shininess);
  vec3 specular = Ks * specularP;

  return ((ambient + diffuse + specular) * baseColor);
}
