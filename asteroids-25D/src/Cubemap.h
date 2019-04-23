#ifndef CUBEMAP_H
#define CUBEMAP_H

// Librer�as de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para representarse
#include "Shader.h"

// Para cargar im�genes
#include "LectorImagen.h"

#include <vector>


class Cubemap
{
public:
	/* Constructor */

	Cubemap (std::vector<std::string> caras);


	/* Getters y setters */

	unsigned int getID () const
	{
		return this->id;
	}


	/* M�todos */

	void dibujar (Shader *shader) const;

private:
	/* Atributos */

	unsigned int id;

	// VBO, EBO y VAO para su representaci�n
	unsigned int VBO = 0, VAO = 0;
};

#endif
