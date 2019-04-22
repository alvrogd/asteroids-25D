#ifndef ESTRELLA_H
#define ESTRELLA_H

// Clase a heredar
#include "Astro.h"

// Para iluminar
#include "PuntoLuz.h"

class Estrella :
	public Astro
{
public:
	/* Constructor */

	Estrella (float radio, float periodoRotacion, Astro *padre, float periodoTraslacion, float distanciaPadre,
		Material material, Textura *textura1, Textura *textura2, PuntoLuz luz);


	/* Getters y setters */

	PuntoLuz & getLuz ()
	{
		return this->luz;
	}

	void setLuz (PuntoLuz luz)
	{
		this->luz = luz;
	}


	/* Métodos */

	void dibujar (glm::mat4 transformacionPadre, Shader *shaderColor, Shader *shaderColorT1, Shader	*shaderColorT2,
		Shader *shaderIluminacionT1, Shader *shaderIluminacionT2) override;


private:
	/* Atributos */

	// Luz emitida
	PuntoLuz luz;
};

#endif
