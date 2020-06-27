#version 330 core

// Input
layout (location = 0) in vec3 aPos;

// Output data read by the fragment shader
out vec3 TexCoords;

// Transformation matrixes
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


void main()
{
	// The texture coordinate is returned as it is
	TexCoords = aPos;

	// The vertex's position depends on the given transformation matrixes
	vec4 pos = projectionMatrix * viewMatrix * vec4(aPos, 1.0);

	// The Z coodinate must be replaced by the W coordiante in the final result
	// 
	// In the perspective division process, each coordinate is divided by W, and the Z value is used in the depth
	// buffer. Therefore, by replacing Z with W, each vertex always has a depth value of "w / w = 1.0"; that is, the
	// maximum allowed value, in order to properly render the skybox
	gl_Position = pos.xyww;
}