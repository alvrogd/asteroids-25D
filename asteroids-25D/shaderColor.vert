#version 330 core

// Input
layout (location = 0) in vec3 aPos;

// Matrices de transformaciones
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	// Se devuelve la posici�n dada aplicando la matriz de transformaci�n
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
}