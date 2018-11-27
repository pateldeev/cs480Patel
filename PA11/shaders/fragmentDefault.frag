#version 330

smooth in vec2 texture;

smooth in vec3 v_posWorld;
smooth in vec3 v_normalWorld;

uniform sampler2D samplerDead;
uniform sampler2D samplerP1_Alive;
uniform sampler2D samplerP2_Alive;
uniform sampler2D samplerP1_Dying;
uniform sampler2D samplerP2_Dying;
uniform sampler2D samplerP1_Marked;
uniform sampler2D samplerP2_Marked;

uniform int sampleType[150];
flat in int instanceID;

uniform vec3 ambientP;
uniform vec3 diffuseP;
uniform vec3 specularP;
uniform float shininess;

uniform vec3 lightPos[3];
uniform vec3 eyePos;

out vec4 frag_color;

//function to add contribution of one light
vec3 addLight(vec3 baseColor, vec3 lightPosition);

void main(void){
  vec3 baseColor; 
  
  if(sampleType[instanceID] == 1)
    baseColor = texture2D(samplerP1_Alive, texture).xyz;
  else if(sampleType[instanceID] == 2)
    baseColor = texture2D(samplerP2_Alive, texture).xyz;
  else if(sampleType[instanceID] == 3)
    baseColor = texture2D(samplerP1_Dying, texture).xyz;
  else if(sampleType[instanceID] == 4)
    baseColor = texture2D(samplerP2_Dying, texture).xyz;
  else if(sampleType[instanceID] == 5)
    baseColor = texture2D(samplerP1_Marked, texture).xyz;
  else if(sampleType[instanceID] == 6)
    baseColor = texture2D(samplerP2_Marked, texture).xyz;
  else
    baseColor = texture2D(samplerDead, texture).xyz;

  //add contribution of each light
  for(int i = 0; i <3; ++i)
    baseColor = addLight(baseColor, lightPos[i]);

  frag_color = vec4(baseColor, 1.0);
}

vec3 addLight(vec3 baseColor, vec3 lightPosition){
  vec3 E = normalize(eyePos - v_posWorld);
  vec3 N = normalize(v_normalWorld);
  vec3 L = normalize(lightPosition - v_posWorld);
  vec3 H = normalize(L + E);
  
  vec3 ambient = ambientP;

  float Kd = max(dot(L, N), 0.0);
  vec3 diffuse = Kd * diffuseP;

  float Ks = pow(max(dot(N, H), 0.0), shininess);
  vec3 specular = Ks * specularP;

  return ((ambient + diffuse + specular) * baseColor);
}
