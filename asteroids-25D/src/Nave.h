#ifndef NAVE_H
#define NAVE_H

// Clase a heredar
#include "Movil.h"

// Para disparar
#include "Disparo.h"

// Sonido
#include <irrKlang.h>


class Nave :
	public Movil
{
public:

	/* Constructor */

	Nave (glm::vec3 escalado, Modelo *modelo, float radioHitbox, glm::vec3 posicion, glm::vec3 velocidad, glm::vec3
		coefAceleracion, glm::vec3 coefDeceleracion, glm::vec3 rotacion, glm::vec3 correcionRotacion);


	/* Getters y setters */

	irrklang::ISound *getSonidoDisparo ()
	{
		return this->sonidoDisparo;
	}

	void setSonidoDisparo (irrklang::ISound *sonidoDisparo)
	{
		this->sonidoDisparo = sonidoDisparo;
	}

	bool getIsAcelerando () const
	{
		return this->isAcelerando;
	}

	void setIsAcelerando(bool isAcelerando)
	{
		this->isAcelerando = isAcelerando;
	}

	bool getIsDestruida () const
	{
		return this->isDestruida;
	}

	void setIsDestruida (bool isDestruida)
	{
		this->isDestruida = isDestruida;
	}


	/* Métodos */

	void actualizarEstado (float tiempoTranscurrido) override;

	void disparar ();

	void explotar ();


private:

	/* Atributos */

	irrklang::ISound *sonidoDisparo = NULL;

	// Si el usuario se encuentra presionando la tecla que provoca que la nave acelere
	bool isAcelerando;

	// Si ha sido destruida
	bool isDestruida;
};

#endif
