#ifndef MOVIL_H
#define MOVIL_H

// Clase a heredar
#include "Objeto.h"


class Movil :
	public Objeto
{
public:

	/* Constructor */
	
	Movil (glm::vec3 escalado, Modelo *modelo, glm::vec3 posicion, glm::vec3 velocidad, glm::vec3 coefAceleracion,
		glm::vec3 coefDeceleracion, glm::vec3 rotacion);


	/* Getters y setters */

	glm::vec3 & getPosicion ()
	{
		return this->posicion;
	}

	void setPosicion (glm::vec3 posicion)
	{
		this->posicion = posicion;
	}

	glm::vec3 & getVelocidad ()
	{
		return this->velocidad;
	}

	void setVelocidad (glm::vec3 velocidad)
	{
		this->velocidad = velocidad;
	}

	glm::vec3 & getCoefAceleracion ()
	{
		return this->coefAceleracion;
	}

	void setCoefAceleracion (glm::vec3 coefAceleracion)
	{
		this->coefAceleracion = coefAceleracion;
	}

	glm::vec3 & getCoefDeceleracion ()
	{
		return this->coefDeceleracion;
	}

	void setCoefDeceleracion (glm::vec3 coefDeceleracion)
	{
		this->coefDeceleracion = coefDeceleracion;
	}

	glm::vec3 & getRotacion ()
	{
		return this->rotacion;
	}

	void setRotacion (glm::vec3 rotacion)
	{
		this->rotacion = rotacion;
	}


	/* Métodos */

	void dibujar (glm::mat4 transformacionPadre, Shader *shader) const override;


private:

	/* Atributos */

	// Posición en el espacio
	glm::vec3 posicion;

	// Velocidad en cada coordenada
	glm::vec3 velocidad;

	// Coeficiente de aceleración en cada coordenada
	glm::vec3 coefAceleracion;

	// Coeficiente de deceleración en cada coordenada
	glm::vec3 coefDeceleracion;

	// Rotación sobre cada eje
	glm::vec3 rotacion;

	// Dirección en la que el móvil mira
	glm::vec3 direccion;
};

#endif