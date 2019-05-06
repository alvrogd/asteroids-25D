#include "Nave.h"

#include <iostream>

// Para reproducir sonido
#include "Sonido.h"


Nave::Nave (glm::vec3 escalado, Modelo * modelo, float radioHitbox, glm::vec3 posicion, glm::vec3 velocidad, glm::vec3 coefAceleracion, glm::vec3 coefDeceleracion, glm::vec3 rotacion, glm::vec3 correcionRotacion)
	: Movil (escalado, modelo, radioHitbox, posicion, velocidad, coefAceleracion, coefDeceleracion, rotacion, correcionRotacion)
{
}

void Nave::disparar ()
{
	// Se crean 12 disparos en la posición de la nave y que avance en la dirección a la que apunta la nave
	//for (int i = 0; i < 10; i++)
	//{
		// Se crea un disparo en la posición de la nave y que avance en la dirección a la que apunta la nave
		new Disparo (getPosicion (),
			glm::vec3 (
				-sinf (glm::radians (getRotacion ().y)) * 35.0f,
				0.0f,
				-cos (glm::radians (getRotacion ().y)) * 35.0f
			),
			getRotacion ());
	//}

	/*// Si ya se ha producido un sonido de disparo, se elimina
	if (this->sonidoDisparo == NULL || (this->sonidoDisparo != NULL && this->sonidoDisparo->isFinished()))
	{
		if (this->sonidoDisparo != NULL)
		{
			this->sonidoDisparo->drop ();
		}
		
		// Se reproduce un efecto de disparo
		//this->sonidoDisparo = Sonido::getSonido ()->getSonido2D ()->play2D ("laser_rapid.mp3", GL_FALSE);
		this->sonidoDisparo = Sonido::getSonido ()->getSonido2D ()->play2D ("laser_rapid.mp3", GL_FALSE, GL_FALSE, GL_TRUE);
		this->sonidoDisparo->setVolume (0.5f);

		// Ya no se establece a NULL porque se va a almacenar una referencia en la variable acto seguido
	}	*/

	// Si ya se ha producido un sonido de disparo, se para y se eliminan sus recursos
	if (this->sonidoDisparo != NULL)
	{
		this->sonidoDisparo->stop ();
		this->sonidoDisparo->drop ();

		// Ya no se establece a NULL porque se va a almacenar una referencia en la variable acto seguido
	}

	// Se reproduce un efecto de disparo; sin repetición, reproducción desde el inicio, y tratamiento de "track" para
	// obtener una referencia a él
	this->sonidoDisparo = Sonido::getSonido ()->getSonido2D ()->play2D ("laser_rapid.mp3", GL_FALSE, GL_FALSE, GL_TRUE);
	// Se reduce su volumen
	this->sonidoDisparo->setVolume (0.5f);
}
