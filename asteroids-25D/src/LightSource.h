#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// So that a light source is able to load its properties in a fragment shader
#include "Shader.h"


class LightSource
{
public:

	/* Constructor */

	LightSource (glm::vec3 position, float constant, float lineal, float quadratic, glm::vec3 ambient, glm::vec3
		diffuse, glm::vec3 specular);


	/* Getters & setters */

	glm::vec3 & getPosition ()
	{
		return this->position;
	}

	void setPosition (glm::vec3 position)
	{
		this->position = position;
	}

	glm::vec3 & getAmbient ()
	{
		return this->ambient;
	}

	void setAmbient (glm::vec3 ambient)
	{
		this->ambient = ambient;
	}

	glm::vec3 & getDiffuse ()
	{
		return this->diffuse;
	}

	void setDiffuse (glm::vec3 diffuse)
	{
		this->diffuse = diffuse;
	}

	glm::vec3 & getSpecular ()
	{
		return this->specular;
	}

	void setSpecular (glm::vec3 specular)
	{
		this->specular = specular;
	}


	/* Methods */

	void load (Shader *shader, glm::mat4 transformation, int lightNumber) const;


private:

	/* Attributes */

	glm::vec3 position;

	// Attenuation (depends on distance between the camera and the light source)
	float constant;
	float lineal;
	float quadratic;

	// RGB components for each one of the light source's components
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

#endif
