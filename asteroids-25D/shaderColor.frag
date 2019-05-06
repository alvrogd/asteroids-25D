#version 330 core

// Salida del shader
out vec4 FragColor;

// Color dado desde el programa
uniform vec3 Color;

void main()
{
	// Se emplea el color dado desde la aplicación
	FragColor = vec4(Color, 1.0);
}