#ifndef SONIDO_H
#define SONIDO_H

// Sonido
#include <irrKlang.h>


class Sonido
{
public:
	
	/* Getters */

	irrklang::ISoundEngine *getSonido2D ()
	{
		return this->sonido2D;
	}

	irrklang::ISoundEngine *getSonido3D ()
	{
		return this->sonido3D;
	}


	/* Destructor */

	~Sonido ();


	/* Métodos */

	static Sonido* getSonido ();


private:

	/* Atributos */

	// Instancia singleton
	static Sonido *sonido;

	// Reproductores
	irrklang::ISoundEngine *sonido2D;
	irrklang::ISoundEngine *sonido3D;


	/* Constructor */

	Sonido ();
};

#endif
