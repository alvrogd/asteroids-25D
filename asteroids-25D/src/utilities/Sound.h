#ifndef SOUND_H
#define SOUND_H


// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Sound library
#include <irrKlang.h>

#include <vector>


class Sound
{
public:
	
	/* Getters */

	irrklang::ISoundEngine *getSound2D ()
	{
		return this->sound2D;
	}

	irrklang::ISoundEngine *getSound3D ()
	{
		return this->sound3D;
	}

	std::vector<irrklang::ISoundEngine *> getSound3DSet ()
	{
		return this->sound3DSet;
	}


	/* Destructor */

	~Sound ();


	/* Methods */

	static Sound* getSound ();

	void update (glm::vec3 listenerPosition, glm::vec3 listenerDirection);

	void playExplosion (glm::vec3 position);


private:

	/* Attributes */

	// Singleton instance
	static Sound *sound;

	// Sound players
	irrklang::ISoundEngine *sound2D;
	irrklang::ISoundEngine *sound3D;

	std::vector<irrklang::ISoundEngine *> sound3DSet;


	/* Constructor */

	Sound ();
};

#endif
