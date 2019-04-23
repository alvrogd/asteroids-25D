#version 330 core

// Input
layout (location = 0) in vec3 aPos;

// Output para el fragment shader
out vec3 TexCoords;

// Matrices de transformaciones
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


void main()
{
	// Se devuelve directamente la coordenada de textura
	TexCoords = aPos;

	// Se calcula la posición del vértice aplicando todas las matrices de transformación
	vec4 pos = projectionMatrix * viewMatrix * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}