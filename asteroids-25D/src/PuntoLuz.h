#ifndef PUNTO_LUZ_H
#define PUNTO_LUZ_H

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para cargar sus propiedades en un fragment shader
#include "Shader.h"


class PuntoLuz
{
public:
	/* Constructor */

	PuntoLuz (glm::vec3 posicion, float constante, float lineal, float cuadratica, glm::vec3 ambiental, glm::vec3 difusa,
		glm::vec3 especular);


	/* Getters y setters */

	glm::vec3 & getAmbiental ()
	{
		return this->ambiental;
	}

	void setAmbiental (glm::vec3 ambiental)
	{
		this->ambiental = ambiental;
	}

	glm::vec3 & getDifusa ()
	{
		return this->difusa;
	}

	void setDifusa (glm::vec3 difusa)
	{
		this->difusa = difusa;
	}

	glm::vec3 & getEspecular ()
	{
		return this->especular;
	}

	void setEspecular (glm::vec3 especular)
	{
		this->especular = especular;
	}


	/* Métodos */

	void cargar (Shader *shader, glm::mat4 transformacion) const;


private:
	// Posición del punto de luz
	glm::vec3 posicion;

	// Atenuación de la luz en función de la distancia
	float constante;
	float lineal;
	float cuadratica;

	// Componentes RGB para cada una de las componentes del punto de luz
	glm::vec3 ambiental;
	glm::vec3 difusa;
	glm::vec3 especular;
};

#endif
