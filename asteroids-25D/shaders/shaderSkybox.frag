#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}