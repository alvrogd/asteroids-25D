#include "Sound.h"

#include <sstream>
#include <string>

#define SOUND_DISINTEGRATION_PREFIX "resources/sounds/effects/infinity_war_disintegration"
#define SOUND_DISINTEGRATION_EXTENSION ".ogg"


Sound *Sound::sound = NULL;


Sound * Sound::getSound ()
{
	// The singleton instance is initialized, if not yet
	if (Sound::sound == NULL)
	{
		Sound::sound = new Sound();
	}

	return Sound::sound;
}

void Sound::update (glm::vec3 listenerPosition, glm::vec3 listenerDirection)
{
	Sound::getSound ()->getSound3D ()->setListenerPosition (irrklang::vec3df (listenerPosition.x, listenerPosition.y,
		listenerPosition.z), irrklang::vec3df (listenerDirection.x, listenerDirection.y, listenerDirection.z));
}

void Sound::playExplosion (glm::vec3 position)
{
	// An explosion effect plays:
	//  - A random one is chosen between the 4 that are available
	//	- Given position
	//	- No looping
	//	- Stats playing inmediately
	//	- No tracking so that its resources are automatically freed
	//	- No sound effects
	std::ostringstream file;
	file << SOUND_DISINTEGRATION_PREFIX;
	file << std::rand () % 4;
	file << SOUND_DISINTEGRATION_EXTENSION;

	this->sound3D->play3D (file.str().c_str(), irrklang::vec3df (position.x, position.y, position.z),
		GL_FALSE, GL_FALSE, GL_TRUE);
}

Sound::Sound()
{
	// 2D player is needed for the background music
	// 3D player is needed for the sound effects
	this->sound2D = irrklang::createIrrKlangDevice ();
	this->sound3D = irrklang::createIrrKlangDevice ();

	// The 2D sounds' default volume is reduced from 1.0f
	this->sound2D->setSoundVolume (0.3f);

	// 400.0f will be the default distance at which all 3D sounds will stop getting louder
	this->sound3D->setDefault3DSoundMinDistance (400.0f);
	// And they will play up to the maximum allowed vol
	this->sound3D->setSoundVolume (1.0f);
}


Sound::~Sound()
{
	this->sound2D->drop ();
	this->sound3D->drop ();
}
