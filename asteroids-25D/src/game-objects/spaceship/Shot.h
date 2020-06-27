#ifndef SHOT_H
#define SHOT_H

#include "../Mobile.h"

// Shots will be rendered as spheres
#include "../visual-effects/Shape.h"

// And they will also emit light
#include "../visual-effects/LightSource.h"

#include <vector>


class Shot :
	public Mobile
{
public:

	/* Attributes */

	static Model *model;

	static std::vector<Shot *> * shotsSet;


	/* Constructor */

	Shot (glm::vec3 position, glm::vec3 speed, glm::vec3 rotation);


	/* Destructor */

	~Shot ();


	/* Getters & setters */

	int getRemainingWarps () const
	{
		return this->remainingWarps;
	}

	void setRemainingWarps (int remainingWarps)
	{
		this->remainingWarps = remainingWarps;
	}

	LightSource *getLight ()
	{
		return this->light;
	}

	void setLight (LightSource *light)
	{
		this->light = light;
	}


	/* Methods */

	void updateState (float elapsedTime) override;

	void draw (glm::mat4 parentTransformation, Shader *shader) override;


private:

	/* Attributes */

	// How many warps (teleports to the opposite side when reaching the edge of the scene) the shot can still perform
	// before disappearing
	int remainingWarps;

	// Emitted light
	LightSource *light;
};

#endif
