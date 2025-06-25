#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
out vec4 ourPosition;
uniform float xOffset;
uniform float yOffset;
void main()
{
	ourPosition = vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0);
	gl_Position = ourPosition;
	ourColor = aColor;
}