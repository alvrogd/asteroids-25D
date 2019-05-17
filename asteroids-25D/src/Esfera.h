#ifndef ESFERA_H
#define ESFERA_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

// Librer�as de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Esfera
{
public:
	/* Constructor */

	Esfera (float radio, int sectores, int stacks);


	/* Destructor */

	~Esfera ();


	/* M�todos */

	void dibujar() const;


private:
	/* Atributos */

	// Radio de la esfera
	float radio;

	// Longitud
	int sectores;

	// Latitud
	int stacks;

	// V�rtices
	std::vector<float> vertices;

	// Normales
	std::vector<float> normales;

	// Coordenadas para las texturas
	std::vector<float> texturasCoordenadas;

	// V�rtices intercalados con sus propiedades
	std::vector<float> verticesIntercalados;

	// �ndices de los v�rtices para el dibujado
	std::vector<int> indices;

	// VBO, EBO y VAO para su representaci�n
	unsigned int VBO = 0, EBO = 0, VAO = 0;


	/* M�todos */

	// Para generar la esfera a partir de sus propiedades
	void construirEsfera ();

	// Generaci�n de los v�rtices
	void generarVertices ();

	// Generaci�n de los �ndices
	void generarIndices ();

	// Juntado de v�rtices y sus propiedades en un �nico vector para el dibujado mediante VBO
	void juntarVerticesPropiedades ();

	// Generaci�n del VAO para representar la esfera
	void generarVAO ();
};

#endif
