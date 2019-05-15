#version 330 core

// Salida del shader
out vec4 FragColor;

// Color dado desde el programa
uniform vec4 Color;

void main()
{
	// Se emplea el color dado desde la aplicación
	FragColor = Color;
}