#include "Disparo.h"

std::vector<Disparo *> *Disparo::conjuntoDisparos = NULL;


Disparo::Disparo (glm::vec3 posicion, glm::vec3 velocidad, glm::vec3 rotacion)
	: Movil(
		// Alargamiento en el eje X
		glm::vec3(1.0f, 1.0f, 5.0f),
		// No se representarán a partir de un modelo
		NULL,
		// Acorde al escalado
		5.0f,
		// Posición y velocidad dadas
		posicion,
		velocidad,
		// Sin aceleración y deceleración
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 (0.0f, 0.0f, 0.0f),
		// Rotación dada
		rotacion,
		// Sin corrección de la rotación de la representación
		glm::vec3 (0.0f, 0.0f, 0.0f))
{
	// Se registra el disparo creado en el conjunto de disparos activos de la escena
	Disparo::conjuntoDisparos->push_back (this);

	// Se crea una luz rosa que el disparo emitirá
	this->luz = new PuntoLuz (getPosicion (), 1.0f, 0.027, 0.0028, glm::vec3 (1.0f, 0.0f, 1.0f), glm::vec3 (1.0f, 0.0f,
		1.0f), glm::vec3 (1.0f, 0.0f, 1.0f));
}

Disparo::~Disparo ()
{
	// El disparo se busca a sí mismo en el conjunto de disparos en escena
	for (int i = 0; i < Disparo::conjuntoDisparos->size (); i++)
	{
		// Si se ha encontrado
		if (Disparo::conjuntoDisparos->at (i) == this)
		{
			std::cout << "disparo encontrado" << std::endl;

			// Se elimina el disparo del conjunto de disparos
			Disparo::conjuntoDisparos->erase (Disparo::conjuntoDisparos->begin () + i);
		}
	}

	// Se elimina la luz emitida
	delete this->luz;
}

void Disparo::actualizarEstado (float tiempoTranscurrido)
{
	glm::vec3 & posicion = getPosicion ();
	glm::vec3 & velocidad = getVelocidad ();

	// Inicialmente, se asume que el disparo no efectuará un warp
	bool isWarp = false;

	// Se modifica la posición del móvil en función de la velocidad registrada
	posicion.x += velocidad.x;
	posicion.z += velocidad.z;

	// Si alguna de las coordenadas ha superado el warp establecido, se "invierte" para mover el móvil al lado opuesto
	if (fabs (posicion.x) > Movil::coordenadasWarp.x)
	{
		posicion.x *= -1;
		isWarp = true;
	}

	if (fabs (posicion.z) > Movil::coordenadasWarp.z)
	{
		posicion.z *= -1;
		isWarp = true;
	}

	// Se reduce la velocidad del móvil en cada coordenada
	velocidad.x *= 1 - getCoefDeceleracion ().x;
	velocidad.z *= 1 - getCoefDeceleracion ().z;

	// Si el disparo ha efecutado un warp, se reduce en uno el número de warps que podrá realizar
	if (isWarp)
	{
		this->warpsRestantes--;
	}

	// Se actualiza la posición de la luz emitida a la nueva posición calculada para el disparo
	this->luz->setPosicion (posicion);
}

void Disparo::dibujar (glm::mat4 transformacionPadre, Shader * shader)
{
	// Se activa el shader dado
	shader->usar ();

	// Se establece un color morado en el shader dado
	shader->setVec3 ("Color", glm::vec3 (1.0f, 0.0f, 1.0f));

	// Se inicializan las matrices a emplear
	glm::mat4 transformacion = transformacionPadre;

	// 3 -> Se aplica la traslación guardada
	transformacion = glm::translate (transformacion, getPosicion ());

	// 2 -> Se aplica la rotación guardada sobre cada uno de los ejes
	glm::vec3 rot = getRotacion ();
	glm::vec3 correcion = getCorreccionRotacion ();
	transformacion = glm::rotate (transformacion, glm::radians (rot.x + correcion.x), glm::vec3 (1.0f, 0.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (rot.y + correcion.y), glm::vec3 (0.0f, 1.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (rot.z + correcion.z), glm::vec3 (0.0f, 0.0f, 1.0f));

	// 1 -> Se aplica el escalado guardado
	transformacion = glm::scale (transformacion, getEscalado ());

	// Se aplica la transformación calculada a la matriz del modelo del shader
	shader->setMat4 ("modelMatrix", transformacion);

	// Se representa el disparo
	Forma::dibujarEsfera ();
}
