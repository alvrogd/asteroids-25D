#include "Asteroide.h"

// Para generar explosiones
#include "ConjuntoParticulas.h"
#include "Particula.h"
#include "Sonido.h"


Modelo *Asteroide::modelo = NULL;
std::vector<Asteroide*> * Asteroide::conjuntoAsteroides = NULL;

Asteroide::Asteroide ()
	// Escalado aleatorio entre x0.5 y x0.8
	: Movil (glm::vec3 (fmod (std::rand (), 3.0f) * 0.1f + 0.5f, fmod (std::rand (), 3.0f) * 0.3f + 0.5f, fmod (
		std::rand (), 3.0f) * 0.1f + 0.5f),
		// Modelo dado para los asteroides
		Asteroide::modelo,
		// Radio de la hitbox
		0.0f,
		// Posici�n aleatoria dentro del rango de warp dado para los objetos m�viles
		glm::vec3 (fmod (std::rand (), (Movil::coordenadasWarp.x * 2.0f)) - Movil::coordenadasWarp.x, 0.0f,
			fmod (std::rand (), (Movil::coordenadasWarp.z * 2.0f)) - Movil::coordenadasWarp.z),
		// Velocidad aleatoria de hasta 1 unidad por frame (no se puede calcular mod 10 porque siempre resultar�a una
		// velocidad nula
		glm::vec3 (fmod (std::rand (), 10.0f) * 0.1f, fmod (std::rand (), 10.0f) * 0.1f, fmod (std::rand (), 10.0f) *
			0.1f),
		// Aceleraci�n y deceleraci�n nulas
		glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3 (0.0f, 0.0f, 0.0f),
		// Rotaci�n aleatoria de hasta 360�
		glm::vec3 (fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f)),
		// Sin correci�n de rotaci�n del modelo
		glm::vec3 (0.0f, 0.0f, 0.0f))
{
	// Se reemplaza el radio de la hitbox por el m�ximo de los escalados generados, escal�ndolo en funci�n del tama�o
	// del modelo empleado
	glm::vec3 escalado = getEscalado ();
	float maximo = escalado.x;

	if (escalado.y > maximo)
	{
		maximo = escalado.y;
	}

	if (escalado.z > maximo)
	{
		maximo = escalado.z;
	}

	// 86.0f medido en blender
	setRadioHitbox (maximo * 43.0f);

	// El asteroide dar� lugar a nuevos asteroides en dos ocasiones
	this->vidas = Asteroide::MAXIMO_VIDAS;
}

Asteroide::Asteroide (glm::vec3 escalado, glm::vec3 posicion, int vidas)
	// Escalado dado
	: Movil (escalado,
		// Modelo dado para los asteroides
		Asteroide::modelo,
		// Radio de la hitbox
		0.0f,
		// Posici�n dada
		posicion,
		// Velocidad aleatoria de hasta 1 unidad por frame (no se puede calcular mod 10 porque siempre resultar�a una
		// velocidad nula; se aplica un multiplicador inversamente proporcional al n�mero de vidas dado, regido por el
		// m�ximo de vidas de un asteroide, y limitado a x2
		// TODO optimizar
		glm::vec3 (fmod (std::rand (), 10.0f) * 0.1f * (1.0f + (float)(Asteroide::MAXIMO_VIDAS - vidas) /
				(float)Asteroide::MAXIMO_VIDAS),
			fmod (std::rand (), 10.0f) * 0.1f * (1.0f + (float)(Asteroide::MAXIMO_VIDAS - vidas) /
				(float)Asteroide::MAXIMO_VIDAS),
			fmod (std::rand (), 10.0f) * 0.1f * (1.0f + (float)(Asteroide::MAXIMO_VIDAS - vidas) /
				(float)Asteroide::MAXIMO_VIDAS)),
		// Aceleraci�n y deceleraci�n nulas
		glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3 (0.0f, 0.0f, 0.0f),
		// Rotaci�n aleatoria de hasta 360�
		glm::vec3 (fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f)),
		// Sin correci�n de rotaci�n del modelo
		glm::vec3 (0.0f, 0.0f, 0.0f))
{
	// Se reemplaza el radio de la hitbox por el m�ximo de los escalados generados, escal�ndolo en funci�n del tama�o
	// del modelo empleado
	float maximo = escalado.x;

	if (escalado.y > maximo)
	{
		maximo = escalado.y;
	}

	if (escalado.z > maximo)
	{
		maximo = escalado.z;
	}

	// 86.0f medido en blender
	setRadioHitbox (maximo * 43.0f);

	// Se guarda el n�mero de vidas dado
	this->vidas = vidas;
}

Asteroide::~Asteroide ()
{
	// El asteroide se busca a s� mismo en el conjunto de asteroides en escena
	for (int i = 0; i < Asteroide::conjuntoAsteroides->size (); i++)
	{
		// Si se ha encontrado
		if (Asteroide::conjuntoAsteroides->at (i) == this)
		{
			std::cout << "encontrado" << std::endl;

			// Se elimina la part�cula del conjunto de part�culas
			Asteroide::conjuntoAsteroides->erase (Asteroide::conjuntoAsteroides->begin () + i);
		}
	}
}

void Asteroide::explotar ()
{
	// Si a�n no se le han agotado las vidas
	if (this->vidas > 0)
	{
		// Se a�aden dos asteroides "hijo" m�s peque�os que aparecer�n en la posici�n del asteroide "padre",
		// reduciendo en 1 el n�mero de vidas en comparaci�n al padre
		Asteroide::conjuntoAsteroides->push_back (new Asteroide (getEscalado () * 0.6f, getPosicion (),
			this->vidas - 1));
		Asteroide::conjuntoAsteroides->push_back (new Asteroide (getEscalado () * 0.6f, getPosicion (),
			this->vidas - 1));
	}

	// Se genera una explosi�n en la posici�n del asteroide; en lugar de generar muchas part�culas
	// individuales, se crea un conjunto de part�culas para obtener un mejor rendimiento (color violeta claro)
	ConjuntoParticulas *conjuntoParticulas = new ConjuntoParticulas (this->getPosicion (),
		glm::vec4 (0.933f, 0.51f, 1.0f, 0.933f));
	ConjuntoParticulas::conjuntoConjuntoParticulas->push_back (conjuntoParticulas);
	conjuntoParticulas->generarExplosion (3.0f);

	// Se reproduce un efecto de explosi�n en la posici�n del asteroide
	Sonido::getSonido ()->reproducirExplosion (this->getPosicion ());

	// Se elimina el asteroide
	delete this;
}
