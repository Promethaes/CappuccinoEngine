#version 420 core
out vec4 outColour;
in vec2 TextCoords;

uniform sampler2D text;
uniform vec3 textColour;

void main(){

vec4 sampled = vec4(1.0,1.0,1.0,texture(text,TextCoords).r);
outColour = vec4(textColour,1.0)*sampled;
}