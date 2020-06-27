#include "Shot.h"


std::vector<Shot *> *Shot::shotsSet = NULL;


Shot::Shot (glm::vec3 position, glm::vec3 speed, glm::vec3 rotation)
	: Mobile(
		// The sphere that will represent the shot is stretched along the X axis
		glm::vec3(1.0f, 1.0f, 5.0f),
		// Therefore no model is needed to render it
		NULL,
		// Hitbox radius equals to the value of the coordinate with the maximum scaling
		5.0f,
		// Given data
		position,
		speed,
		// No acceleration nor deceleration
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 (0.0f, 0.0f, 0.0f),
		// Given rotation
		rotation,
		// No rotation fix as no model is used
		glm::vec3 (0.0f, 0.0f, 0.0f))
{
	// The shot is added to the scene's currently present shots
	Shot::shotsSet->push_back (this);

	// The shot will emit a pink light
	this->light = new LightSource (getPosition (), 1.0f, 0.027f, 0.0028f, glm::vec3 (1.0f, 0.0f, 1.0f),
		glm::vec3 (1.0f, 0.0f, 1.0f), glm::vec3 (1.0f, 0.0f, 1.0f));
}

Shot::~Shot ()
{
	// The shot searches for itself in the scene's set of currently shown shots
	for (size_t i = 0; i < Shot::shotsSet->size (); i++)
	{
		if (Shot::shotsSet->at (i) == this)
		{
			Shot::shotsSet->erase (Shot::shotsSet->begin () + i);
		}
	}

	delete this->light;
}

void Shot::updateState (float elapsedTime)
{
	glm::vec3 & position = getPosition ();
	glm::vec3 & speed = getSpeed ();

	// Initially, it is assumed that the shot will not perform a warp
	bool isWarp = false;

	// The shot's new position depends on its speed
	position.x += speed.x;
	position.z += speed.z;

	// If any of the coordinates has surpassed the established warp, they are inverted in order to "teleport" the
	// mobile to the opposite side
	if (fabs (position.x) > Mobile::warpCoordinates.x)
	{
		position.x *= -1;
		isWarp = true;
	}

	if (fabs (position.z) > Mobile::warpCoordinates.z)
	{
		position.z *= -1;
		isWarp = true;
	}

	// The speed is also modified depending on the deceleration rates
	speed.x *= 1 - getDecelerationRate ().x;
	speed.z *= 1 - getDecelerationRate ().z;

	// If the shot has been warped, its remaning warp count must be manually updated
	if (isWarp)
	{
		this->remainingWarps--;
	}

	// The position of the shot's emitted light also needs to be updated
	this->light->setPosition (position);
}

void Shot::draw (glm::mat4 parentTransformation, Shader * shader)
{
	shader->use ();

	// Purple color
	shader->setVec3 ("Color", glm::vec3 (1.0f, 0.0f, 1.0f));

	glm::mat4 transformation = parentTransformation;

	// 3º
	transformation = glm::translate (transformation, getPosition ());

	// 2º
	glm::vec3 rot = getRotation ();
	glm::vec3 fix = getRotationFix ();
	transformation = glm::rotate (transformation, glm::radians (rot.x + fix.x), glm::vec3 (1.0f, 0.0f, 0.0f));
	transformation = glm::rotate (transformation, glm::radians (rot.y + fix.y), glm::vec3 (0.0f, 1.0f, 0.0f));
	transformation = glm::rotate (transformation, glm::radians (rot.z + fix.z), glm::vec3 (0.0f, 0.0f, 1.0f));

	// 1º
	transformation = glm::scale (transformation, getScaling ());

	shader->setMat4 ("modelMatrix", transformation);

	// The shot will be rendered using a sphere
	Shape::drawSphere();
}
