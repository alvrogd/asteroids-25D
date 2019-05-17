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

	static const int MAXIMO_VIDAS = 2;


	/* Constructores */

	Asteroide ();

	Asteroide (glm::vec3 escalado, glm::vec3 posicion, int vidas);


	/* Destructor */

	~Asteroide ();


	/* Getters y setters */

	int getVidas () const
	{
		return this->vidas;
	}

	void setVidas (int vidas)
	{
		this->vidas = vidas;
	}


	/* Métodos */

	void explotar ();


private:

	/* Atributos */

	// Número de veces que el asteroide dará lugar a dos asteroides más pequeños
	int vidas;
};

#endif
