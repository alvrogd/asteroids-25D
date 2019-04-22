#ifndef OBJETO_H
#define OBJETO_H

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para ser representado por un modelo
#include "Modelo.h"

// Para poder aplicar sus transformaciones en el renderizado
#include "Shader.h"


class Objeto
{
public:
	/* Constructor */

	Objeto (glm::vec3 escalado, Modelo *modelo);


	/* Getters y setters */

	glm::vec3 & getEscalado ()
	{
		return this->escalado;
	}

	void setEscalado (glm::vec3 escalado)
	{
		this->escalado = escalado;
	}

	Modelo * getModelo ()
	{
		return this->modelo;
	}

	void setModelo (Modelo *modelo)
	{
		this->modelo = modelo;
	}


	/* Métodos */

	virtual void dibujar (glm::mat4 transformacionPadre, Shader *shader) const;


private:
	/* Atributos */

	// Factor de escalado en cada coordenada
	glm::vec3 escalado;

	// Modelo que representa al objeto
	Modelo *modelo;

};

#endif