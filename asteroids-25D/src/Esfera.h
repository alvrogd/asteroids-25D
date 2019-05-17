#ifndef ESFERA_H
#define ESFERA_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Esfera
{
public:
	/* Constructor */

	Esfera (float radio, int sectores, int stacks);


	/* Destructor */

	~Esfera ();


	/* Métodos */

	void dibujar() const;


private:
	/* Atributos */

	// Radio de la esfera
	float radio;

	// Longitud
	int sectores;

	// Latitud
	int stacks;

	// Vértices
	std::vector<float> vertices;

	// Normales
	std::vector<float> normales;

	// Coordenadas para las texturas
	std::vector<float> texturasCoordenadas;

	// Vértices intercalados con sus propiedades
	std::vector<float> verticesIntercalados;

	// Índices de los vértices para el dibujado
	std::vector<int> indices;

	// VBO, EBO y VAO para su representación
	unsigned int VBO = 0, EBO = 0, VAO = 0;


	/* Métodos */

	// Para generar la esfera a partir de sus propiedades
	void construirEsfera ();

	// Generación de los vértices
	void generarVertices ();

	// Generación de los índices
	void generarIndices ();

	// Juntado de vértices y sus propiedades en un único vector para el dibujado mediante VBO
	void juntarVerticesPropiedades ();

	// Generación del VAO para representar la esfera
	void generarVAO ();
};

#endif
