#include "Particle.h"

// They are rendered as spheres
#include "Shape.h"

#include <cmath>


const double PI = 3.1415926535897;


std::vector<Particle *> *Particle::particleSet = NULL;


Particle::Particle (glm::vec3 position, glm::vec3 speed, glm::vec4 color, float lifetime)
{
	this->position = position;
	this->speed = speed;
	this->color = color;
	this->lifetime = lifetime;
	// It has just been generated
	this->age = 0.0f;

	// As it has not been pointed out, it is assumed that the particle does belong to no set
	this->belongsToParticleSet = false;
}

Particle::Particle (glm::vec3 position, glm::vec3 speed, glm::vec4 color, float lifetime, bool belongsToParticleSet)
{
	this->position = position;
	this->speed = speed;
	this->color = color;
	this->lifetime = lifetime;
	this->belongsToParticleSet = belongsToParticleSet;
	// It has just been generated
	this->age = 0.0f;
}

Particle::~Particle ()
{
	// The particle searches for itself in the scene's set of currently shown particles
	// It will only be present if it does not belong to a particle set
	if (!this->belongsToParticleSet)
	{
		for (int i = 0; i < Particle::particleSet->size (); i++)
		{
			if (Particle::particleSet->at (i) == this)
			{
				Particle::particleSet->erase (Particle::particleSet->begin () + i);
			}
		}
	}
}

void Particle::updateState (float elapsedTime)
{
	// The particle's new position depends on its speed
	this->position.x += this->speed.x;
	this->position.y += this->speed.y;
	this->position.z += this->speed.z;

	this->age += elapsedTime;
}

void Particle::updateStateSet (float elapsedTime)
{
	// The particle's new position depends on its speed
	this->position.x += this->speed.x;
	this->position.y += this->speed.y;
	this->position.z += this->speed.z;

	// The particle does not need to know its individual age as it belongs to a set
}

void Particle::draw (glm::mat4 parentTransformation, Shader * shader)
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
	float currentPercentage = 1 - remainingPercentage;

	color.r += currentPercentage;
	color.g += currentPercentage;
	color.b += currentPercentage;
	color.a *= remainingPercentage;

	shader->setVec4 ("Color", color);

	glm::mat4 transformation = parentTransformation;

	// 2º
	transformation = glm::translate (transformation, getPosition ());

	// 1º (standard scaling for all particles)
	transformation = glm::scale (transformation, glm::vec3(1.0f, 1.0f, 1.0f));

	shader->setMat4 ("modelMatrix", transformation);

	// The particle is rendered as a sphere
	Shape::drawSphere ();

	// Transparency should be turned off before exiting the function
	glDisable (GL_BLEND);
}

void Particle::drawSet (glm::mat4 parentTransformation, Shader * shader)
{
	// As the particle belongs to a set, the latter will already have turned on both transparency and the shader,
	// as well as established the particle's color in the shader
	
	glm::mat4 transformation = parentTransformation;

	// 2º
	transformation = glm::translate (transformation, getPosition ());

	// 1º (standard scaling for all particles)
	transformation = glm::scale (transformation, glm::vec3 (1.0f, 1.0f, 1.0f));

	shader->setMat4 ("modelMatrix", transformation);

	// The particle is rendered as a sphere
	Shape::drawSphere();
}

void Particle::generateExplosion (glm::vec3 position)
{
	// 100 random particles are generated, taken the given position as their source
	for (int i = 0; i < 100; i++)
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

		// The particle is put into the scene:
		//  - Explosion's origin
		//	- Computed speed
		//  - Color
		//	- Lifetime in seconds
		Particle::particleSet->push_back (new Particle (position, speed,
			glm::vec4 (1.0f, 0.0f, 1.0f, 1.0f), 3.0f));
	}
}

bool Particle::isDead () const
{
	return(this->age >= this->lifetime);
}
