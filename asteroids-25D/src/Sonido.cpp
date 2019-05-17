#include "Sonido.h"

#include <sstream>
#include <string>


Sonido *Sonido::sonido = NULL;


Sonido * Sonido::getSonido ()
{
	// Si la instancia singleton no se ha inicializado, se crea
	if (Sonido::sonido == NULL)
	{
		Sonido::sonido = new Sonido ();
	}

	return Sonido::sonido;
}

void Sonido::actualizar (glm::vec3 posicionOyente, glm::vec3 direccionOyente)
{
	// Se actualiza, en el reproductor 3D, la posición y dirección del oyente con los valores dados
	Sonido::getSonido ()->getSonido3D ()->setListenerPosition (irrklang::vec3df (posicionOyente.x, posicionOyente.y,
		posicionOyente.z), irrklang::vec3df (direccionOyente.x, direccionOyente.y, direccionOyente.z));
}

void Sonido::reproducirExplosion (glm::vec3 posicion)
{
	// Se reproduce un efecto de explosión:
	//  - Sonido aleatorio entre los 4 disponibles
	//	- Posición dada
	//	- Sin repetición
	//	- Reproducción inmediata
	//	- Sin seguimiento para evitar tener que liberar manualmente los recursos que haya usado
	//	- Sin efectos de sonido
	std::ostringstream fichero;
	fichero << "infinity_war_disintegration";
	fichero << std::rand () % 4;
	fichero << ".ogg";

	this->sonido3D->play3D (fichero.str().c_str(), irrklang::vec3df (posicion.x, posicion.y, posicion.z),
		GL_FALSE, GL_FALSE, GL_TRUE);
}

Sonido::Sonido ()
{
	// Se crean los reproductores de sonido necesarios
	this->sonido2D = irrklang::createIrrKlangDevice ();
	this->sonido3D = irrklang::createIrrKlangDevice ();

	// Se reduce el volumen predeterminado de los sonidos 2D
	this->sonido2D->setSoundVolume (0.3f);

	// La distancia mínima para que un sonido 3D deje de incrementarse es 400.0f
	this->sonido3D->setDefault3DSoundMinDistance (400.0f);
	// Se pone al máximo el volumen predeterminado de los sonidos 3D
	this->sonido3D->setSoundVolume (1.0f);
}


Sonido::~Sonido ()
{
	// Se destruyen los reproductores de sonido creados
	this->sonido2D->drop ();
	this->sonido3D->drop ();
}
