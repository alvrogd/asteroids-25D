#ifndef TEXTURA_H
#define TEXTURA_H

// Para evitar un error de compilaci�n
// todo sigue siendo necesaria?
#include <windows.h>

// Librer�as de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para cargar informaci�n en shaders
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


	/* M�todos */

	void activar (Shader *shader, const char *uniform, int unidadTextura) const;

	static void desactivar (int unidadTextura);


private:
	/* Atributos */

	unsigned int idTextura;


	/* M�todos */

	void cargarTextura (const char *fichero, GLenum modo);
};

#endif