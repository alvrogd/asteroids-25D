#ifndef MOBILE_H
#define MOBILE_H

#include "Object.h"


class Mobile :
	public Object
{
public:

	/* Attributes */

	static glm::vec3 warpCoordinates;


	/* Constructor */
	
	Mobile (glm::vec3 scaling, Model *model, float hitboxRadius, glm::vec3 position, glm::vec3 speed, glm::vec3
		accelerationRate, glm::vec3 decelerationRate, glm::vec3 rotation, glm::vec3 rotationFix);


	/* Getters & setters */

	float getHitboxRadius () const
	{
		return this->hitboxRadius;
	}

	void setHitboxRadius (float hitboxRadius)
	{
		this->hitboxRadius = hitboxRadius;
	}

	glm::vec3 & getPosition ()
	{
		return this->position;
	}

	glm::vec3 * getPositionReference ()
	{
		return &(this->position);
	}

	void setPosition (glm::vec3 position)
	{
		this->position = position;
	}

	glm::vec3 & getSpeed ()
	{
		return this->speed;
	}

	glm::vec3 * getSpeedReference ()
	{
		return &(this->speed);
	}

	void setSpeed (glm::vec3 speed)
	{
		this->speed = speed;
	}

	glm::vec3 & getAccelerationRate ()
	{
		return this->accelerationRate;
	}

	glm::vec3 * getAccelerationRateReference ()
	{
		return &(this->accelerationRate);
	}

	void setAccelerationRate (glm::vec3 accelerationRate)
	{
		this->accelerationRate = accelerationRate;
	}

	glm::vec3 & getDecelerationRate ()
	{
		return this->decelerationRate;
	}

	void setDecelerationRate (glm::vec3 decelerationRate)
	{
		this->decelerationRate = decelerationRate;
	}

	glm::vec3 & getRotation ()
	{
		return this->rotation;
	}

	glm::vec3 * getRotationReference ()
	{
		return &(this->rotation);
	}

	void setRotation (glm::vec3 rotation)
	{
		this->rotation = rotation;
	}

	glm::vec3 & getRotationFix ()
	{
		return this->rotationFix;
	}

	glm::vec3 * getRotationFixReference ()
	{
		return &(this->rotationFix);
	}

	void setRotationFix (glm::vec3 rotationFix)
	{
		this->rotationFix = rotationFix;
	}


	/* Methods */

	virtual void updateState (float elapsedTime);

	void draw (glm::mat4 parentTransformation, Shader *shader) override;

	bool checkCollision (Mobile *mobile) const;


private:

	/* Attributes */

	// The hitbox is a sphere
	float hitboxRadius;

	// Movement properties are specified for each coordinate (x, y, z)
	glm::vec3 position;

	glm::vec3 speed;

	glm::vec3 accelerationRate;

	glm::vec3 decelerationRate;

	// For each axis
	glm::vec3 rotation;

	// Direction in which the mobile object looks
	glm::vec3 direction;

	// The model that represents the mobile may need it
	glm::vec3 rotationFix;
};

#endif
