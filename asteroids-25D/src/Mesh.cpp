#include "Mesh.h"

#include <iostream>

Mesh::Mesh (std::vector<SVertice> vertices, std::vector<unsigned int> indices, std::vector<STextura> texturas)
{
	// Se guardan los datos dados
	this->vertices = vertices;
	this->indices = indices;
	this->texturas = texturas;

	// Se configura el mesh a partir de los datos dados
	configurarMesh ();
}

void Mesh::dibujar (Shader * shader)
{
	// Se inicializa a '1' el número de texturas difusas y especulares disponibles
	unsigned int numeroDifusa = 1;
	unsigned int numeroEspecular = 1;

	// Se recorren todas las texturas del mesh
	for (unsigned int i = 0; i < this->texturas.size (); i++)
	{
		// Se activa la unidad de textura necesaria
		glActiveTexture (GL_TEXTURE0 + i);

		glBindTexture (GL_TEXTURE_2D, this->texturas.at(i).id);

		// Se compone el nombre de la variable uniform que corresponderá a la textura iterada
		std::string numero;
		std::string nombre = this->texturas.at (i).tipo;

		if (nombre == "texturaDifusa")
		{
			numero = std::to_string (numeroDifusa++);
		}

		else if (nombre == "texturaEspecular")
		{
			numero = std::to_string (numeroEspecular++);
		}

		else
		{
			std::cout << "oh oh" << std::endl;
		}

		// Se carga en el uniform el ID de la textura
		// TODO posible fallo
		//shader->setInt (("material." + nombre + numero).c_str (), this->texturas.at (i).id);
		shader->setInt (("material." + nombre + numero).c_str (), i);
	}

	// TODO no es necesario si no me equivoco
	// glActiveTexture(GL_TEXTURE0);

	// Ahora sí se representa el mesh
	glBindVertexArray (this->VAO);
	glDrawElements (GL_TRIANGLES, this->indices.size (), GL_UNSIGNED_INT, 0);
}

void Mesh::configurarMesh ()
{
	// Se crean los identificadores de los buffers necesarios para representarse en OpenGL
	glGenVertexArrays (1, &(this->VAO));
	glGenBuffers (1, &(this->VBO));
	glGenBuffers (1, &(this->EBO));

	// Se vincula el VAO para trabajar sobre él
	glBindVertexArray (this->VAO);

	// Se configura el VBO en primer lugar
	glBindBuffer (GL_ARRAY_BUFFER, this->VBO);

	// Se cargan todos los datos de los vértices dados
	glBufferData (GL_ARRAY_BUFFER, this->vertices.size () * sizeof (SVertice), this->vertices.data (), GL_STATIC_DRAW);

	// Ahora se configura el EBO
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	// Se cargan todos los índices dados
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, this->indices.size () * sizeof (unsigned int), this->indices.data (),
	GL_STATIC_DRAW);

	// Se configuran los inputs de la información cargada para los shaders

	// Posiciones de los vértices
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (SVertice), (void *)0);

	// Normales de los vértices
	glEnableVertexAttribArray (1);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (SVertice), (void *)offsetof (SVertice, normal));

	// Coordenadas de texturas de los vértices
	glEnableVertexAttribArray (2);
	glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, sizeof (SVertice), (void *)offsetof (SVertice, coordenadasTexturas));

	// Se desvincula el VAO
	glBindVertexArray (0);

	// TODO si no me equivoco ahora podrían eliminarse los buffers
}
