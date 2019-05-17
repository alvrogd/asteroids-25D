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
		// Posición aleatoria dentro del rango de warp dado para los objetos móviles
		glm::vec3 (fmod (std::rand (), (Movil::coordenadasWarp.x * 2.0f)) - Movil::coordenadasWarp.x, 0.0f,
			fmod (std::rand (), (Movil::coordenadasWarp.z * 2.0f)) - Movil::coordenadasWarp.z),
		// Velocidad aleatoria de hasta 1 unidad por frame (no se puede calcular mod 10 porque siempre resultaría una
		// velocidad nula
		glm::vec3 (fmod (std::rand (), 10.0f) * 0.1f, fmod (std::rand (), 10.0f) * 0.1f, fmod (std::rand (), 10.0f) *
			0.1f),
		// Aceleración y deceleración nulas
		glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3 (0.0f, 0.0f, 0.0f),
		// Rotación aleatoria de hasta 360º
		glm::vec3 (fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f)),
		// Sin correción de rotación del modelo
		glm::vec3 (0.0f, 0.0f, 0.0f))
{
	// Se reemplaza el radio de la hitbox por el máximo de los escalados generados, escalándolo en función del tamaño
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

	// El asteroide dará lugar a nuevos asteroides en dos ocasiones
	this->vidas = Asteroide::MAXIMO_VIDAS;
}

Asteroide::Asteroide (glm::vec3 escalado, glm::vec3 posicion, int vidas)
	// Escalado dado
	: Movil (escalado,
		// Modelo dado para los asteroides
		Asteroide::modelo,
		// Radio de la hitbox
		0.0f,
		// Posición dada
		posicion,
		// Velocidad aleatoria de hasta 1 unidad por frame (no se puede calcular mod 10 porque siempre resultaría una
		// velocidad nula; se aplica un multiplicador inversamente proporcional al número de vidas dado, regido por el
		// máximo de vidas de un asteroide, y limitado a x2
		// TODO optimizar
		glm::vec3 (fmod (std::rand (), 10.0f) * 0.1f * (1.0f + (float)(Asteroide::MAXIMO_VIDAS - vidas) /
				(float)Asteroide::MAXIMO_VIDAS),
			fmod (std::rand (), 10.0f) * 0.1f * (1.0f + (float)(Asteroide::MAXIMO_VIDAS - vidas) /
				(float)Asteroide::MAXIMO_VIDAS),
			fmod (std::rand (), 10.0f) * 0.1f * (1.0f + (float)(Asteroide::MAXIMO_VIDAS - vidas) /
				(float)Asteroide::MAXIMO_VIDAS)),
		// Aceleración y deceleración nulas
		glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3 (0.0f, 0.0f, 0.0f),
		// Rotación aleatoria de hasta 360º
		glm::vec3 (fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f), fmod (std::rand (), 360.0f)),
		// Sin correción de rotación del modelo
		glm::vec3 (0.0f, 0.0f, 0.0f))
{
	// Se reemplaza el radio de la hitbox por el máximo de los escalados generados, escalándolo en función del tamaño
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

	// Se guarda el número de vidas dado
	this->vidas = vidas;
}

Asteroide::~Asteroide ()
{
	// El asteroide se busca a sí mismo en el conjunto de asteroides en escena
	for (int i = 0; i < Asteroide::conjuntoAsteroides->size (); i++)
	{
		// Si se ha encontrado
		if (Asteroide::conjuntoAsteroides->at (i) == this)
		{
			std::cout << "encontrado" << std::endl;

			// Se elimina la partícula del conjunto de partículas
			Asteroide::conjuntoAsteroides->erase (Asteroide::conjuntoAsteroides->begin () + i);
		}
	}
}

void Asteroide::explotar ()
{
	// Si aún no se le han agotado las vidas
	if (this->vidas > 0)
	{
		// Se añaden dos asteroides "hijo" más pequeños que aparecerán en la posición del asteroide "padre",
		// reduciendo en 1 el número de vidas en comparación al padre
		Asteroide::conjuntoAsteroides->push_back (new Asteroide (getEscalado () * 0.6f, getPosicion (),
			this->vidas - 1));
		Asteroide::conjuntoAsteroides->push_back (new Asteroide (getEscalado () * 0.6f, getPosicion (),
			this->vidas - 1));
	}

	// Se genera una explosión en la posición del asteroide; en lugar de generar muchas partículas
	// individuales, se crea un conjunto de partículas para obtener un mejor rendimiento (color violeta claro)
	ConjuntoParticulas *conjuntoParticulas = new ConjuntoParticulas (this->getPosicion (),
		glm::vec4 (0.933f, 0.51f, 1.0f, 0.933f));
	ConjuntoParticulas::conjuntoConjuntoParticulas->push_back (conjuntoParticulas);
	conjuntoParticulas->generarExplosion (3.0f);

	// Se reproduce un efecto de explosión en la posición del asteroide
	Sonido::getSonido ()->reproducirExplosion (this->getPosicion ());

	// Se elimina el asteroide
	delete this;
}
