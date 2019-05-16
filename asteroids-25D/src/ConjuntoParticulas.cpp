#include "ConjuntoParticulas.h"


std::vector<ConjuntoParticulas *> *ConjuntoParticulas::conjuntoConjuntoParticulas = NULL;


const double PI = 3.1415926535897;


ConjuntoParticulas::ConjuntoParticulas (glm::vec3 posicion, glm::vec4 color)
{
	// Se guardan los valores dados
	this->posicion = posicion;
	this->color = color;
}

ConjuntoParticulas::~ConjuntoParticulas ()
{
	// Se destruyen todas las partículas contenidas
	for (Particula *particula : this->particulas)
	{
		delete particula;
	}

	// El conjunto de partículas se busca a sí mismo en el conjunto de conjuntos de partículas en escena
	for (int i = 0; i < ConjuntoParticulas::conjuntoConjuntoParticulas->size (); i++)
	{
		// Si se ha encontrado
		if (ConjuntoParticulas::conjuntoConjuntoParticulas->at (i) == this)
		{
			// Se elimina la partícula del conjunto de partículas
			ConjuntoParticulas::conjuntoConjuntoParticulas->erase (
				ConjuntoParticulas::conjuntoConjuntoParticulas->begin () + i);
		}
	}
}

void ConjuntoParticulas::actualizarEstado (float tiempoTranscurrido)
{
	// Se modifica la posición de todas las partículas contenidas
	for (Particula *particula : this->particulas)
	{
		particula->actualizarEstado (tiempoTranscurrido);
	}

	// Se aumenta el tiempo que la partícula lleva activa
	this->edad += tiempoTranscurrido;
}

void ConjuntoParticulas::dibujar (glm::mat4 transformacionPadre, Shader * shader)
{
	// Se dibujan todas las partículas contenidas
	for (Particula *particula : this->particulas)
	{
		particula->dibujar(transformacionPadre, shader);
	}
}

void ConjuntoParticulas::generarExplosion (float vida)
{
	// Se generan 100 partículas aleatorias centradas en el punto de la explosión
	for (int i = 0; i < 100; i++)
	{
		// La partícula saldrá disparada en una dirección aleatoria
		glm::vec3 velocidad;

		// Para ello, se calcula inicialmente un vector unitario que defina el sentido del movimiento.
		//
		// Para una esfera:
		//	x = r * sin(theta) * cos(phi)
		//	y = r * sin(theta) * sin(phi)
		//	z = r * cos(theta)

		// TODO simplificar

		// Se calcula una phi aleatoria, en el rango [0, 2pi) con precisión de centésimas
		int aleatorio = std::rand () % (int)(2 * PI * 100);
		float phi = (float)aleatorio / 100.0f;

		// Se calcula un cos(theta) aleatorio, en el rango [-1, 1) con precisión de centésimas
		aleatorio = (std::rand () % 200) - 100;
		float cosTheta = (float)aleatorio / 100.f;


		/* NO VA
		// Se calcula una phi aleatoria, en el rango [0, 2pi) con precisión de centésimas
		float phi = fmod (std::rand (), 2 * PI);

		// Se calcula un cos(theta) aleatorio, en el rango [-1, 1) con precisión de centésimas
		float cosTheta = fmod (std::rand (), 2.0f) - 1.0f;
		*/


		// Se obtiene la theta correspondiente al cos(theta) generado
		float theta = acos (cosTheta);

		// Se generan las componentes del vector unitario
		velocidad.x = sinf (theta) * cosf (phi);
		velocidad.y = sinf (theta) * sinf (phi);
		velocidad.z = cosTheta;

		// Ahora se multiplica el vector unitario para darle una velocidad mayor
		velocidad *= 5;

		// Se genera una partícula en la escena:
		//  - Posición de la explosión
		//	- Velocidad generada
		//  - Color dado en la creación del conjunto
		//	- Duración especificada
		//	- Pertenece a un conjunto de partículas
		this->particulas.push_back (new Particula (this->posicion, velocidad, this->color, vida, true));
	}

	// Se guarda la duración dada para el conjunto y se inicializa a 0 su edad
	this->edad = 0;
	this->vida = vida;
}

bool ConjuntoParticulas::isMuerto () const
{
	// El conjunto habrá muerto si lleva presente en pantalla un tiempo igual o superior al indicado en su creación
	return(this->edad >= this->vida);
}
