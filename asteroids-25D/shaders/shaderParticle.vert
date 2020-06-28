#version 330 core

// Input
layout (location = 0) in vec3 aPos;

// Transformation matrixes
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	// The position depends on the given transformation matrixes
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
}