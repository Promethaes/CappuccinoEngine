#version 420 core
layout (location = 0) in vec4 aPos;
out vec2 TextCoords;

uniform mat4 projection;

void main(){
    gl_Position = projection * vec4(aPos.xy,0.0, 1.0);
    TextCoords = aPos.zw;
}
