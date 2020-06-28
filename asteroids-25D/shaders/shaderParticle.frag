#version 330 core

// Output
out vec4 FragColor;

// Color established directly from the CPU
uniform vec4 Color;

void main()
{
	FragColor = Color;
}