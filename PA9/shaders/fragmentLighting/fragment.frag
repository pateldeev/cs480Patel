#version 330

smooth in vec2 texture;

smooth in vec3 pos;
smooth in vec3 normal;

uniform sampler2D sampler;

uniform vec3 ambientP;
uniform vec3 diffuseP;
uniform vec3 specularP;

uniform vec3 lightPos;
uniform vec3 eyePos;

uniform float shininess;

out vec4 frag_color;

void main(void){
  vec3 ambient = ambientP;

  vec3 N = normalize(normal);
  vec3 L = normalize(lightPos - pos);
  float Kd = max( dot(N,L), 0.0 );
  vec3 diffuse = Kd*diffuseP;

  vec3 viewDir = normalize(eyePos - pos);
  vec3 reflectDir = reflect(-L, N);     
  float Ks = pow( max(dot(viewDir, reflectDir), 0.0), shininess );
  vec3 specular = Ks * specularP;

  vec3 textureColor = texture2D(sampler, texture).xyz;

  frag_color = vec4((ambient + diffuse + specular) * textureColor, 1.0);
}
