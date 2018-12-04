#version 330

smooth in vec2 texture;

smooth in vec3 v_posWorld;
smooth in vec3 v_normalWorld;

uniform sampler2D samplers[11];

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
    baseColor = texture2D(samplers[1], texture).xyz;
  else if(sampleType[instanceID] == 2)
    baseColor = texture2D(samplers[2], texture).xyz;
  else if(sampleType[instanceID] == 3)
    baseColor = texture2D(samplers[3], texture).xyz;
  else if(sampleType[instanceID] == 4)
    baseColor = texture2D(samplers[4], texture).xyz;
  else if(sampleType[instanceID] == 5)
    baseColor = texture2D(samplers[5], texture).xyz;
  else if(sampleType[instanceID] == 6)
    baseColor = texture2D(samplers[6], texture).xyz;
  else if(sampleType[instanceID] == 7)
    baseColor = texture2D(samplers[7], texture).xyz;
  else if(sampleType[instanceID] == 8)
    baseColor = texture2D(samplers[8], texture).xyz;
  else if(sampleType[instanceID] == 9)
    baseColor = texture2D(samplers[9], texture).xyz;
  else if(sampleType[instanceID] == 10)
    baseColor = texture2D(samplers[10], texture).xyz;
  else
    baseColor = texture2D(samplers[0], texture).xyz;

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
