#include "Sonido.h"

Sonido *Sonido::sonido = NULL;


Sonido * Sonido::getSonido ()
{
	// Si la instancia singleton no se ha inicializado, se crea
	if (Sonido::sonido == NULL)
	{
		Sonido::sonido = new Sonido ();
	}

	return Sonido::sonido;
}

Sonido::Sonido ()
{
	// Se crean los reproductores de sonido necesarios
	Sonido::sonido2D = irrklang::createIrrKlangDevice ();
	Sonido::sonido3D = irrklang::createIrrKlangDevice ();
}

Sonido::~Sonido ()
{
	// Se destruyen los reproductores de sonido creados
	Sonido::sonido2D->drop ();
	Sonido::sonido3D->drop ();
}
