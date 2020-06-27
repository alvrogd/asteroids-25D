#include "Spaceship.h"

#include <iostream>

// To shoot
#include "Sound.h"
#include "Particle.h"
#include "ParticleSet.h"


Spaceship::Spaceship (glm::vec3 scaling, Model * model, float hitboxRadius, glm::vec3 position, glm::vec3 speed,
	glm::vec3 accelerationRate, glm::vec3 decelerationRate, glm::vec3 rotation, glm::vec3 rotationFix)
	: Mobile (scaling, model, hitboxRadius, position, speed, accelerationRate, decelerationRate, rotation,
		rotationFix)
{
	// Initially, the spaceship is still
	this->speedingUp = false;
	this->destroyed = false;
}

void Spaceship::updateState (float elapsedTime)
{
	glm::vec3 oldPosition = this->getPosition ();

	Mobile::updateState (elapsedTime);

	// If the spaceship is speeding up and has moved at least 1 unit, a "combustion" visual effect is shown behind
	if (this->speedingUp && glm::distance (oldPosition, this->getPosition ()) >= 1.0f)
	{
		// Must be behind the spaceship
		glm::vec3 effectPosition = this->getPosition ();

		effectPosition.x += sinf (glm::radians (this->getRotation ().y)) * 20.0f;
		effectPosition.z += cosf (glm::radians (this->getRotation ().y)) * 20.0f;

		// The combustion effect is made up of multiple particles (spheres)
		for (int i = 0; i < 8; i++)
		{
			// - Position: behind the spaceship + a random displacement
			// - Direction: the particle follows the spaceship's direction, but much slower
			// - Color: orange
			// - Lifetime: 0.25 seconds

			// [-3.0, 3.0)
			glm::vec3 alteracion;
			alteracion.x = fmod (std::rand (), 6.0f) - 3.0f;
			alteracion.y = fmod (std::rand (), 6.0f) - 3.0f;
			alteracion.z = fmod (std::rand (), 6.0f) - 3.0f;

			Particle::particleSet->push_back (new Particle (effectPosition + alteracion, 0.1f *
				this->getSpeed (), glm::vec4 (1.0f, 0.38f, 0.28f, 1.0f), 0.25f));
		}
	}
}

void Spaceship::shoot ()
{
	// A shot is spawned right where the spaceship is, and moves to where the latter points
	new Shot (getPosition (),
		glm::vec3 (
			-sinf (glm::radians (getRotation ().y)) * 35.0f,
			0.0f,
			-cos (glm::radians (getRotation ().y)) * 35.0f
		),
		getRotation ());

	// At most, one shot sound effect will play at any given time
	if (this->shotSound != NULL)
	{
		this->shotSound->stop ();
		this->shotSound->drop ();
	}

	// The sound effect is played:
	//  - No repetition
	//	- Play instantly
	//	- A reference to the "track" is needed in order to stop it (see previous "if" block)
	this->shotSound = Sound::getSound()->getSound2D()->play2D("laser_rapid.ogg", GL_FALSE, GL_FALSE, GL_TRUE);
	this->shotSound->setVolume (0.50f);
}

void Spaceship::explode ()
{
	this->destroyed = true;

	// The ship cannot longer be moving
	this->setSpeed (glm::vec3 (0.0f, 0.0f, 0.0f));

	// An explosion effect will be rendered right where the spaceship is
	// A ParticleSet is used, instead of multiple Particles, in oder to achieve better performance
	// - Purple color
	ParticleSet *particleSet = new ParticleSet (this->getPosition (),
		glm::vec4 (1.0f, 0.0f, 1.0f, 1.0f));
	ParticleSet::particleSetContainer->push_back (particleSet);
	particleSet->generateExplosion (3.0f);

	// No additional sound effect is played as an asteroid must have exploded, therefore playing its corresponding
	// sound effect
}
