#ifndef MESH_H
#define MESH_H

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para representarse
#include "Shader.h"

// Para conjuntos de datos
#include <vector>


// Representación de un vértice (no se modela como una clase para aprovechar mejor el principio de localidad)
struct SVertice
{
	glm::vec3 posicion;
	glm::vec3 normal;
	glm::vec2 coordenadasTexturas;
};

// Información de una textura (no se modela como una clase para aprovechar mejor el principio de localidad)
struct STextura
{
	unsigned int id;
	std::string tipo;
	std::string ruta;
};

class Mesh
{
public:
	/* Constructor */

	Mesh (std::vector<SVertice> vertices, std::vector<unsigned int> indices, std::vector<STextura> texturas);


	/* Getters y setters */

	std::vector<SVertice> & getVertices ()
	{
		return this->vertices;
	}

	void setVertices (std::vector<SVertice> vertices)
	{
		this->vertices = vertices;
	}

	std::vector<unsigned int> & getIndices ()
	{
		return this->indices;
	}

	void setIndices (std::vector<unsigned int> indices)
	{
		this->indices = indices;
	}

	std::vector<STextura> & getTexturas ()
	{
		return this->texturas;
	}

	void setTexturas (std::vector<STextura> texturas)
	{
		this->texturas = texturas;
	}

	unsigned int getVAO () const
	{
		return this->VAO;
	}

	void setVAO (unsigned int VAO)
	{
		this->VAO = VAO;
	}

	unsigned int getVBO () const
	{
		return this->VBO;
	}

	void setVBO (unsigned int VBO)
	{
		this->VBO = VBO;
	}

	unsigned int getEBO () const
	{
		return this->EBO;
	}

	void setEBO (unsigned int EBO)
	{
		this->EBO = EBO;
	}


	/* Métodos */

	void dibujar (Shader *shader);


private:

	/* Atributos */

	// Datos del mesh
	std::vector<SVertice> vertices;
	std::vector<unsigned int> indices;
	std::vector<STextura> texturas;

	// Información de renderizado
	unsigned int VAO, VBO, EBO;


	/* Métodos */

	void configurarMesh ();
};

#endif