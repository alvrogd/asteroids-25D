#ifndef PARTICLE_H
#define PARTICLE_H

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// So that the particle is able to apply its transformation during rendering
#include "../../utilities/Shader.h"

#include <vector>


class Particle
{
public:

	/* Attributes */

	static std::vector<Particle *> *particleSet;


	/* Constructors */

	Particle (glm::vec3 position, glm::vec3 speed, glm::vec4 color, float lifetime);

	Particle (glm::vec3 position, glm::vec3 speed, glm::vec4 color, float lifetime, bool belongsToSet);


	/* Destructor */

	~Particle ();


	/* Getters & setters */

	glm::vec3 & getPosition ()
	{
		return this->position;
	}

	void setPosition (glm::vec3 position)
	{
		this->position = position;
	}

	glm::vec3 & getSpeed ()
	{
		return this->speed;
	}

	void setSpeed (glm::vec3 speed)
	{
		this->speed = speed;
	}

	glm::vec4 & getColor ()
	{
		return this->color;
	}

	void setColor (glm::vec4 color)
	{
		this->color = color;
	}

	float getLifetime () const
	{
		return this->lifetime;
	}

	void setLifetime (float lifetime)
	{
		this->lifetime = lifetime;
	}

	float getAge () const
	{
		return this->age;
	}

	void setAge (float age)
	{
		this->age = age;
	}

	bool isBelongsToParticleSet () const
	{
		return this->belongsToParticleSet;
	}

	void setBelongsToParticleSet (bool belongsToParticleSet)
	{
		this->belongsToParticleSet = belongsToParticleSet;
	}


	/* Methods */

	void updateState (float elapsedTime);

	void updateStateSet (float elapsedTime);

	void draw (glm::mat4 parentTransformation, Shader *shader);

	void drawSet (glm::mat4 parentTransformation, Shader *shader);

	static void generateExplosion (glm::vec3 position);

	bool isDead () const;


private:

	/* Attributes */

	glm::vec3 position;
	glm::vec3 speed;

	glm::vec4 color;

	// How much time must elapse since the particle spawns in order to stop being rendered
	float lifetime;

	// How much time it has already been available to the rendering process
	float age;

	bool belongsToParticleSet;
};

#endif
