#include "Forma.h"

Esfera * Forma::esfera = NULL;

void Forma::inicializarFormas ()
{
	// Se crea una esfera
	Forma::esfera = new Esfera (1.0f, 6, 6);
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
