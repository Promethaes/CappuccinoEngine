
#version 420 core
out vec4 FragColor;
uniform vec4 Colour;

void main()
{
    FragColor = vec4(1.0,1.0,1.0,1.0).xyzw; // set all 4 vector values to 1.0
}