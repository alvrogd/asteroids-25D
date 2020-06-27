#ifndef OBJECT_H
#define OBJECT_H

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// An object is represented via a model
#include "Model.h"

// So that the object is able to apply its transformation during rendering
#include "Shader.h"


class Object
{
public:

	/* Constructor */

	Object (glm::vec3 scaling, Model *model);


	/* Getters & setters */

	glm::vec3 & getScaling ()
	{
		return this->scaling;
	}

	void setScaling (glm::vec3 scaling)
	{
		this->scaling = scaling;
	}

	Model * getModel ()
	{
		return this->model;
	}

	void setModel (Model *model)
	{
		this->model = model;
	}


	/* Methods */

	virtual void draw (glm::mat4 parentTransformation, Shader *shader);


private:

	/* Attributes */

	// Scaling factor for each coordinate (x, y, z)
	glm::vec3 scaling;

	Model *model;
};

#endif