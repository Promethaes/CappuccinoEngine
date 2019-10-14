#version 420 core
layout (location = 0) in vec3 aPos;


uniform mat4 projection;
uniform vec4 colour;

out vec4 Acolour;
void main(){

    gl_Position = projection * vec4(aPos, 1.0);
    Acolour = colour;
} 