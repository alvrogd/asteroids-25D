#include "Asteroide.h"

Modelo *Asteroide::modelo = NULL;

Asteroide::Asteroide ()
	// Escalado aleatorio entre x0.1 y x0.3
	: Movil (glm::vec3 (fmod (std::rand (), 3.0f) * 0.1f + 0.1f, fmod (std::rand (), 3.0f) * 0.1f + 0.1f, fmod (
		std::rand (), 3.0f) * 0.1f + 0.1f),
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
}
