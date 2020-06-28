#ifndef CUBEMAP_H
#define CUBEMAP_H


// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// To render itsef
#include "../../utilities/Shader.h"

#include "../../utilities/ImageReader.h"

#include <vector>


class Cubemap
{
public:

	/* Constructor */

	Cubemap (std::vector<std::string> faces);


	/* Getters & setters */

	unsigned int getID () const
	{
		return this->id;
	}


	/* Methods */

	void draw (Shader *shader) const;

private:

	/* Attributes */

	// Its ID inside OpenGL
	unsigned int id;

	// VBO and VAO to draw it
	unsigned int VBO = 0, VAO = 0;
};

#endif
