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

	// Al no indicarse, se asume que la part�cula no pertenece a un conjunto de part�culas
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
	// La part�cula se busca a s� misma en el conjunto de part�culas en escena; tan solo se habr� a�adido a �l si no
	// pertenece a un conjunto de part�culas
	if (!this->perteneceConjunto)
	{
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

void Particula::actualizarEstadoConjunto (float tiempoTranscurrido)
{
	// Se modifica la posici�n de la part�cula en funci�n de la velocidad registrada
	this->posicion.x += this->velocidad.x;
	this->posicion.y += this->velocidad.y;
	this->posicion.z += this->velocidad.z;

	// Al pertenecer a un conjunto, no es necesario que la part�cula sepa su edad individual
}

void Particula::dibujar (glm::mat4 transformacionPadre, Shader * shader)
{
	// Se activa la transparencia
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);

	// Se activa el shader dado
	shader->usar ();

	// Se establece el color de la part�cula en el shader:
	//	- El color se desvanecer� a medida que la part�cula se acerque a su fin
	//	- El color tornar� hacia blanco a medida que la part�cula se acerque a su fin
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

	// 2 -> Se aplica la traslaci�n guardada
	transformacion = glm::translate (transformacion, getPosicion ());

	// 1 -> Se aplica un escalado est�ndar para todas las part�culas
	transformacion = glm::scale (transformacion, glm::vec3(1.0f, 1.0f, 1.0f));

	// Se aplica la transformaci�n calculada a la matriz del modelo del shader
	shader->setMat4 ("modelMatrix", transformacion);

	// Se representa la part�cula
	Forma::dibujarEsfera ();

	// Se desactiva la transparencia
	glDisable (GL_BLEND);
}

void Particula::dibujarConjunto (glm::mat4 transformacionPadre, Shader * shader)
{
	// Como la part�cula pertenece a un conjunto, este ya habr� activado el "blend", el shader dado y habr�
	// establecido en el shader el color degradado de la part�cula
	
	// Se inicializan las matrices a emplear
	glm::mat4 transformacion = transformacionPadre;

	// 2 -> Se aplica la traslaci�n guardada
	transformacion = glm::translate (transformacion, getPosicion ());

	// 1 -> Se aplica un escalado est�ndar para todas las part�culas
	transformacion = glm::scale (transformacion, glm::vec3 (1.0f, 1.0f, 1.0f));

	// Se aplica la transformaci�n calculada a la matriz del modelo del shader
	shader->setMat4 ("modelMatrix", transformacion);

	// Se representa la part�cula
	Forma::dibujarEsfera ();
}

void Particula::generarExplosion (glm::vec3 posicion)
{
	// Se generan 100 part�culas aleatorias centradas en el punto de la explosi�n
	for (int i = 0; i < 100; i++)
	{
		// La part�cula saldr� disparada en una direcci�n aleatoria
		glm::vec3 velocidad;

		// Para ello, se calcula inicialmente un vector unitario que defina el sentido del movimiento.
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


		/* NO VA
		// Se calcula una phi aleatoria, en el rango [0, 2pi) con precisi�n de cent�simas
		float phi = fmod (std::rand (), 2 * PI);

		// Se calcula un cos(theta) aleatorio, en el rango [-1, 1) con precisi�n de cent�simas
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
