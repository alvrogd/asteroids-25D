#include "Asteroide.h"

Modelo *Asteroide::modelo = NULL;

Asteroide::Asteroide ()
// Escalado de x0.1
	: Movil (glm::vec3 (0.1f, 0.1f, 0.1f),
		// Modelo dado para los asteroides
		Asteroide::modelo,
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
}
