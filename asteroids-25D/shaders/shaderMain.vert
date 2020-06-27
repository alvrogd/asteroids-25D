#version 330 core

// Input
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Output data read by the fragment shader
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

// Transformation matrixes
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix; // Already computed in the CPU in order to avoid computing it for each vertex


void main()
{
	// The coordiantes of the fragment that is being rendered are transformed into world coordinates
	FragPos = vec3(modelMatrix * vec4(aPos, 1.0));

	// The normal matrix is applied in orden to avoid non-unitary transformations
	Normal = normalMatrix * aNormal;

	// The texture coordinate is returned as it is
	TexCoord = aTexCoord;


	// The position depends on the given transformation matrixes
	gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}