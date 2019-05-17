#include "Movil.h"

#include <cmath>

// Se inicializan a un valor predeterminado m�nimo
glm::vec3 Movil::coordenadasWarp = glm::vec3 (10.0f, 10.0f, 10.0f);


Movil::Movil (glm::vec3 escalado, Modelo * modelo, float radioHitbox, glm::vec3 posicion, glm::vec3 velocidad,
	glm::vec3 coefAceleracion, glm::vec3 coefDeceleracion, glm::vec3 rotacion, glm::vec3 correccionRotacion)
	: Objeto(escalado, modelo)
{
	// Se guardan los par�metros dados
	this->radioHitbox = radioHitbox;
	this->posicion = posicion;
	this->velocidad = velocidad;
	this->coefAceleracion = coefAceleracion;
	this->coefDeceleracion = coefDeceleracion;
	this->rotacion = rotacion;
	this->correccionRotacion = correccionRotacion;
}

void Movil::actualizarEstado (float tiempoTranscurrido)
{
	// Se modifica la posici�n del m�vil en funci�n de la velocidad registrada
	this->posicion.x += this->velocidad.x;
	this->posicion.z += this->velocidad.z;

	// Si alguna de las coordenadas ha superado el warp establecido, se "invierte" para mover el m�vil al lado opuesto
	if (fabs (this->posicion.x) > Movil::coordenadasWarp.x)
	{
		this->posicion.x *= -1;
	}

	if (fabs (this->posicion.z) > Movil::coordenadasWarp.z)
	{
		this->posicion.z *= -1;
	}

	// Se reduce la velocidad del m�vil en cada coordenada
	this->velocidad.x *= 1 - this->coefDeceleracion.x;
	this->velocidad.z *= 1 - this->coefDeceleracion.z;
}

void Movil::dibujar (glm::mat4 transformacionPadre, Shader * shader)
{
	// Se inicializan las matrices a emplear
	glm::mat4 transformacion = transformacionPadre;

	// 2 -> Se aplica la traslaci�n guardada
	transformacion = glm::translate (transformacion, this->posicion);
	
	// 1 -> Se aplica la rotaci�n guardada sobre cada uno de los ejes
	transformacion = glm::rotate (transformacion, glm::radians (this->rotacion.x + this->correccionRotacion.x), glm::vec3 (1.0f, 0.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (this->rotacion.y + this->correccionRotacion.y), glm::vec3 (0.0f, 1.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (this->rotacion.z + this->correccionRotacion.z), glm::vec3 (0.0f, 0.0f, 1.0f));
	
	// Se ejecuta el m�todo sobreescrito pas�ndole como argumento la matriz compuesta; es decir, se preserva el m�todo
	// original y simplemente se a�aden las transformaciones aqu� realizadas
	Objeto::dibujar (transformacion, shader);
}

bool Movil::checkColision (Movil * movil) const
{
	// Se obtiene la distancia entre los centros de los m�viles
	float distancia = glm::distance(this->posicion, movil->getPosicion ());

	// Los m�viles colisionar�n si la distancia entre ellos es menor que la suma de sus radios
	return(distancia < this->radioHitbox + movil->getRadioHitbox ());
}
