#version 330

smooth in vec2 texture;

smooth in vec3 fN;
smooth in vec3 fL;
smooth in vec3 fE;

uniform sampler2D sampler;

uniform vec3 ambientProduct;
uniform vec3 diffuseProduct;
uniform vec3 specularProduct;

uniform float shininess;

out vec4 frag_color;

void main(void){
  vec3 N = normalize(fN);
  vec3 E = normalize(fE);
  vec3 L = normalize(fL);
  vec3 H = normalize(fL + fE);

  float kD = max(dot(L,N), 0.0);
  float kS = (dot(L,N) >= 0.0) ? pow(max(dot(N, H), 0.0), shininess) : 0.0;

  vec3 color = ambientProduct + kD*diffuseProduct + kS*specularProduct;

  //frag_color = texture2D(sampler, texture);
frag_color = vec4(0,0,0,1);  
frag_color += vec4(color, 0.0);
}
