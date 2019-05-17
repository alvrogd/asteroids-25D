#ifndef SONIDO_H
#define SONIDO_H


// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Sonido
#include <irrKlang.h>

// Conjuntos de datos
#include <vector>


class Sonido
{
public:
	
	/* Getters */

	irrklang::ISoundEngine *getSonido2D ()
	{
		return this->sonido2D;
	}

	irrklang::ISoundEngine *getSonido3D ()
	{
		return this->sonido3D;
	}

	std::vector<irrklang::ISoundEngine *> getEfectosSonido ()
	{
		return this->sonidos3D;
	}


	/* Destructor */

	~Sonido ();


	/* Métodos */

	static Sonido* getSonido ();

	void actualizar (glm::vec3 posicionOyente, glm::vec3 direccionOyente);

	void reproducirExplosion (glm::vec3 posicion);


private:

	/* Atributos */

	// Instancia singleton
	static Sonido *sonido;

	// Reproductores
	irrklang::ISoundEngine *sonido2D;
	irrklang::ISoundEngine *sonido3D;

	std::vector<irrklang::ISoundEngine *> sonidos3D;


	/* Constructor */

	Sonido ();
};

#endif
