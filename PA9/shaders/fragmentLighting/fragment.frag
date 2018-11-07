#version 330

smooth in vec2 texture;

smooth in vec3 v_posWorld;
smooth in vec3 v_normalWorld;

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

  vec3 E = normalize(eyePos - v_posWorld);
  vec3 L = normalize(lightPos - v_posWorld);
  vec3 H = normalize( L + E );
  vec3 N = normalize(v_normalWorld);
  
  float Kd = max( dot(L,N), 0.0 );
  vec3 diffuse = Kd*diffuseP;

  float Ks = pow( max(dot(N, H), 0.0), shininess );
  vec3 specular = Ks * specularP;

  vec3 textureColor = texture2D(sampler, texture).xyz;

  frag_color = vec4((ambient + diffuse + specular) * textureColor, 1.0);
}
