#include "Nave.h"

#include <iostream>

// Para reproducir sonido
#include "Sonido.h"

// Para crear part�culas
#include "Particula.h"


Nave::Nave (glm::vec3 escalado, Modelo * modelo, float radioHitbox, glm::vec3 posicion, glm::vec3 velocidad, glm::vec3 coefAceleracion, glm::vec3 coefDeceleracion, glm::vec3 rotacion, glm::vec3 correcionRotacion)
	: Movil (escalado, modelo, radioHitbox, posicion, velocidad, coefAceleracion, coefDeceleracion, rotacion, correcionRotacion)
{
	// Inicialmente, se supone que la nave no est� siendo acelerada
	this->isAcelerando = false;
}

void Nave::actualizarEstado (float tiempoTranscurrido)
{
	// Se almacena la posici�n antes de ser actualizada
	glm::vec3 antiguaPosicion = this->getPosicion ();

	// Se actualiza el estado de la nave
	Movil::actualizarEstado (tiempoTranscurrido);

	// Si la nave se encuentran acelerando y se ha movido al menos una unidad
	if (this->isAcelerando && glm::distance (antiguaPosicion, this->getPosicion ()) > 1.0f)
	{
		// Se calcula una posici�n que se sit�e ligeramente por detr�s de la nave
		glm::vec3 posicion = this->getPosicion ();

		posicion.x += sinf (glm::radians (this->getRotacion ().y)) * 20.0f;
		posicion.z += cosf (glm::radians (this->getRotacion ().y)) * 20.0f;

		// Se crean varias part�culas que simular�n un rastro que la nave dejar� al moverse
		for (int i = 0; i < 8; i++)
		{
			// - Detr�s de la nave, con la posici�n alterada ligeramente
			// - Mismo sentido de movimiento que la nave, pero con la velocidad reducida al 10%
			// - Color naranja
			// - 0.25 segundos de duraci�n

			// Rango [-3.0, 3.0)
			glm::vec3 alteracion;
			alteracion.x = fmod (std::rand (), 6.0f) - 3.0f;
			alteracion.y = fmod (std::rand (), 6.0f) - 3.0f;
			alteracion.z = fmod (std::rand (), 6.0f) - 3.0f;

			Particula::conjuntoParticulas->push_back (new Particula (posicion + alteracion, 0.1f *
				this->getVelocidad (), glm::vec4 (1.0f, 0.38f, 0.28f, 1.0f), 0.25f));
		}
	}
}

void Nave::disparar ()
{
	// Se crean 12 disparos en la posici�n de la nave y que avance en la direcci�n a la que apunta la nave
	//for (int i = 0; i < 10; i++)
	//{
		// Se crea un disparo en la posici�n de la nave y que avance en la direcci�n a la que apunta la nave
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

	// Se reproduce un efecto de disparo; sin repetici�n, reproducci�n desde el inicio, y tratamiento de "track" para
	// obtener una referencia a �l
	this->sonidoDisparo = Sonido::getSonido ()->getSonido2D ()->play2D ("laser_rapid.ogg", GL_FALSE, GL_FALSE, GL_TRUE);
	// Se reduce su volumen
	this->sonidoDisparo->setVolume (0.5f);
}
