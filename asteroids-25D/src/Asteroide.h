#ifndef ASTEROIDE_H
#define ASTEROIDE_H

#include "Movil.h"

// Para conjuntos de datos
#include <vector>

#include <iostream>


class Asteroide :
	public Movil
{
public:

	/* Atributos */

	static Modelo *modelo;

	static std::vector<Asteroide *> * conjuntoAsteroides;


	/* Constructor */

	Asteroide ();

	Asteroide (glm::vec3 escalado);


	/* Métodos */

	void explotar ();
};

#endif
