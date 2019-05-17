#ifndef PARTICULA_H
#define PARTICULA_H

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para poder aplicar sus transformaciones en el renderizado
#include "Shader.h"

// Conjuntos de datos
#include <vector>


class Particula
{
public:

	/* Atributos */

	static std::vector<Particula *> *conjuntoParticulas;


	/* Constructor */

	Particula (glm::vec3 posicion, glm::vec3 velocidad, glm::vec4 color, float vida);

	Particula (glm::vec3 posicion, glm::vec3 velocidad, glm::vec4 color, float vida, bool perteneceConjunto);


	/* Destructor */

	~Particula ();


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

	glm::vec4 & getColor ()
	{
		return this->color;
	}

	void setColor (glm::vec4 color)
	{
		this->color = color;
	}

	float getVida () const
	{
		return this->vida;
	}

	void setVida (float vida)
	{
		this->vida = vida;
	}

	float getEdad () const
	{
		return this->edad;
	}

	void setEdad (float edad)
	{
		this->edad = edad;
	}

	bool getPerteneceConjunto () const
	{
		return this->perteneceConjunto;
	}

	void setPerteneceConjunto (bool perteneceConjunto)
	{
		this->perteneceConjunto = perteneceConjunto;
	}


	/* Métodos */

	void actualizarEstado (float tiempoTranscurrido);

	void actualizarEstadoConjunto (float tiempoTranscurrido);

	void dibujar (glm::mat4 transformacionPadre, Shader *shader);

	void dibujarConjunto (glm::mat4 transformacionPadre, Shader *shader);

	static void generarExplosion (glm::vec3 posicion);

	bool isMuerta () const;


private:

	/* Atributos */

	glm::vec3 posicion;
	glm::vec3 velocidad;

	glm::vec4 color;

	// Tiempo desde que aparece hasta que debe dejar de mostrarse en pantalla
	float vida;

	// Tiempo que lleva activa en pantalla
	float edad;

	// Si la partícula forma parte de un conjunto de partículas
	bool perteneceConjunto;
};

#endif
