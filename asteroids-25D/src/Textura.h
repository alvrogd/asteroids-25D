#ifndef TEXTURA_H
#define TEXTURA_H

// Para evitar un error de compilación
// todo sigue siendo necesaria?
#include <windows.h>

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para cargar información en shaders
#include "Shader.h"



class Textura
{
public:
	/* Constructor */
	Textura (const char *fichero, GLenum modo);


	/* Destructor */

	~Textura ();


	/* Getters y setters */
	
	unsigned int getIdTextura () const
	{
		return this->idTextura;
	}

	void setIdTextura (unsigned int idTextura)
	{
		this->idTextura = idTextura;
	}


	/* Métodos */

	void activar (Shader *shader, const char *uniform, int unidadTextura) const;

	static void desactivar (int unidadTextura);


private:
	/* Atributos */

	unsigned int idTextura;


	/* Métodos */

	void cargarTextura (const char *fichero, GLenum modo);
};

#endif