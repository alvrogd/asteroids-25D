#include "Astro.h"

#include <iostream>

bool Astro::representarOrbitas = true;

Astro::Astro (float radio, float periodoRotacion, Astro * padre, float periodoTraslacion, float distanciaPadre,
	Material material, Textura *textura1, Textura *textura2)
	// El tamaño de la esfera se determina en función del radio del astro (radio máximo = 100 km)
	: rastro (1.0f, 1.0f, 1.0f), material(material)
{
	// Se guardan los parámetros dados
	this->radio = radio;
	this->periodoRotacion = periodoRotacion;
	this->padre = padre;
	this->periodoTraslacion = periodoTraslacion;
	this->distanciaPadre = distanciaPadre;

	this->textura1 = textura1;

	// La segunda textura es opcional
	if (textura2 != NULL)
	{
		this->textura2 = textura2;
	}
	else
	{
		this->textura2 = NULL;
	}

	// Si existe un padre, el astro se registra como uno de sus hijos para ser dibujado
	if (this->padre != NULL)
	{
		this->padre->getHijos ().push_back (this);
	}

	// Los demás parámetros se inicializan a 0
	this->anguloRotacion = 0;
	this->anguloTraslacion = 0;

	// Sí se representará por defecto la órbita del astro
	this->representarRastro = true;
}

void Astro::actualizarEstado (float tiempoTranscurrido)
{
	// Se actualiza el ángulo de rotación teniendo en cuenta las horas que tarda en completar una y el tiempo
	// transcurrido (también en horas)
	this->anguloRotacion += 360.0f / this->periodoRotacion * (double) tiempoTranscurrido;
	this->anguloRotacion = fmod (this->anguloRotacion, 360.0f);

	// Se actualiza el ángulo de traslación teniendo en cuenta las horas que tarda en completar una y el tiempo
	// transcurrido (también en horas), siempre y cuando rote en torno a otro astro
	if (this->padre != NULL)
	{
		this->anguloTraslacion += 360.0f / this->periodoTraslacion * (double) tiempoTranscurrido;
		this->anguloTraslacion = fmod (this->anguloTraslacion, 360.0f);
	}

	// Se añade a la órbita una nueva posición
	this->rastro.insertarPosicion (this->distanciaPadre * cos (this->anguloTraslacion), 0.0f,
		this->distanciaPadre * sin (this->anguloTraslacion), 1.0f, 1.0f, 1.0f);

	// Se actualizan todos los hijos
	for (Astro *astro : this->hijos)
	{
		astro->actualizarEstado (tiempoTranscurrido);
	}
}

void Astro::dibujar (glm::mat4 transformacionPadre, Shader *shaderColor, Shader *shaderColorT1, Shader *shaderColorT2,
	Shader *shaderIluminacionT1, Shader *shaderIluminacionT2)
{
	Shader *shaderEscogido;

	// Es escoge el shader con el que representar el astro en función del número de texturas disponibles (para los
	// astros se emplean los shaders con iluminación)
	if (this->textura2 == NULL)
	{
		shaderEscogido = shaderIluminacionT1;
	}
	else
	{
		shaderEscogido = shaderIluminacionT2;
	}

	// Se activa el shader escogido
	shaderEscogido->usar ();

	// Se inicializan las matrices a emplear
	glm::mat4 traslacion = transformacionPadre; 
	glm::mat4 rotacion;
	glm::mat4 reescalado;
	glm::mat3 normalMatrix;

	// Si el astro rota en torno a otro
	if (this->padre != NULL)
	{
		// 3 -> Se aplica una traslación sobre el plano XOZ en función de su distancia al padre y el período de
		// traslación, empleando las ecuaciones del Movimiento Circular Uniforme (distancia máxima en un sentido = 32
		// UA)
		traslacion = glm::translate (traslacion, glm::vec3 (this->distanciaPadre * cosf (this->anguloTraslacion),
			0.0f, this->distanciaPadre * sinf (this->anguloTraslacion)));
	}

	// Se guarda como última posición el resultado de aplicar la traslación a un vector de coordenadas (0, 0, 0)
	glm::vec4 resultado = traslacion * glm::vec4 (0.0f, 0.0f, 0.0f, 1.0f);
	this->ultimaPosicion = glm::vec3(resultado.x, resultado.y, resultado.z);
	
	// 2 -> Se aplica una rotación alrededor del eje Y y en (0, 0, 0) en función de su período de rotación
	rotacion = glm::rotate (traslacion, glm::radians((float) this->anguloRotacion), glm::vec3 (0.0f, 1.0f, 0.0f));

	// 1 -> Se aplica un reescalado en función del radio del astro (radio máximo = 100 km)
	reescalado = glm::scale (rotacion, glm::vec3 (this->radio, this->radio, this->radio));

	// Se aplica la matriz de transformaciones al shader con iluminación
	shaderEscogido->setMat4("modelMatrix", reescalado);
	
	// Se calcula la matriz normal y se carga al shader
	normalMatrix = glm::transpose (glm::inverse (glm::mat3 (reescalado)));
	shaderEscogido->setMat3 ("normalMatrix", normalMatrix);

	// Se aplica el material al shader
	this->material.cargar (shaderEscogido);

	// Se cargan las texturas de la estrella al shader
	getTextura1 ()->activar (shaderEscogido, "Textura1", 0);
	
	if (shaderEscogido == shaderIluminacionT2)
	{
		getTextura2 ()->activar (shaderEscogido, "Textura2", 1);
	}

	// Se representa la esfera
	Forma::dibujarEsfera ();

	// Se representa la órbita del astro si se ha indicado
	if (Astro::representarOrbitas == true && this->representarRastro == true)
	{
		// La iluminación no afectará al rastro
		this->rastro.dibujar (transformacionPadre, shaderColor);
	}

	// Se representan todos los hijos
	for (Astro *astro : this->hijos)
	{
		// A cada hijo se le pasa la matriz resultado de aplicar, con las transformaciones del padre, la traslación
		// de este astro, para evitar que la rotación afecte a los hijos
		astro->dibujar (traslacion, shaderColor, shaderColorT1, shaderColorT2, shaderIluminacionT1,
			shaderIluminacionT2);
	}
}
