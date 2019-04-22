#ifndef RASTRO_H
#define RASTRO_H

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para almacenar las posiciones que componen un rastro
#include <vector>

// Para cargar al program shader la model matrix
#include "Shader.h"


class Rastro
{
public:
	/* Constructor */

	Rastro (float r, float g, float b);


	/* Destructor */

	~Rastro ();


	/* Atributos */

	// Máximo de posiciones que puede almacenar cada rastro
	static int maximoPosiciones;


	/* Getters y setters */

	std::vector<float> & getCoordenadasPosiciones ()
	{
		return coordenadasPosiciones;
	}

	void setCoordenadasPosiciones (std::vector<float> coordenadasPosiciones)
	{
		this->coordenadasPosiciones = coordenadasPosiciones;
	}

	float getR () const
	{
		return this->r;
	}

	void setR (float r)
	{
		this->r = r;
	}

	float getG () const
	{
		return this->g;
	}

	void setG (float g)
	{
		this->g = g;
	}

	float getB () const
	{
		return this->b;
	}

	void setB (float b)
	{
		this->b = b;
	}

	unsigned int getVBO () const
	{
		return VBO;
	}

	void setVBO (unsigned int VBO)
	{
		this->VBO = VBO;
	}

	unsigned int getVAO () const
	{
		return VAO;
	}

	void setVAO (unsigned int VAO)
	{
		this->VAO = VAO;
	}


	/* Métodos */

	// Se añade una nueva posición con la que representar el rastro
	void insertarPosicion (float x, float y, float z, float r, float g, float b);

	void dibujar (glm::mat4 transformacionPadre, Shader *shaderColor) const;


private:
	/* Atributos */

	// Coordenadas y color de las posiciones en el espacio que componen el rastro
	std::vector<float> coordenadasPosiciones;

	// Color con el que representar el rastro
	float r, g, b;

	// VBO, EBO y VAO para su representación
	unsigned int VBO = 0, EBO = 0, VAO = 0;

	// Generación del VAO para representar el rastro
	void generarVAO ();
};

#endif
