#include "Cubemap.h"

float verticesCubemap[] = {
	// Posiciones          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

Cubemap::Cubemap (std::vector<std::string> caras)
{
	// Se genera el VAO del cubo y se vincula
	glGenVertexArrays (1, &(this->VAO));
	glGenBuffers (1, &(this->VBO));

	glBindVertexArray (this->VAO);
	// AHORA S� ES POSIBLE configurar los correspondientes VBO, EBO y atributos porque los cambios se registrar�n en
	// el VAO



	// Se vincula el VBO del cubo
	glBindBuffer (GL_ARRAY_BUFFER, this->VBO);

	// Se copian los v�rtices del cubo al VBO (STATIC = una modificaci�n y muchas lecturas, DRAW = contenidos
	// modificados por el programa y usados por OpenGL para dibujado); cada v�rtice tiene 6 floats
	glBufferData (GL_ARRAY_BUFFER, sizeof(verticesCubemap), verticesCubemap, GL_STATIC_DRAW);

	// Se vinculan estas propiedades con la entrada 0 de los shaders
	glEnableVertexAttribArray (0);

	// Se especifica c�mo tratar la informaci�n de los v�rtices ("0" al haber especificado "location = 0" en el
	// shader, "GL_FALSE" para informaci�n no normalizada); ahora, se ha vinculado la informaci�n de VBO al vertex
	// attribute 0 del shader
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void *)0);



	// Se desvinculan el VAO y ambos buffers (ES NECESARIO DESVINCULAR EN PRIMER LUGAR EL VAO)
	glBindVertexArray (0);
	//glBindBuffer (GL_ARRAY_BUFFER, 0);

	// Se borran los buffers


	// Se cargan en OpenGL las texturas especificadas
	this->id = LectorImagen::cargarCubemap (caras);
}

void Cubemap::dibujar (Shader *shader) const
{
	glDepthFunc (GL_LEQUAL);
	shader->usar ();
	glBindVertexArray (this->VAO);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, this->id);
	glDrawArrays (GL_TRIANGLES, 0, 36);
	glBindVertexArray (0);
	glDepthFunc (GL_LESS);
}
