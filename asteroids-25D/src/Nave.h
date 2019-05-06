#ifndef NAVE_H
#define NAVE_H

// Clase a heredar
#include "Movil.h"

// Para disparar
#include "Disparo.h"


class Nave :
	public Movil
{
public:

	/* Constructor */

	Nave (glm::vec3 escalado, Modelo *modelo, float radioHitbox, glm::vec3 posicion, glm::vec3 velocidad, glm::vec3
		coefAceleracion, glm::vec3 coefDeceleracion, glm::vec3 rotacion, glm::vec3 correcionRotacion);


	/* Métodos */

	void disparar ();
};

#endif
