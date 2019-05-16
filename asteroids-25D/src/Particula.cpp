#include "Particula.h"

// Se represent�n como esferas
#include "Forma.h"

// Funciones matem�ticas
#include <cmath>


const double PI = 3.1415926535897;


std::vector<Particula *> *Particula::conjuntoParticulas = NULL;


Particula::Particula (glm::vec3 posicion, glm::vec3 velocidad, glm::vec4 color, float vida)
{
	// Se guardan los valores dados
	this->posicion = posicion;
	this->velocidad = velocidad;
	this->color = color;
	this->vida = vida;

	// Inicialmente, la edad se inicializa a 0 dado que acaba de generarse
	this->edad = 0.0f;
}

Particula::~Particula ()
{
	// La part�cula se busca a s� misma en el conjunto de part�culas en escena
	for (int i = 0; i < Particula::conjuntoParticulas->size (); i++)
	{
		// Si se ha encontrado
		if (Particula::conjuntoParticulas->at (i) == this)
		{
			// Se elimina la part�cula del conjunto de part�culas
			Particula::conjuntoParticulas->erase (Particula::conjuntoParticulas->begin () + i);
		}
	}
}

void Particula::actualizarEstado (float tiempoTranscurrido)
{
	// Se modifica la posici�n de la part�cula en funci�n de la velocidad registrada
	this->posicion.x += this->velocidad.x;
	this->posicion.y += this->velocidad.y;
	this->posicion.z += this->velocidad.z;

	// Se aumenta el tiempo que la part�cula lleva activa
	this->edad += tiempoTranscurrido;
}

void Particula::dibujar (glm::mat4 transformacionPadre, Shader * shader)
{
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

	// Se activa el shader dado
	shader->usar ();

	// Se establece el color de la part�cula en el shader
	shader->setVec4 ("Color", this->color);

	// Se inicializan las matrices a emplear
	glm::mat4 transformacion = transformacionPadre;

	// 3 -> Se aplica la traslaci�n guardada
	transformacion = glm::translate (transformacion, getPosicion ());

	/*// 2 -> Se aplica la rotaci�n guardada sobre cada uno de los ejes
	glm::vec3 rot = getRotacion ();
	glm::vec3 correcion = getCorreccionRotacion ();
	transformacion = glm::rotate (transformacion, glm::radians (rot.x + correcion.x), glm::vec3 (1.0f, 0.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (rot.y + correcion.y), glm::vec3 (0.0f, 1.0f, 0.0f));
	transformacion = glm::rotate (transformacion, glm::radians (rot.z + correcion.z), glm::vec3 (0.0f, 0.0f, 1.0f));*/

	// 1 -> Se aplica un escalado est�ndar para todas las part�culas
	transformacion = glm::scale (transformacion, glm::vec3(1.0f, 1.0f, 1.0f));

	// Se aplica la transformaci�n calculada a la matriz del modelo del shader
	shader->setMat4 ("modelMatrix", transformacion);

	// Se representa el disparo
	Forma::dibujarEsfera ();


	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Particula::generarExplosion (glm::vec3 posicion)
{
	// Se generan 200 part�culas aleatorias centradas en el punto de la explosi�n
	for (int i = 0; i < 100; i++)
	{
		// La part�cula saldr� disparada en una direcci�n aleatoria
		glm::vec3 velocidad;

		// Para ello, se calcula inicialmente un vector unitario que defina el sentido del movimiento.
		//
		//
		// Para una esfera:
		//	x = r * sin(theta) * cos(phi)
		//	y = r * sin(theta) * sin(phi)
		//	z = r * cos(theta)

		// TODO simplificar

		// Se calcula una phi aleatoria, en el rango [0, 2pi) con precisi�n de cent�simas
		int aleatorio = std::rand () % (int)(2 * PI * 100);
		float phi = (float)aleatorio / 100.0f;

		// Se calcula un cos(theta) aleatorio, en el rango [-1, 1) con precisi�n de cent�simas
		aleatorio = (std::rand () % 200) - 100;
		float cosTheta = (float)aleatorio / 100.f;

		// Se obtiene la theta correspondiente al cos(theta) generado
		float theta = acos (cosTheta);

		// Se generan las componentes del vector unitario
		velocidad.x = sinf (theta) * cosf (phi);
		velocidad.y = sinf (theta) * sinf (phi);
		velocidad.z = cosTheta;

		// Ahora se multiplica el vector unitario para darle una velocidad mayor
		velocidad *= 5;

		// Se genera una part�cula en la escena:
		//  - Posici�n de la explosi�n
		//	- Velocidad generada
		//  - Color de los disparos
		//	- Tres segundos de duraci�n
		Particula::conjuntoParticulas->push_back (new Particula (posicion, velocidad,
			glm::vec4 (1.0f, 0.0f, 1.0f, 1.0f), 3.0f));
	}
}

bool Particula::isMuerta () const
{
	// La part�cula habr� muerto si lleva presente en pantalla un tiempo igual o superior al indicado en su creaci�n
	return(this->edad >= this->vida);
}
