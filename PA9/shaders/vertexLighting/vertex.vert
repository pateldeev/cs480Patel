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

uniform sampler2D sampler;

void main(void){
    vec3 v_posMV = (viewMatrix * modelMatrix * vec4(v_pos, 1.0)).xyz;;
    gl_Position = projectionMatrix * vec4(v_posMV, 1.0);

    vec3 lightPostionMV = (viewMatrix * vec4(lightPos, 1.0)).xyz;
	
    vec3 L = normalize(lightPostionMV - v_posMV);
    vec3 E = normalize(-v_posMV);
    vec3 H = normalize( L + E );
    vec3 N = normalize(viewMatrix * modelMatrix * vec4(v_normal, 0.0)).xyz;

    vec3 ambient = ambientP;

    float Kd = max( dot(L, N), 0.0 );
    vec3 diffuse = Kd*diffuseP;
    
    float Ks = pow( max(dot(N, H), 0.0), shininess );
    vec3 specular = Ks * specularP;
    if( dot(L, N) < 0.0 ) specular = vec3(0.0, 0.0, 0.0); 

    vec3 textureColor = texture2D(sampler, v_texture).xyz;

    color = (ambient + diffuse + specular) * textureColor;
}
