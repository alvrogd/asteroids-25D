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

	// Se calcula la posición del vértice aplicando todas las matrices de transformación dadas
	vec4 pos = projectionMatrix * viewMatrix * vec4(aPos, 1.0);

	// Y se devuelve el resultado cambiando la coordenada 'z' por la 'w' -> en la "división de perspectiva", se divide
	// cada una de las coordenadas del vértice por 'w', y el valor de 'z' es el empleado para el depth buffer; al
	// reemplazar 'z' por 'w', se consigue que cada vértice tenga siempre un valor de profunidad "w / w = 1.0", es
	// decir, el valor máximo de profunidad
	gl_Position = pos.xyww;
}