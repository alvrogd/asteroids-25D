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

	// Se calcula la posici�n del v�rtice aplicando todas las matrices de transformaci�n dadas
	vec4 pos = projectionMatrix * viewMatrix * vec4(aPos, 1.0);

	// Y se devuelve el resultado cambiando la coordenada 'z' por la 'w' -> en la "divisi�n de perspectiva", se divide
	// cada una de las coordenadas del v�rtice por 'w', y el valor de 'z' es el empleado para el depth buffer; al
	// reemplazar 'z' por 'w', se consigue que cada v�rtice tenga siempre un valor de profunidad "w / w = 1.0", es
	// decir, el valor m�ximo de profunidad
	gl_Position = pos.xyww;
}