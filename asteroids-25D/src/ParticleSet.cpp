#include "ParticleSet.h"


std::vector<ParticleSet *> *ParticleSet::particleSetContainer = NULL;


const double PI = 3.1415926535897;


ParticleSet::ParticleSet (glm::vec3 position, glm::vec4 color)
{
	this->position = position;
	this->color = color;
}

ParticleSet::~ParticleSet ()
{
	// All of its particles are deleted
	for (Particle *particle : this->particles)
	{
		delete particle;
	}

	// The particle set searches for itself in the scene's set of currently shown sets of particles
	for (int i = 0; i < ParticleSet::particleSetContainer->size (); i++)
	{
		if (ParticleSet::particleSetContainer->at (i) == this)
		{
			ParticleSet::particleSetContainer->erase (
				ParticleSet::particleSetContainer->begin () + i);
		}
	}
}

void ParticleSet::updateState (float elapsedTime)
{
	// The position of all contained particles is updated
	for (Particle *particle : this->particles)
	{
		particle->updateStateSet (elapsedTime);
	}

	this->age += elapsedTime;
}

void ParticleSet::draw (glm::mat4 parentTransformation, Shader * shader)
{
	// Transparency needs to be enabled
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);

	shader->use ();

	// The particle's color is set in the shader:
	//	- The particle will vanish as it approaches its end
	//	- And the color will also gradually fade to white
	glm::vec4 color = this->color;

	float remainingPercentage = (this->lifetime - this->age) / this->lifetime;
	//float currentPercentage = 1 - remainingPercentage;

	/*color.r += currentPercentage;
	color.g += currentPercentage;
	color.b += currentPercentage;*/
	color.a *= remainingPercentage;

	shader->setVec4 ("Color", color);

	// All contained particles are rendered
	for (Particle *particle : this->particles)
	{
		particle->drawSet (parentTransformation, shader);
	}

	// Transparency should be turned off before exiting the function
	glDisable (GL_BLEND);
}

void ParticleSet::generateExplosion (float lifetime)
{
	// 60 random particles are generated, taken the set's position as their source
	for (int i = 0; i < 60; i++)
	{
		// Each particle will move in a random direction, but such as all particles take the shape of a sphere
		glm::vec3 speed;

		// A unit vector will define such direction
		//
		// For a sphere:
		//	x = r * sin(theta) * cos(phi)
		//	y = r * sin(theta) * sin(phi)
		//	z = r * cos(theta)
		
		// Each particle gets is own random phi, in the interval [0, 2pi), with precision of hundredths
		float phi = fmod ((double)std::rand (), 2.0f * PI);

		// And its own random cos(theta), in the interval [-1, 1), also with precision of hundredths
		int random = (std::rand () % 200) - 100;
		float cosTheta = (float)random / 100.f;
		float theta = acos (cosTheta);

		speed.x = sinf (theta) * cosf (phi);
		speed.y = sinf (theta) * sinf (phi);
		speed.z = cosTheta;

		// The unit vector is multiplied in order to increase the particle's speed
		speed *= 5;

		// The particle is put into the set:
		//  - Its origin is where the particle set is
		//  - Color is determined by the set
		//  - The particle belongs to a set
		this->particles.push_back (new Particle (this->position, speed, this->color, lifetime, true));
	}

	this->age = 0;
	this->lifetime = lifetime;
	this->lifetimeInverse = 1 / this->lifetime;
}

bool ParticleSet::isDead () const
{
	return(this->age >= this->lifetime);
}
