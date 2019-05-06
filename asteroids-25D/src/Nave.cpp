#include "Nave.h"

#include <iostream>

Nave::Nave (glm::vec3 escalado, Modelo * modelo, float radioHitbox, glm::vec3 posicion, glm::vec3 velocidad, glm::vec3 coefAceleracion, glm::vec3 coefDeceleracion, glm::vec3 rotacion, glm::vec3 correcionRotacion)
	: Movil (escalado, modelo, radioHitbox, posicion, velocidad, coefAceleracion, coefDeceleracion, rotacion, correcionRotacion)
{
}

void Nave::disparar ()
{
	std::cout << "DISPAAAAAAAAAAAARO" << std::endl;

	/*new Disparo (getPosicion (), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 0.0f));*/

	// Se crea un disparo en la posición de la nave y que avance en la dirección a la que apunta la nave
	new Disparo (getPosicion (),
		glm::vec3 (
			-sinf (glm::radians (getRotacion ().y)) * 50.0f,
			0.0f,
			-cos (glm::radians (getRotacion ().y)) * 50.0f
		),
		getRotacion ());
}
