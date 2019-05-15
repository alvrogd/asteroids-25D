#include "Particula.h"

// Se representán como esferas
#include "Forma.h"

std::vector<Particula *> *Particula::conjuntoParticulas = NULL;


Particula::Particula (glm::vec3 posicion, glm::vec3 velocidad, glm::vec4 color, float vida)
{
	// Se guardan los valores dados
	this->posicion = posicion;
	this->velocidad = velocidad;
	this->color = color;
	this->vida = vida;
}

void Particula::actualizarEstado (float tiempoTranscurrido)
{
	// Se modifica la posición de la partícula en función de la velocidad registrada
	this->posicion.x += this->velocidad.x;
	this->posicion.y += this->velocidad.y;
	this->posicion.z += this->velocidad.z;

	// Se reduce el tiempo de vida de la partícula
	this->vida -= tiempoTranscurrido;
}

void Particula::dibujar (glm::mat4 transformacionPadre, Shader * shader)
{
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

	// Se activa el shader dado
	shader->usar ();

	// Se establece el color de la partícula en el shader
	shader->setVec4 ("Color", this->color);

	// Se inicializan las matrices a emplear
	glm::mat4 transformacion = transformacionPadre;

	// 3 -> Se aplica la traslación guardada
	transformacion = glm::translate (transformacion, getPosicion ());

	/*// 2 -> Se aplica la rotación guardada sobre cada uno de los ejes
	glm::vec3 rot = getRotacion ();
	glm::vec3 correcion = getCorreccionRotacion ();
	transformacion = glm::rotate (transformacion, glm::radians (rot.x + correcion.x), glm::vec3 (1.0f, 0.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (rot.y + correcion.y), glm::vec3 (0.0f, 1.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (rot.z + correcion.z), glm::vec3 (0.0f, 0.0f, 1.0f));*/

	// 1 -> Se aplica un escalado estándar para todas las partículas
	transformacion = glm::scale (transformacion, glm::vec3(2.0f, 2.0f, 2.0f));

	// Se aplica la transformación calculada a la matriz del modelo del shader
	shader->setMat4 ("modelMatrix", transformacion);

	// Se representa el disparo
	Forma::dibujarEsfera ();


	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Particula::generarExplosion (glm::vec3 posicion)
{
}
