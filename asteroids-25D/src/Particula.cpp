#include "Particula.h"

// Se representán como esferas
#include "Forma.h"

// Funciones matemáticas
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

	// Al no indicarse, se asume que la partícula no pertenece a un conjunto de partículas
	this->perteneceConjunto = false;
}

Particula::Particula (glm::vec3 posicion, glm::vec3 velocidad, glm::vec4 color, float vida, bool perteneceConjunto)
{
	// Se guardan los valores dados
	this->posicion = posicion;
	this->velocidad = velocidad;
	this->color = color;
	this->vida = vida;
	this->perteneceConjunto = perteneceConjunto;

	// Inicialmente, la edad se inicializa a 0 dado que acaba de generarse
	this->edad = 0.0f;
}

Particula::~Particula ()
{
	// La partícula se busca a sí misma en el conjunto de partículas en escena; tan solo se habrá añadido a él si no
	// pertenece a un conjunto de partículas
	if (!this->perteneceConjunto)
	{
		for (int i = 0; i < Particula::conjuntoParticulas->size (); i++)
		{
			// Si se ha encontrado
			if (Particula::conjuntoParticulas->at (i) == this)
			{
				// Se elimina la partícula del conjunto de partículas
				Particula::conjuntoParticulas->erase (Particula::conjuntoParticulas->begin () + i);
			}
		}
	}
}

void Particula::actualizarEstado (float tiempoTranscurrido)
{
	// Se modifica la posición de la partícula en función de la velocidad registrada
	this->posicion.x += this->velocidad.x;
	this->posicion.y += this->velocidad.y;
	this->posicion.z += this->velocidad.z;

	// Se aumenta el tiempo que la partícula lleva activa
	this->edad += tiempoTranscurrido;
}

void Particula::actualizarEstadoConjunto (float tiempoTranscurrido)
{
	// Se modifica la posición de la partícula en función de la velocidad registrada
	this->posicion.x += this->velocidad.x;
	this->posicion.y += this->velocidad.y;
	this->posicion.z += this->velocidad.z;

	// Al pertenecer a un conjunto, no es necesario que la partícula sepa su edad individual
}

void Particula::dibujar (glm::mat4 transformacionPadre, Shader * shader)
{
	// Se activa la transparencia
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);

	// Se activa el shader dado
	shader->usar ();

	// Se establece el color de la partícula en el shader:
	//	- El color se desvanecerá a medida que la partícula se acerque a su fin
	//	- El color tornará hacia blanco a medida que la partícula se acerque a su fin
	glm::vec4 color = this->color;

	float porcentajeRestante = (this->vida - this->edad) / this->vida;
	float porcentajeAvanzado = 1 - porcentajeRestante;

	color.r += porcentajeAvanzado;
	color.g += porcentajeAvanzado;
	color.b += porcentajeAvanzado;
	color.a *= porcentajeRestante;

	shader->setVec4 ("Color", color);

	// Se inicializan las matrices a emplear
	glm::mat4 transformacion = transformacionPadre;

	// 2 -> Se aplica la traslación guardada
	transformacion = glm::translate (transformacion, getPosicion ());

	// 1 -> Se aplica un escalado estándar para todas las partículas
	transformacion = glm::scale (transformacion, glm::vec3(1.0f, 1.0f, 1.0f));

	// Se aplica la transformación calculada a la matriz del modelo del shader
	shader->setMat4 ("modelMatrix", transformacion);

	// Se representa la partícula
	Forma::dibujarEsfera ();

	// Se desactiva la transparencia
	glDisable (GL_BLEND);
}

void Particula::dibujarConjunto (glm::mat4 transformacionPadre, Shader * shader)
{
	// Como la partícula pertenece a un conjunto, este ya habrá activado el "blend", el shader dado y habrá
	// establecido en el shader el color degradado de la partícula
	
	// Se inicializan las matrices a emplear
	glm::mat4 transformacion = transformacionPadre;

	// 2 -> Se aplica la traslación guardada
	transformacion = glm::translate (transformacion, getPosicion ());

	// 1 -> Se aplica un escalado estándar para todas las partículas
	transformacion = glm::scale (transformacion, glm::vec3 (1.0f, 1.0f, 1.0f));

	// Se aplica la transformación calculada a la matriz del modelo del shader
	shader->setMat4 ("modelMatrix", transformacion);

	// Se representa la partícula
	Forma::dibujarEsfera ();
}

void Particula::generarExplosion (glm::vec3 posicion)
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
		//  - Color de los disparos
		//	- Tres segundos de duración
		Particula::conjuntoParticulas->push_back (new Particula (posicion, velocidad,
			glm::vec4 (1.0f, 0.0f, 1.0f, 1.0f), 3.0f));
	}
}

bool Particula::isMuerta () const
{
	// La partícula habrá muerto si lleva presente en pantalla un tiempo igual o superior al indicado en su creación
	return(this->edad >= this->vida);
}
