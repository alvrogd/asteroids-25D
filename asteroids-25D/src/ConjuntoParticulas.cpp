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
	// Se destruyen todas las part�culas contenidas
	for (Particula *particula : this->particulas)
	{
		delete particula;
	}

	// El conjunto de part�culas se busca a s� mismo en el conjunto de conjuntos de part�culas en escena
	for (int i = 0; i < ConjuntoParticulas::conjuntoConjuntoParticulas->size (); i++)
	{
		// Si se ha encontrado
		if (ConjuntoParticulas::conjuntoConjuntoParticulas->at (i) == this)
		{
			// Se elimina la part�cula del conjunto de part�culas
			ConjuntoParticulas::conjuntoConjuntoParticulas->erase (
				ConjuntoParticulas::conjuntoConjuntoParticulas->begin () + i);
		}
	}
}

void ConjuntoParticulas::actualizarEstado (float tiempoTranscurrido)
{
	// Se modifica la posici�n de todas las part�culas contenidas
	for (Particula *particula : this->particulas)
	{
		particula->actualizarEstadoConjunto (tiempoTranscurrido);
	}

	// Se aumenta el tiempo que la part�cula lleva activa
	this->edad += tiempoTranscurrido;
}

void ConjuntoParticulas::dibujar (glm::mat4 transformacionPadre, Shader * shader)
{
	// Se activa la transparencia
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);

	// Se activa el shader dado
	shader->usar ();

	// Se establece el color de las part�culas en el shader:
	//	- El color se desvanecer� a medida que las part�culas se acerquen a su fin
	//	- El color tornar� hacia blanco a medida que las part�culas se acerquen a su fin (desactivado)
	glm::vec4 color = this->color;

	float porcentajeRestante = (this->vida - this->edad) * this->inversoVida;
	//float porcentajeAvanzado = 1 - porcentajeRestante;

	/*color.r += porcentajeAvanzado;
	color.g += porcentajeAvanzado;
	color.b += porcentajeAvanzado;*/
	color.a *= porcentajeRestante;

	shader->setVec4 ("Color", color);

	// Se representan todas las part�culas contenidas
	for (Particula *particula : this->particulas)
	{
		particula->dibujarConjunto (transformacionPadre, shader);
	}

	// Se desactiva la transparencia
	glDisable (GL_BLEND);
}

void ConjuntoParticulas::generarExplosion (float vida)
{
	// Se generan 60 part�culas aleatorias centradas en el punto de la explosi�n
	for (int i = 0; i < 60; i++)
	{
		// La part�cula saldr� disparada en una direcci�n aleatoria
		glm::vec3 velocidad;

		// Para ello, se calcula inicialmente un vector unitario que defina el sentido del movimiento.
		//
		// Para una esfera:
		//	x = r * sin(theta) * cos(phi)
		//	y = r * sin(theta) * sin(phi)
		//	z = r * cos(theta)
		
		// Se calcula una phi aleatoria, en el rango[0, 2pi) con precisi�n de cent�simas
		float phi = fmod ((double)std::rand (), 2.0f * PI);

		// Se calcula un cos(theta) aleatorio, en el rango [-1, 1) con precisi�n de cent�simas (no se genera un
		// flotante directamente porque, al trabajar con ellos, la p�rdida de precisi�n podr�a causar que "acos" no
		// funcione correctamente
		int aleatorio = (std::rand () % 200) - 100;
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
		//  - Color dado en la creaci�n del conjunto
		//	- Duraci�n especificada
		//	- Pertenece a un conjunto de part�culas
		this->particulas.push_back (new Particula (this->posicion, velocidad, this->color, vida, true));
	}

	// Se guarda la duraci�n dada para el conjunto y se inicializa a 0 su edad
	this->edad = 0;
	this->vida = vida;
	this->inversoVida = 1 / this->vida;
}

bool ConjuntoParticulas::isMuerto () const
{
	// El conjunto habr� muerto si lleva presente en pantalla un tiempo igual o superior al indicado en su creaci�n
	return(this->edad >= this->vida);
}
