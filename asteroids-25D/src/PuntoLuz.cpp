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

void PuntoLuz::cargar (Shader * shader, glm::mat4 transformacion) const
{
	// Se cargan en el fragment shader las propiedades de la luz

	// A la posición se aplica además la transformación dada
	glm::vec4 posicionTransformada = transformacion * glm::vec4 (this->posicion, 1.0f);
	shader->setVec3 ("puntoLuz.posicion", glm::vec3(posicionTransformada));

	shader->setFloat ("puntoLuz.constante", this->constante);
	shader->setFloat ("puntoLuz.lineal", this->lineal);
	shader->setFloat ("puntoLuz.cuadratica", this->cuadratica);

	shader->setVec3 ("puntoLuz.ambiental", this->ambiental);
	shader->setVec3 ("puntoLuz.difusa", this->difusa);
	shader->setVec3 ("puntoLuz.especular", this->especular);
}
