#ifndef DISPARO_H
#define DISPARO_H

// Clase a heredar
#include "Movil.h"

// Se represent�n como rect�ngulos
#include "Forma.h"

// Para conjuntos de datos
#include <vector>

// Para emitir luz
#include "PuntoLuz.h"


class Disparo :
	public Movil
{
public:

	/* Atributos */

	static Modelo *modelo;

	static std::vector<Disparo *> * conjuntoDisparos;


	/* Constructor */

	Disparo (glm::vec3 posicion, glm::vec3 velocidad, glm::vec3 rotacion);


	/* Destructor */

	~Disparo ();


	/* Getters y setters */

	int getWarpsRestantes () const
	{
		return this->warpsRestantes;
	}

	void setWarpsRestantes (int warpsRestantes)
	{
		this->warpsRestantes = warpsRestantes;
	}


	/* M�todos */

	void actualizarEstado (float tiempoTranscurrido) override;

	void dibujar (glm::mat4 transformacionPadre, Shader *shader) override;


private:

	/* Atributos */

	// N�mero de warps restantes del disparo antes de desaparecer
	int warpsRestantes;

	// Luz emitida

};

#endif
