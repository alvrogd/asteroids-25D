#include "Asteroid.h"

// To generate explosions
#include "../visual-effects/ParticleSet.h"
#include "../visual-effects/Particle.h"
#include "../../utilities/Sound.h"


Model *Asteroid::model = NULL;
std::vector<Asteroid*> * Asteroid::asteroidSet = NULL;


Asteroid::Asteroid ()
	// Random size scaling between x0.5 and x0.8
	: Mobile (glm::vec3 (fmod (std::rand (), 3.0f) * 0.1f + 0.5f, fmod (std::rand (), 3.0f) * 0.3f + 0.5f, fmod (
		std::rand (), 3.0f) * 0.1f + 0.5f),
		// Global model that has been set for asteroids
		Asteroid::model,
		// Hitbox radius
		0.0f,
		// Random position inside the warping range set for mobile objects
		glm::vec3 (fmod (std::rand (), (Mobile::warpCoordinates.x * 2.0f)) - Mobile::warpCoordinates.x, 0.0f,
			fmod (std::rand (), (Mobile::warpCoordinates.z * 2.0f)) - Mobile::warpCoordinates.z),
		// Random speed up to 1 unit per frame
		glm::vec3 (fmod (std::rand (), 10.0f) * 0.1f, fmod (std::rand (), 10.0f) * 0.1f, fmod (std::rand (), 10.0f) *
			0.1f),
		// Null acceleration and deceleration
		glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3 (0.0f, 0.0f, 0.0f),
		// Random rotation up to 360º
		glm::vec3 (fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f)),
		// The model needs no rotation fix
		glm::vec3 (0.0f, 0.0f, 0.0f))
{
	// The hitbox radius is replaced with the maximum of all the components in the size scaling
	glm::vec3 scaling = getScaling ();
	float max = scaling.x;

	if (scaling.y > max)
	{
		max = scaling.y;
	}

	if (scaling.z > max)
	{
		max = scaling.z;
	}

	// 86.0f measured in Blender
	setHitboxRadius (max * 43.0f);

	// The asteroid will explode into smaller asteroids up to two times
	this->lives = Asteroid::MAX_LIVES;
}

Asteroid::Asteroid (glm::vec3 scaling, glm::vec3 position, int lives)
	// Given size scaling
	: Mobile (scaling,
		// Global model that has been set for asteroids
		Asteroid::model,
		// Hitbox radius
		0.0f,
		// Given position
		position,
		// Random speed up to 1 unit per frame
		// It is inversely proportional to the given life count, also taking into account an asteroid's maximum life
		// count, and being capped at x2
		glm::vec3 (fmod (std::rand (), 10.0f) * 0.1f,
			fmod (std::rand (), 10.0f) * 0.1f,
			fmod (std::rand (), 10.0f) * 0.1f)
			* (1.0f + (float)(Asteroid::MAX_LIVES - lives) / (float)Asteroid::MAX_LIVES),
		// Null acceleration and deceleration
		glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3 (0.0f, 0.0f, 0.0f),
		// Random rotation up to 360º
		glm::vec3 (fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f)),
		// The model needs no rotation fix
		glm::vec3 (0.0f, 0.0f, 0.0f))
{
	// The hitbox radius is replaced with the maximum of all the components in the size scaling
	float max = scaling.x;

	if (scaling.y > max)
	{
		max = scaling.y;
	}

	if (scaling.z > max)
	{
		max = scaling.z;
	}

	// 86.0f measured in Blender
	setHitboxRadius (max * 43.0f);

	// As many as specified
	this->lives = lives;
}

Asteroid::~Asteroid ()
{
	// The asteroids searches for itself in the set of currently existing asteroids in the scene
	for (size_t i = 0; i < Asteroid::asteroidSet->size (); i++)
	{
		if (Asteroid::asteroidSet->at (i) == this)
		{
			// It erases itself
			Asteroid::asteroidSet->erase (Asteroid::asteroidSet->begin () + i);
		}
	}
}

void Asteroid::explode ()
{
	// If the asteroid can still be divided into smaller ones
	if (this->lives > 0)
	{
		// It generates two "children" asteroids, that will be spawned right where the parent is
		Asteroid::asteroidSet->push_back (new Asteroid (getScaling () * 0.6f, getPosition (),
			this->lives - 1));
		Asteroid::asteroidSet->push_back (new Asteroid (getScaling () * 0.6f, getPosition (),
			this->lives - 1));
	}

	// An explosion is generated where the asteroid is right now, using a particle set
	ParticleSet *particleSet = new ParticleSet (this->getPosition (),
		glm::vec4 (0.933f, 0.51f, 1.0f, 0.933f));
	ParticleSet::particleSetContainer->push_back (particleSet);
	particleSet->generateExplosion (3.0f);

	// The explosion sound effect is played
	Sound::getSound()->playExplosion (this->getPosition ());

	// The asteroid object is removed
	delete this;
}
