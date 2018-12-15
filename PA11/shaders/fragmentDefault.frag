#version 330

flat in int obj_row, obj_col;

smooth in vec3 v_posWorld, v_normalWorld;
smooth in vec2 texture;

out vec4 frag_color;

uniform sampler2D samplers[11];
uniform int sampleType[500];
uniform int numPerRow;

uniform vec3 ambientP, diffuseP, specularP;
uniform float shininess;

uniform vec3 lightPos;
uniform vec3 eyePos;

//function to add contribution of one light
vec3 addLight(vec3 baseColor, vec3 lightPosition);

void main(void){
  vec4 baseColor;

  int currentSample = sampleType[obj_row * numPerRow + obj_col]; 
  
  if(currentSample== 1)
    baseColor = texture2D(samplers[1], texture);
  else if(currentSample == 2)
    baseColor = texture2D(samplers[2], texture);
  else if(currentSample == 3)
    baseColor = texture2D(samplers[3], texture);
  else if(currentSample == 4)
    baseColor = texture2D(samplers[4], texture);
  else if(currentSample == 5)
    baseColor = texture2D(samplers[5], texture);
  else if(currentSample == 6)
    baseColor = texture2D(samplers[6], texture);
  else if(currentSample == 7)
    baseColor = texture2D(samplers[7], texture);
  else if(currentSample == 8)
    baseColor = texture2D(samplers[8], texture);
  else if(currentSample == 9)
    baseColor = texture2D(samplers[9], texture);
  else if(currentSample == 10)
    baseColor = texture2D(samplers[10], texture);
  else
    baseColor = texture2D(samplers[0], texture);

  //add contribution of light
  baseColor = vec4(addLight(baseColor.rgb, lightPos), baseColor.a);

  frag_color = baseColor;
}

vec3 addLight(vec3 baseColor, vec3 lightPosition){
  vec3 E = normalize(eyePos - v_posWorld);
  vec3 N = normalize(v_normalWorld);
  vec3 L = normalize(lightPosition - v_posWorld);
  vec3 H = normalize(L + E);
  
  vec3 ambient = ambientP;

  float kD = max(dot(N, L), 0.0);
  vec3 diffuse = diffuseP * kD;

  float kS = pow(max(dot(N, H), 0.0), shininess);
  vec3 specular = specularP * kS;
  if(dot(N, L) < 0.0) 
    specular = vec3(0.0,0.0,0.0);

  float distance = length(lightPosition - v_posWorld);
  float attenuation = 1.0 / (1.0 + 0.009 * distance + 0.0032 * (distance * distance));
  diffuse *= attenuation;    
  specular *= attenuation;    

  return ((ambient + diffuse + specular) * baseColor);
}
