#ifndef MOVIL_H
#define MOVIL_H

// Clase a heredar
#include "Objeto.h"


class Movil :
	public Objeto
{
public:

	/* Atributos */

	static glm::vec3 coordenadasWarp;


	/* Constructor */
	
	Movil (glm::vec3 escalado, Modelo *modelo, float radioHitbox, glm::vec3 posicion, glm::vec3 velocidad, glm::vec3
		coefAceleracion, glm::vec3 coefDeceleracion, glm::vec3 rotacion, glm::vec3 correcionRotacion);


	/* Getters y setters */

	float getRadioHitbox () const
	{
		return this->radioHitbox;
	}

	void setRadioHitbox (float radioHitbox)
	{
		this->radioHitbox = radioHitbox;
	}

	glm::vec3 & getPosicion ()
	{
		return this->posicion;
	}

	glm::vec3 * getPosicionReferencia ()
	{
		return &(this->posicion);
	}

	void setPosicion (glm::vec3 posicion)
	{
		this->posicion = posicion;
	}

	glm::vec3 & getVelocidad ()
	{
		return this->velocidad;
	}

	glm::vec3 * getVelocidadReferencia ()
	{
		return &(this->velocidad);
	}

	void setVelocidad (glm::vec3 velocidad)
	{
		this->velocidad = velocidad;
	}

	glm::vec3 & getCoefAceleracion ()
	{
		return this->coefAceleracion;
	}

	glm::vec3 * getCoefAceleracionReferencia ()
	{
		return &(this->coefAceleracion);
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

	glm::vec3 * getRotacionReferencia ()
	{
		return &(this->rotacion);
	}

	void setRotacion (glm::vec3 rotacion)
	{
		this->rotacion = rotacion;
	}

	glm::vec3 & getCorreccionRotacion ()
	{
		return this->correccionRotacion;
	}

	glm::vec3 * getCorreccionRotacionReferencia ()
	{
		return &(this->correccionRotacion);
	}

	void setCorreccionRotacion (glm::vec3 correccionRotacion)
	{
		this->correccionRotacion = correccionRotacion;
	}


	/* Métodos */

	virtual void actualizarEstado (float tiempoTranscurrido);

	void dibujar (glm::mat4 transformacionPadre, Shader *shader) override;

	bool checkColision (Movil *movil) const;


private:

	/* Atributos */

	// Radio de su hitbox, las hitbox son esferas
	float radioHitbox;

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

	// Corrección de la rotación por el modelo empleado
	glm::vec3 correccionRotacion;
};

#endif