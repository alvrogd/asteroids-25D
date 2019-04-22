#ifndef ASTRO_H
#define ASTRO_H

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para representar el astro
#include "Forma.h"

// Para manejar los ángulos
#include <cmath>

// Para los hijos del astro
#include <vector>

// Para representar la órbita del astro
#include "Rastro.h"

// Para cargar al program shader la model matrix
#include "Shader.h"

// Material del que se encuentra formado el astro
#include "Material.h"

// Texturas que componen el astro
#include "Textura.h"


class Astro
{
public:
	/* Constructor */

	Astro (float radio, float periodoRotacion, Astro *padre, float periodoTraslacion, float distanciaPadre, Material
		material, Textura *textura1, Textura *textura2 );


	/* Atributos */

	// Si deben representarse las órbitas de los astros
	static bool representarOrbitas;


	/* Getters y setters */

	float getRadio () const
	{
		return radio;
	}

	void setRadio (float radio)
	{
		this->radio = radio;
	}

	float getPeriodoRotacion () const
	{
		return periodoRotacion;
	}

	void setPeriodoRotacion (float periodoRotacion)
	{
		this->periodoRotacion = periodoRotacion;
	}

	float getAnguloRotacion () const
	{
		return anguloRotacion;
	}

	void setAnguloRotacion (float anguloRotacion)
	{
		this->anguloRotacion = anguloRotacion;
	}

	float getPeriodoTraslacion () const
	{
		return periodoTraslacion;
	}

	void setPeriodoTraslacion (float periodoTraslacion)
	{
		this->periodoTraslacion = periodoTraslacion;
	}

	float getAnguloTraslacion () const
	{
		return anguloTraslacion;
	}

	void setAnguloTraslacion (float anguloTraslacion)
	{
		this->anguloTraslacion = anguloTraslacion;
	}

	Astro * getPadre () const
	{
		return padre;
	}

	void setPadre (Astro *padre)
	{
		this->padre = padre;
	}

	float getDistanciaPadre () const
	{
		return distanciaPadre;
	}

	void setDistanciaPadre (float distanciaPadre)
	{
		this->distanciaPadre = distanciaPadre;
	}

	std::vector<Astro *> & getHijos ()
	{
		return this->hijos;
	}

	void setHijos (const std::vector<Astro *> hijos)
	{
		this->hijos = hijos;
	}

	Rastro & getRastro ()
	{
		return rastro;
	}

	void setRastro (const Rastro rastro)
	{
		this->rastro = rastro;
	}

	bool isRepresentarRastro ()
	{
		return this->representarRastro;
	}

	void setRepresentarRastro (bool representarRastro)
	{
		this->representarRastro = representarRastro;
	}

	glm::vec3 & getUltimaPosicion ()
	{
		return this->ultimaPosicion;
	}

	void setUltimaPosicion (const glm::vec3 ultimaPosicion)
	{
		this->ultimaPosicion = ultimaPosicion;
	}

	Material & getMaterial ()
	{
		return this->material;
	}

	void setMaterial (const Material material)
	{
		this->material = material;
	}

	Textura * getTextura1 ()
	{
		return this->textura1;
	}

	void setTextura1 (Textura *textura1)
	{
		this->textura1 = textura1;
	}

	Textura * getTextura2 ()
	{
		return this->textura2;
	}

	void setTextura2 (Textura *textura2)
	{
		this->textura2 = textura2;
	}


	/* Métodos */

	// Se actualizan la traslación y rotación del astro
	void actualizarEstado (float tiempoTranscurrido);

	// Se representa el astro
	virtual void dibujar (glm::mat4 transformacionPadre, Shader *shaderColor, Shader *shaderColorT1, Shader
		*shaderColorT2, Shader *shaderIluminacionT1, Shader *shaderIluminacionT2);


private:
	/* Atributos */

	float radio;    // Km

	float periodoRotacion;    // Horas
	float anguloRotacion;

	float periodoTraslacion;    // Horas
	float anguloTraslacion;

	Astro *padre;
	float distanciaPadre;    // UA; distancia a la que orbita del padre

	std::vector<Astro *> hijos;

	Rastro rastro;
	bool representarRastro;    // Si se representa el rastro del astro o no

	glm::vec3 ultimaPosicion;    // Coordenadas del centro del astro en su última renderización

	Material material;    // Material del que está formado el astro

	Textura *textura1;    // Texturas con las que representar el astro
	Textura *textura2;
};

#endif
