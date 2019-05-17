#include "PuntoLuz.h"

PuntoLuz::PuntoLuz (glm::vec3 posicion, float constante, float lineal, float cuadratica, glm::vec3 ambiental,
	glm::vec3 difusa, glm::vec3 especular)
{
	// Se guardan los valores dados
	this->posicion = posicion;

	this->constante = constante;
	this->lineal = lineal;
	this->cuadratica = cuadratica;

	this->ambiental = ambiental;
	this->difusa = difusa;
	this->especular = especular;
}

void PuntoLuz::cargar (Shader * shader, glm::mat4 transformacion, int numLuz) const
{
	// Se cargan en el fragment shader las propiedades de la luz

	// A la posición se aplica además la transformación dada
	glm::vec4 posicionTransformada = transformacion * glm::vec4 (this->posicion, 1.0f);
	shader->setVec3 ("puntosLuz[" + std::to_string(numLuz) + "].posicion", glm::vec3(posicionTransformada));

	shader->setFloat ("puntosLuz[" + std::to_string (numLuz) + "].constante", this->constante);
	shader->setFloat ("puntosLuz[" + std::to_string (numLuz) + "].lineal", this->lineal);
	shader->setFloat ("puntosLuz[" + std::to_string (numLuz) + "].cuadratica", this->cuadratica);

	shader->setVec3 ("puntosLuz[" + std::to_string (numLuz) + "].ambiental", this->ambiental);
	shader->setVec3 ("puntosLuz[" + std::to_string (numLuz) + "].difusa", this->difusa);
	shader->setVec3 ("puntosLuz[" + std::to_string (numLuz) + "].especular", this->especular);
}
