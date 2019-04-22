#include "Rastro.h"

#include <iostream>

int Rastro::maximoPosiciones = 2000;

Rastro::Rastro (float r, float g, float b)
{
	// Se guarda el color dado
	this->r = r;
	this->g = g;
	this->b = b;

	// Se generan los objetos de OpenGL para representar el rastro
	generarVAO ();
}

Rastro::~Rastro ()
{
	// Se eliminan los objetos creados
	glDeleteVertexArrays (1, &(this->VAO));
	glDeleteBuffers (1, &(this->VBO));
}

void Rastro::insertarPosicion (float x, float y, float z, float r, float g, float b)
{
	// Se guardan las coordenadas de la posición
	this->coordenadasPosiciones.push_back (x);
	this->coordenadasPosiciones.push_back (y);
	this->coordenadasPosiciones.push_back (z);

	// Se guarda el color de la posición
	this->coordenadasPosiciones.push_back (r);
	this->coordenadasPosiciones.push_back (g);
	this->coordenadasPosiciones.push_back (b);

	// Si se ha superado el tamaño máximo permitido, se eliminan los datos de la primera posición
	if (this->coordenadasPosiciones.size() / 6 > Rastro::maximoPosiciones)
	{
		this->coordenadasPosiciones.erase (this->coordenadasPosiciones.begin (), this->coordenadasPosiciones.begin ()
			+ 6);
	}

	// Se vincula el VAO creado para configurar los vértices del rastro
	glBindVertexArray (this->VAO);

	// Se vincula el VBO para trabajar con él
	glBindBuffer (GL_ARRAY_BUFFER, this->VBO);

	// Se actualiza el contenido del buffer
	glBufferSubData (GL_ARRAY_BUFFER, 0, this->coordenadasPosiciones.size () * sizeof (float),
		this->coordenadasPosiciones.data ());

	// Se desvinculan los objetos una vez finalizada la configuración
	glBindVertexArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void Rastro::dibujar (glm::mat4 transformacionPadre, Shader *shaderColor) const
{
	// Se activa el shader con color a emplear
	shaderColor->usar ();

	// Se aplica la transformación del padre al shader directamente
	shaderColor->setMat4("modelMatrix", transformacionPadre);

	// Se representa la órbita
	glBindVertexArray (this->VAO);
	glDrawArrays (GL_LINE_STRIP, 0, this->coordenadasPosiciones.size () / 6);

	// Se desvincula el VAO
	glBindVertexArray (0);
}

void Rastro::generarVAO ()
{
	// Se elimina los posibles objetos creados con anterioridad
	glDeleteVertexArrays (1, &(this->VAO));
	glDeleteBuffers (1, &(this->VBO));

	// Se generan objetos para representar los vértices que conforman el rastro
	glGenVertexArrays (1, &(this->VAO));
	glGenBuffers (1, &(this->VBO));

	// Se vincula el VAO creado para configurar los vértices del rastro
	glBindVertexArray (this->VAO);

	// Se vincula el VBO para trabajar con él
	glBindBuffer (GL_ARRAY_BUFFER, this->VBO);

	// Se indica cómo debe ser el buffer donde copiar todas las propiedades de cada vértice (dynamic = se modificará
	// muchas veces)
	glBufferData (GL_ARRAY_BUFFER, Rastro::maximoPosiciones * 6 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	// Se especifica cómo se encuentran distribuidas las coordenadas de cada vértice y se vinculan al atributo 0 de los
	// shaders
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), (void*)0);
	glEnableVertexAttribArray (0);

	// Se especifica cómo se encuentran distribuidas las propiedades de los colores de cada vértice y se vinculan al
	// atributo 1 de los shaders
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), (void*)(3 * sizeof (float)));
	glEnableVertexAttribArray (1);

	// Se desvinculan los objetos una vez finalizada la configuración
	glBindVertexArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}
