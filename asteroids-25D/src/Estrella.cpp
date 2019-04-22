#include "Estrella.h"

Estrella::Estrella (float radio, float periodoRotacion, Astro * padre, float periodoTraslacion, float distanciaPadre,
	Material material, Textura *textura1, Textura *textura2, PuntoLuz luz)
	: Astro(radio, periodoRotacion, padre, periodoTraslacion, distanciaPadre, material, textura1, textura2), luz(luz)
{
}

void Estrella::dibujar (glm::mat4 transformacionPadre, Shader *shaderColor, Shader *shaderColorT1, Shader
	*shaderColorT2, Shader *shaderIluminacionT1, Shader *shaderIluminacionT2)
{
	Shader *shaderEscogido;

	// Es escoge el shader con el que representar la estrella en funci�n del n�mero de texturas disponibles (para las
	// estrellas se emplean los shaders sin iluminaci�n)
	if (getTextura2 () == NULL)
	{
		shaderEscogido = shaderColorT1;
	}
	else
	{
		shaderEscogido = shaderColorT2;
	}

	// Se activa el shader escogido
	shaderEscogido->usar ();

	// Se inicializan las matrices a emplear
	glm::mat4 traslacion = transformacionPadre;
	glm::mat4 rotacion;
	glm::mat4 reescalado;
	glm::mat3 normalMatrix;

	// Si el astro rota en torno a otro
	if (getPadre () != NULL)
	{
		// 3 -> Se aplica una traslaci�n sobre el plano XOZ en funci�n de su distancia al padre y el per�odo de
		// traslaci�n, empleando las ecuaciones del Movimiento Circular Uniforme (distancia m�xima en un sentido = 32
		// UA)
		traslacion = glm::translate (traslacion, glm::vec3 (getDistanciaPadre () * cosf (getAnguloTraslacion ()),
			0.0f, getDistanciaPadre () * sinf (getAnguloTraslacion ())));
	}

	// Se guarda como �ltima posici�n el resultado de aplicar la traslaci�n a un vector de coordenadas (0, 0, 0)
	glm::vec4 resultado = traslacion * glm::vec4 (0.0f, 0.0f, 0.0f, 1.0f);
	setUltimaPosicion(glm::vec3 (resultado.x, resultado.y, resultado.z));

	// 2 -> Se aplica una rotaci�n alrededor del eje Y y en (0, 0, 0) en funci�n de su per�odo de rotaci�n
	rotacion = glm::rotate (traslacion, glm::radians ((float)getAnguloRotacion ()), glm::vec3 (0.0f, 1.0f, 0.0f));

	// 1 -> Se aplica un reescalado en funci�n del radio del astro (radio m�ximo = 100 km)
	reescalado = glm::scale (rotacion, glm::vec3 (getRadio (), getRadio (), getRadio ()));

	// Se aplica la matriz de transformaciones al shader
	shaderEscogido->setMat4 ("modelMatrix", reescalado);

	// Se establece como color de output la componente difusa de la luz
	shaderEscogido->setVec3 ("Color", this->luz.getDifusa ());

	// Se cargan las texturas de la estrella al shader
	getTextura1 ()->activar (shaderEscogido, "Textura1", 0);

	if (shaderEscogido == shaderColorT2)
	{
		getTextura2 ()->activar (shaderEscogido, "Textura2", 1);
	}

	// Se representa la esfera
	Forma::dibujarEsfera ();

	// Se aplican en los shaders de iluminaci�n la luz emitida; a esta le afectar� la traslaci�n llevada a cabo sobre
	// la estrella
	shaderIluminacionT1->usar ();
	this->luz.cargar (shaderIluminacionT1, traslacion);
	shaderIluminacionT2->usar ();
	this->luz.cargar (shaderIluminacionT2, traslacion);

	// Se representa la �rbita del astro si se ha indicado
	if (Astro::representarOrbitas == true && isRepresentarRastro () == true)
	{
		// La iluminaci�n no afectar� al rastro
		getRastro ().dibujar (transformacionPadre, shaderColor);
	}

	// Se representan todos los hijos
	for (Astro *astro : getHijos ())
	{
		// A cada hijo se le pasa la matriz resultado de aplicar, con las transformaciones del padre, la traslaci�n
		// de este astro, para evitar que la rotaci�n afecte a los hijos
		astro->dibujar (traslacion, shaderColor, shaderColorT1, shaderColorT2, shaderIluminacionT1,
			shaderIluminacionT2);
	}
}
