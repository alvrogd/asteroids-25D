#include "Movil.h"

Movil::Movil (glm::vec3 escalado, Modelo * modelo, glm::vec3 posicion, glm::vec3 velocidad, glm::vec3 coefAceleracion, glm::vec3 coefDeceleracion, glm::vec3 rotacion)
	: Objeto(escalado, modelo)
{
	// Se guardan los par�metros dados
	this->posicion = posicion;
	this->velocidad = velocidad;
	this->coefAceleracion = coefAceleracion;
	this->coefDeceleracion = coefDeceleracion;
	this->rotacion = rotacion;
}

void Movil::dibujar (glm::mat4 transformacionPadre, Shader * shader) const
{
	// Se inicializan las matrices a emplear
	glm::mat4 transformacion = transformacionPadre;

	// 2 -> Se aplica la traslaci�n guardada
	transformacion = glm::translate (transformacion, this->posicion);
	
	// 1 -> Se aplica la rotaci�n guardada sobre cada uno de los ejes
	transformacion = glm::rotate (transformacion, glm::radians (this->rotacion.x), glm::vec3 (1.0f, 0.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (this->rotacion.y), glm::vec3 (0.0f, 1.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (this->rotacion.z), glm::vec3 (0.0f, 0.0f, 1.0f));
	
	// Se ejecuta el m�todo sobreescrito pas�ndole como argumento la matriz compuesta; es decir, se preserva el m�todo
	// original y simplemente se a�aden las transformaciones aqu� realizadas
	Objeto::dibujar (transformacion, shader);
}
