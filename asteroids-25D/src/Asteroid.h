#ifndef ASTEROID_H
#define ASTEROID_H

#include "Mobile.h"

#include <vector>


class Asteroid :
	public Mobile
{
public:

	/* Attributes */

	static Model *model;

	static std::vector<Asteroid *> * asteroidSet;

	static const int MAX_LIVES = 2;


	/* Constructors */

	Asteroid ();

	Asteroid (glm::vec3 scaling, glm::vec3 position, int lives);


	/* Destructor */

	~Asteroid ();


	/* Getters & setters */

	int getLives () const
	{
		return this->lives;
	}

	void setLives (int lives)
	{
		this->lives = lives;
	}


	/* Methods */

	void explode ();


private:

	/* Attributes */

	// How many times the asteroid will generate smaller asteroids upon exploding
	int lives;
};

#endif
