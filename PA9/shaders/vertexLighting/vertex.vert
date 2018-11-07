#version 330

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture;

smooth out vec3 color;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec3 ambientP;
uniform vec3 diffuseP;
uniform vec3 specularP;

uniform float shininess;

uniform vec3 lightPos;
uniform vec3 eyePos;

uniform sampler2D sampler;

void main(void){
    vec3 v_posWorld = (modelMatrix * vec4(v_pos, 1.0)).xyz;
    gl_Position = projectionMatrix * viewMatrix * vec4(v_posWorld, 1.0);
	
    vec3 L = normalize(lightPos - v_posWorld);
    vec3 E = normalize(eyePos - v_posWorld);
    vec3 H = normalize( L + E );
    vec3 N = normalize(mat3(transpose(inverse(modelMatrix))) * v_normal);

    vec3 ambient = ambientP;

    float Kd = max( dot(L, N), 0.0 );
    vec3 diffuse = Kd*diffuseP;
    
    float Ks = pow( max(dot(N, H), 0.0), shininess );
    vec3 specular = Ks * specularP;
    if( dot(L, N) < 0.0 ) specular = vec3(0.0, 0.0, 0.0); 

    vec3 textureColor = texture2D(sampler, v_texture).xyz;

    color = (ambient + diffuse + specular) * textureColor;
}
