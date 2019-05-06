#include "Objeto.h"

Objeto::Objeto (glm::vec3 escalado, Modelo * modelo)
{
	// Se guardan los valores dados
	this->escalado = escalado;
	this->modelo = modelo;
}

void Objeto::dibujar (glm::mat4 transformacionPadre, Shader * shader)
{
	// Se aplica el escalado guardado a la matriz dada, y se le pasa al modelo que representa al objeto
	this->modelo->dibujar (glm::scale(transformacionPadre, this->escalado), shader);
}

