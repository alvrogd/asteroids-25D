#include "Forma.h"

Esfera * Forma::esfera = NULL;
Sphere * Forma::sphere = NULL;

void Forma::inicializarFormas ()
{
	// Se crea una esfera
	Forma::esfera = new Esfera (1.0f, 50, 50);
	// todo quitarla
	Forma::sphere = new Sphere (40, 40);
}

void Forma::destruirFormas ()
{
	// Se destruyen los objetos creados
	delete Forma::esfera;
}

void Forma::dibujarEsfera ()
{
	Forma::esfera->dibujar ();
	//Mesh::sphere->draw ();
}
