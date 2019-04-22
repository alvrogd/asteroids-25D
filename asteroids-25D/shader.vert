#version 330 core

// Input
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Output para el fragment shader
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

// Matrices de transformaciones
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
// Se calcula en la CPU para evitar calcularla para cada v�rtice
uniform mat3 normalMatrix;

void main()
{
	/* Informaci�n para el fragment shader */

	// La coordenada del fragmento a renderizar se devuelve en coordenadas del mundo
	FragPos = vec3(modelMatrix * vec4(aPos, 1.0));

	// Se devuelve la normal aplic�ndole la matriz normal para evitar transformaciones no unitarias
	Normal = normalMatrix * aNormal;

	// Se devuelve directamente la coordenada de textura
	TexCoord = aTexCoord;


	/* Renderizado */

	// Se calcula la posici�n del v�rtice aplicando todas las matrices de transformaci�n
	gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}