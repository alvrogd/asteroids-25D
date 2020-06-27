#ifndef PARTICLE_SET_H
#define PARTICLE_SET_H


// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Particle.h"

#include <vector>


class ParticleSet
{
public:

	/* Attributes */

	static std::vector<ParticleSet *> *particleSetContainer;


	/* Constructor */

	ParticleSet (glm::vec3 position, glm::vec4 color);


	/* Destructor */

	~ParticleSet ();


	/* Getters & setters */

	std::vector<Particle *> getParticles ()
	{
		return this->particles;
	}

	void setParticles (std::vector<Particle *> particles)
	{
		this->particles = particles;
	}

	glm::vec3 & getPosition ()
	{
		return this->position;
	}

	void setPosition (glm::vec3 position)
	{
		this->position = position;
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


	/* Methods */

	void updateState (float elapsedTime);

	void draw (glm::mat4 parentTransformation, Shader *shader);

	void generateExplosion (float lifetime);

	bool isDead () const;


private:

	/* Attributes */

	std::vector<Particle *> particles;

	glm::vec3 position;

	glm::vec4 color;

	// How much time must elapse since the particles spawns in order to stop being rendered
	float lifetime;
	float lifetimeInverse;

	// How much time it has already been available to the rendering process
	float age;
};

#endif
