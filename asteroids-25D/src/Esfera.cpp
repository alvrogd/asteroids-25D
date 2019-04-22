#include "Esfera.h"

Esfera::Esfera (float radio, int sectores, int stacks)
{
	// Se guardan los atributos dados
	this->radio = radio;
	this->sectores = sectores;
	this->stacks = stacks;

	construirEsfera ();
}

Esfera::~Esfera ()
{
	// Se destruye el VAO
	glDeleteVertexArrays (1, &(this->VAO));
}

void Esfera::construirEsfera ()
{
	// http://www.songho.ca/opengl/gl_sphere.html

	// Se generan los v�rtices de la esfera
	generarVertices ();

	// Se generan los �ndices de los tri�ngulos que conforman la esfera
	generarIndices ();
	
	// Se insertan en un �nico array los v�rtices y sus propiedades
	juntarVerticesPropiedades ();

	// Se genera un VAO con el que dibujar la esfera
	generarVAO ();
}

void Esfera::generarVertices ()
{
	// Se libera la memoria de anteriores construcciones
	std::vector<float> ().swap (vertices);
	std::vector<float> ().swap (normales);
	std::vector<float> ().swap (texturasCoordenadas);

	// Posiciones del v�rtice iterado
	float x, y, z, xy;
	// Normales del v�rtice iterado
	float nX, nY, nZ, longitudInversa = 1.0f / radio;
	// Coordenadas de textura del v�rtice iterado
	float s, t;

	// Paso entre sectores
	float pasoSectores = 2 * (float)M_PI / sectores;
	// Paso entre stacks
	float pasoStacks = (float)M_PI / stacks;

	// �ngulos del v�rtice iterado
	float anguloSector, anguloStack;

	// Contadores
	int i, j;


	// Se recorre la latitud de la esfera
	for (i = 0; i <= stacks; i++)
	{
		// Se recorren desde PI/2 (arriba) hasta -PI/2 (abajo)
		anguloStack = (float)M_PI / 2 - i * pasoStacks;

		// Valor auxiliar para calcular las coordenadas X e Y (vector situado en el plano XYO); r * cos(u) 
		xy = radio * cosf (anguloStack);

		// Coordenada Z; r * sin(u)
		z = radio * sinf (anguloStack);

		// Se a�aden (sectores + 1) v�rtices por stack; el primer y �ltimo v�rtice tienen la misma posici�n y normal pero
		// distinta coordenadas de texturas
		for (j = 0; j <= sectores; j++)
		{
			// Se recorren desde 0 hasta 2*PI
			anguloSector = j * pasoSectores;

			// Coordenadas X e Y del v�rtice
			x = xy * cosf (anguloSector);
			y = xy * sinf (anguloSector);

			// Se guardan las coordenadas del v�rtice
			vertices.push_back (x);
			vertices.push_back (y);
			vertices.push_back (z);

			// Se calculan las normales (en una esfera se obtienen directamente de las coordenadas) y se normalizan (se
			// multiplican por la longitud inversa)
			nX = x * longitudInversa;
			nY = y * longitudInversa;
			nZ = z * longitudInversa;

			// Se guardan
			normales.push_back (nX);
			normales.push_back (nY);
			normales.push_back (nZ);

			// Se calculan las coordenadas de las texturas del v�rtice (s, t), en el rango [0, 1]
			s = (float)j / sectores;
			t = (float)i / stacks;

			// Se almacenan
			texturasCoordenadas.push_back (s);
			texturasCoordenadas.push_back (t);
		}
	}
}

void Esfera::generarIndices ()
{
	// Variables auxiliares para los �ndices iterados para conformar un tri�ngulo
	int k1, k2;

	// Contadores
	int i, j;


	// Se recorren todos los stacks
	for (i = 0; i < stacks; i++)
	{
		// V�rtice inicial (superior) del stack iterado
		k1 = i * (sectores + 1);
		// V�rtice inicial del siguiente stack (v�rtice inferior del stack iterado)
		k2 = k1 + sectores + 1;

		// Se recorren todos los sectores
		for (j = 0; j < sectores; j++, k1++, k2++)
		{
			// Cada sector cuenta con dos tri�ngulos, excepto el primero y el �ltimo

			// Si no es el primer stack
			if (i != 0)
			{
				// Tri�ngulo superior/izquierdo
				indices.push_back (k1);
				indices.push_back (k2);
				indices.push_back (k1 + 1);
			}

			// Si no es el �ltimo stack
			if (i != stacks - 1)
			{
				indices.push_back (k1 + 1);
				indices.push_back (k2);
				indices.push_back (k2 + 1);
			}
		}
	}
}

void Esfera::juntarVerticesPropiedades ()
{
	// N�mero de v�rtices creados para la esfera
	std::size_t total = vertices.size ();
	// Contadores
	std::size_t i, j;


	// Se libera la memoria de las anteriores construcciones
	std::vector<float> ().swap (verticesIntercalados);

	// Para cada v�rtice generado
	for (i = 0, j = 0; i < total; i += 3, j += 2)
	{
		// Se a�aden sus coordenadas
		verticesIntercalados.push_back (vertices[i]);
		verticesIntercalados.push_back (vertices[i + 1]);
		verticesIntercalados.push_back (vertices[i + 2]);

		// Sus normales para el color
		verticesIntercalados.push_back (normales[i]);
		verticesIntercalados.push_back (normales[i + 1]);
		verticesIntercalados.push_back (normales[i + 2]);

		// Sus coordenadas de texturas
		verticesIntercalados.push_back (texturasCoordenadas[j]);
		verticesIntercalados.push_back (texturasCoordenadas[j + 1]);
	}
}

void Esfera::generarVAO ()
{
	// Se elimina el posible VAO creado con anterioridad
	glDeleteVertexArrays (1, &VAO);

	// Se generan objetos para representar los v�rtices que conforman la esfera
	glGenVertexArrays (1, &VAO);
	glGenBuffers (1, &VBO);
	glGenBuffers (1, &EBO);

	// Se vincula el VAO creado para configurar los v�rtices de la esfera
	glBindVertexArray (VAO);

	// Se vincula el VBO para trabajar con �l
	glBindBuffer (GL_ARRAY_BUFFER, VBO);

	// Se copian todas las propiedades de cada v�rtice al buffer
	glBufferData (GL_ARRAY_BUFFER, verticesIntercalados.size () * sizeof (float), &verticesIntercalados[0],
		GL_STATIC_DRAW);

	// Se vincula ahora el EBO para indicar los �ndices de los v�rtices a representar
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Se copia la informaci�n directamente del array de v�rtices
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, indices.size () * sizeof (int), &indices[0], GL_STATIC_DRAW);

	// Se especifica c�mo se encuentran distribuidas las coordenadas de cada v�rtice y se vinculan al atributo 0 de los
	// shaders
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)0);
	glEnableVertexAttribArray (0);

	// Se especifica c�mo se encuentran distribuidas las propiedades de los colores de cada v�rtice y se vinculan al
	// atributo 1 de los shaders
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(3 * sizeof (float)));
	glEnableVertexAttribArray (1);

	// Se especifica c�mo se encuentran distribuidas las coordenadas de texturas de cada v�rtice y se vinculan al
	// atributo 2 de los shaders
	glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(6 * sizeof (float)));
	glEnableVertexAttribArray (2);

	// Se desvincula el VAO una vez finalizada su configuraci�n
	glBindVertexArray (0);

	// Se libera la memoria de los buffers
	glDeleteBuffers (1, &VBO);
	glDeleteBuffers (1, &EBO);
}

void Esfera::dibujar () const
{
	// Se vincula el VAO asignado a la esfera
	glBindVertexArray (VAO);

	// Se representan todos los tri�ngulos que la componen
	glDrawElements (GL_TRIANGLES, indices.size (), GL_UNSIGNED_INT, 0);

	// Se desvincula el VAO
	glBindVertexArray (0);
}
