#include "Movil.h"

#include <cmath>

glm::vec3 Movil::coordenadasWarp = glm::vec3 (10.0f, 10.0f, 10.0f);


Movil::Movil (glm::vec3 escalado, Modelo * modelo, glm::vec3 posicion, glm::vec3 velocidad, glm::vec3 coefAceleracion, glm::vec3 coefDeceleracion, glm::vec3 rotacion, glm::vec3 correccionRotacion)
	: Objeto(escalado, modelo)
{
	// Se guardan los parámetros dados
	this->posicion = posicion;
	this->velocidad = velocidad;
	this->coefAceleracion = coefAceleracion;
	this->coefDeceleracion = coefDeceleracion;
	this->rotacion = rotacion;
	this->correccionRotacion = correccionRotacion;
}

void Movil::actualizarEstado ()
{
	// Se modifica la posición del móvil en función de la velocidad registrada
	this->posicion.x += this->velocidad.x;
	this->posicion.z += this->velocidad.z;

	// Si alguna de las coordenadas ha superado el warp establecido, se "invierte" para mover el móvil al lado opuesto
	if (fabs (this->posicion.x) > Movil::coordenadasWarp.x)
	{
		this->posicion.x *= -1;
	}

	if (fabs (this->posicion.z) > Movil::coordenadasWarp.z)
	{
		this->posicion.z *= -1;
	}

	// Se reduce la velocidad de la nave en cada coordenada
	this->velocidad.x *= 1 - this->coefDeceleracion.x;
	this->velocidad.z *= 1 - this->coefDeceleracion.z;
}

void Movil::dibujar (glm::mat4 transformacionPadre, Shader * shader) const
{
	// Se inicializan las matrices a emplear
	glm::mat4 transformacion = transformacionPadre;

	// 2 -> Se aplica la traslación guardada
	transformacion = glm::translate (transformacion, this->posicion);
	
	// 1 -> Se aplica la rotación guardada sobre cada uno de los ejes
	transformacion = glm::rotate (transformacion, glm::radians (this->rotacion.x + this->correccionRotacion.x), glm::vec3 (1.0f, 0.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (this->rotacion.y + this->correccionRotacion.y), glm::vec3 (0.0f, 1.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (this->rotacion.z + this->correccionRotacion.z), glm::vec3 (0.0f, 0.0f, 1.0f));
	
	// Se ejecuta el método sobreescrito pasándole como argumento la matriz compuesta; es decir, se preserva el método
	// original y simplemente se añaden las transformaciones aquí realizadas
	Objeto::dibujar (transformacion, shader);
}
