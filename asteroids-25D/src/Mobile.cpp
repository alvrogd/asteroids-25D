#include "Mobile.h"

#include <cmath>

// They are initialized to a default minimum value
glm::vec3 Mobile::warpCoordinates = glm::vec3 (10.0f, 10.0f, 10.0f);


Mobile::Mobile (glm::vec3 scaling, Model * model, float hitboxRadius, glm::vec3 position, glm::vec3 speed,
	glm::vec3 accelerationRate, glm::vec3 decelerationRate, glm::vec3 rotation, glm::vec3 rotationFix)
	: Object(scaling, model)
{
	this->hitboxRadius = hitboxRadius;
	this->position = position;
	this->speed = speed;
	this->accelerationRate = accelerationRate;
	this->decelerationRate = decelerationRate;
	this->rotation = rotation;
	this->rotationFix = rotationFix;
}

void Mobile::updateState (float elapsedTime)
{
	// The mobile's new position depends on its speed
	this->position.x += this->speed.x;
	this->position.z += this->speed.z;

	// If any of the coordinates has surpassed the established warp, they are inverted in order to "teleport" the
	// mobile to the opposite side
	if (fabs (this->position.x) > Mobile::warpCoordinates.x)
	{
		this->position.x *= -1;
	}

	if (fabs (this->position.z) > Mobile::warpCoordinates.z)
	{
		this->position.z *= -1;
	}

	// The speed is also modified depending on the deceleration rates
	this->speed.x *= 1 - this->decelerationRate.x;
	this->speed.z *= 1 - this->decelerationRate.z;
}

void Mobile::draw (glm::mat4 parentTransformation, Shader * shader)
{
	// For hierarchies of objects, where the transformation of a certain node also depends on its antecesors
	glm::mat4 transformation = parentTransformation;

	// 2º
	transformation = glm::translate (transformation, this->position);
	
	// 1º
	transformation = glm::rotate (transformation, glm::radians (this->rotation.x + this->rotationFix.x),
		glm::vec3 (1.0f, 0.0f, 0.0f));
	transformation = glm::rotate (transformation, glm::radians (this->rotation.y + this->rotationFix.y),
		glm::vec3 (0.0f, 1.0f, 0.0f));
	transformation = glm::rotate (transformation, glm::radians (this->rotation.z + this->rotationFix.z),
		glm::vec3 (0.0f, 0.0f, 1.0f));
	
	// The original method takes care of the last steps in the rendering 
	Object::draw (transformation, shader);
}

bool Mobile::checkCollision (Mobile * mobile) const
{
	// Distance between the center of both objects
	float distance = glm::distance(this->position, mobile->getPosition ());

	// They will collide if the distance is less than the sum of their radiuses
	return(distance < this->hitboxRadius + mobile->getHitboxRadius ());
}
