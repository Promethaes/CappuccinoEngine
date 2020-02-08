#version 420 core

out vec4 outColour;

uniform vec4 ourColour;
void main()
{
	outColour = ourColour;
}