#ifndef ASTEROIDE_H
#define ASTEROIDE_H

#include "Movil.h"

class Asteroide :
	public Movil
{
public:

	/* Atributos */

	static Modelo *modelo;


	/* Constructor */

	Asteroide ();
};

#endif
