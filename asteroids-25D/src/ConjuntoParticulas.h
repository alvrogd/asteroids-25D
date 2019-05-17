#ifndef CONJUNTO_PARTICULAS_H
#define CONJUNTO_PARTICULAS_H


// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Partes que componen un objeto de la clase
#include "Particula.h"

// Para conjuntos de datos
#include <vector>


class ConjuntoParticulas
{
public:

	/* Atributos */

	static std::vector<ConjuntoParticulas *> *conjuntoConjuntoParticulas;


	/* Constructor */

	ConjuntoParticulas (glm::vec3 posicion, glm::vec4 color);


	/* Destructor */

	~ConjuntoParticulas ();


	/* Getters y setters */

	std::vector<Particula *> getParticulas ()
	{
		return this->particulas;
	}

	void setParticulas (std::vector<Particula *> particulas)
	{
		this->particulas = particulas;
	}

	glm::vec3 & getPosicion ()
	{
		return this->posicion;
	}

	void setPosicion (glm::vec3 posicion)
	{
		this->posicion = posicion;
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


	/* Métodos */

	void actualizarEstado (float tiempoTranscurrido);

	void dibujar (glm::mat4 transformacionPadre, Shader *shader);

	void generarExplosion (float vida);

	bool isMuerto () const;


private:

	/* Atributos */

	std::vector<Particula *> particulas;

	glm::vec3 posicion;

	glm::vec4 color;

	// Tiempo desde que aparece hasta que debe dejar de mostrarse en pantalla
	float vida;

	// Inverso de la vida dada
	float inversoVida;

	// Tiempo que lleva activo en pantalla
	float edad;
};

#endif
