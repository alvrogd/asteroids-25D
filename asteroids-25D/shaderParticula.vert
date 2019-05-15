#version 330 core

// Input
layout (location = 0) in vec3 aPos;

// Matrices de transformaciones
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	// Se devuelve la posición dada aplicando la matriz de transformación
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
}