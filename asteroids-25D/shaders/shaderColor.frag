#version 330 core

// Output
out vec4 FragColor;

// Color established directly from the CPU
uniform vec3 Color;

void main()
{
	FragColor = vec4(Color, 1.0);
}