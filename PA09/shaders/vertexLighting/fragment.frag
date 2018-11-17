#version 330

smooth in vec3 color;

smooth in vec2 texture;

uniform sampler2D sampler;

out vec4 frag_color;

void main(void) { 

  vec3 textureColor = texture2D(sampler, texture).xyz;

  frag_color = vec4(color * textureColor, 1.0);
}
