#ifndef CONTROLADOR_H
#define CONTROLADOR_H

// Librer�as de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para modificar la matriz de visionado
#include "Shader.h"

// Para almacenar referencias a los objetos presentes en la escena
#include "Asteroide.h"
#include "Nave.h"
#include "ConjuntoParticulas.h"
#include "Particula.h"


class Controlador
{
public:
	/* Atributos */

	// Tama�o de la ventana
	static int *wWidth;
	static int *wHeight;
	
	// Relaci�n de aspecto de la ventana
	static float *relacionAspecto;

	// Asteroides en la escena
	static std::vector<Asteroide *> *asteroides;

	// Part�culas en la escena
	static std::vector<Particula *> *particulas;
	static std::vector<ConjuntoParticulas *> *conjuntosParticulas;

	// Si se ha soltado el bot�n de disparo
	static bool botonDisparoSoltado;

	// N�mero de disparos de la nave en una pulsaci�n del bot�n de disparo
	static int numDisparosPulsacion;

	// Si se ha soltado el bot�n de resetar la partida
	static bool botonResetSoltado;


	/* Getters y setters */

	static Nave *getNave ()
	{
		return Controlador::nave;
	}

	static void setNave (Nave *nave)
	{
		Controlador::nave = nave;

		// Se actualizan las referencias a los campos de la nave para no tener que referenciarlos indirectamente en
		// cada m�todo del controlador
		Controlador::posicionNave = Controlador::nave->getPosicionReferencia ();
		Controlador::velocidadNave = Controlador::nave->getVelocidadReferencia ();
		Controlador::rotacionNave = Controlador::nave->getRotacionReferencia ();
		Controlador::correcionRotNave = Controlador::nave->getCorreccionRotacionReferencia ();
		Controlador::coefAceleracionNave = Controlador::nave->getCoefAceleracionReferencia ();
	}


	/* M�todos */

	// Control de la c�mara
	static void calcularViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &posicionCamara);

	// Input del teclado
	static void inputTeclado (GLFWwindow *ventana);

	// Redimensionado de la ventana
	static void redimensionarVentana (GLFWwindow *ventana, int ancho, int alto);


private:

	/* Atributos */

	// Nave
	static Nave *nave;

	// Posici�n de la nave
	static glm::vec3 *posicionNave;

	// Velocidad de la nave en cada coordenada
	static glm::vec3 *velocidadNave;

	// Posici�n a la que apunta la nave
	static glm::vec3 *rotacionNave;

	// Correcci�n de la rotaci�n de la nave
	static glm::vec3 *correcionRotNave;

	// Coeficiente de aceleraci�n de la nave
	static glm::vec3 *coefAceleracionNave;
};

#endif
