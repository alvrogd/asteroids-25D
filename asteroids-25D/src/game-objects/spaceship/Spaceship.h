#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "../Mobile.h"

#include "Shot.h"

// Sound library
#include <irrKlang.h>


class Spaceship :
	public Mobile
{
public:

	/* Constructor */

	Spaceship (glm::vec3 scaling, Model *model, float hitboxRadius, glm::vec3 position, glm::vec3 speed,
		glm::vec3 accelerationRate, glm::vec3 decelerationRate, glm::vec3 rotation, glm::vec3 rotationFix);


	/* Getters & setters */

	irrklang::ISound *getShotSound ()
	{
		return this->shotSound;
	}

	void setShotSound (irrklang::ISound *shotSound)
	{
		this->shotSound = shotSound;
	}

	bool isSpeedingUp () const
	{
		return this->speedingUp;
	}

	void setSpeedingUp(bool speedingUp)
	{
		this->speedingUp = speedingUp;
	}

	bool isDestroyed () const
	{
		return this->destroyed;
	}

	void setDestroyed (bool destroyed)
	{
		this->destroyed = destroyed;
	}


	/* Methods */

	void updateState (float elapsedTime) override;

	void shoot ();

	void explode ();


private:

	/* Attributes */

	// Sound effect played anytime the spaceship shoots
	irrklang::ISound *shotSound = NULL;

	bool speedingUp;

	bool destroyed;
};

#endif
