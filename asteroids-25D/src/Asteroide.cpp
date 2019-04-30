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
}
