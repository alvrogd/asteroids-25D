#ifndef MATERIAL_H
#define MATERIAL_H

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para cargar sus propiedades en un fragment shader
#include "Shader.h"


class Material
{
public:
	/* Constructor */
	Material (glm::vec3 ambiental, glm::vec3 difusa, glm::vec3 especular, float brillo);


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

	float getBrillo () const
	{
		return this->brillo;
	}

	void setBrillo (float brillo)
	{
		this->brillo = brillo;
	}


	/* Métodos */

	void cargar (Shader *shader) const;


private:
	/* Atributos */

	// Reflexión de los componentes RGB para cada uno de los componentes de la luz
	glm::vec3 ambiental;
	glm::vec3 difusa;
	glm::vec3 especular;

	// Brillo que determina la concentración con la luz especular
	float brillo;
};

#endif
