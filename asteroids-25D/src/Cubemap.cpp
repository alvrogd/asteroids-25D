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
	// Se generan el VAO y VBO del cubemap
	glGenVertexArrays (1, &(this->VAO));
	glGenBuffers (1, &(this->VBO));

	// Se vincula el VAO para configurarlo
	glBindVertexArray (this->VAO);

	// Se vincula el VBO
	glBindBuffer (GL_ARRAY_BUFFER, this->VBO);

	// Se copian los vértices del cubemap al VBO
	glBufferData (GL_ARRAY_BUFFER, sizeof(verticesCubemap), verticesCubemap, GL_STATIC_DRAW);

	// Se vinculan estas propiedades con la entrada 0 de los shaders y se especifica de dónde obtener la información
	// para dicha entrada
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void *)0);

	// Se desvinculan el VAO y el VBO
	glBindVertexArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, 0);

	// Se borran el VBO
	glDeleteBuffers (1, &(this->VBO));

	// Se cargan en OpenGL las texturas especificadas
	this->id = LectorImagen::cargarCubemap (caras);
}

void Cubemap::dibujar (Shader *shader) const
{
	// Se modifica la función de depth-testing dado que el cubemap presenta valores de profundidad de 1.0 y, si no se
	// hiciese, no se renderizaría al ser la condición por defecto que "valorDepthBuffer < 1.0"
	glDepthFunc (GL_LEQUAL);

	// Se emplea el shader dado
	shader->usar ();

	// Se renderiza el cubemap, empleando la textura cargada
	glBindVertexArray (this->VAO);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, this->id);
	glDrawArrays (GL_TRIANGLES, 0, 36);
	glBindVertexArray (0);

	// Se restaura la función de depth-testing por defecto
	glDepthFunc (GL_LESS);
}
