#ifndef CONTROLADOR_H
#define CONTROLADOR_H

// Librerías de OpenGL
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

	// Tamaño de la ventana
	static int *wWidth;
	static int *wHeight;
	
	// Relación de aspecto de la ventana
	static float *relacionAspecto;

	// Asteroides en la escena
	static std::vector<Asteroide *> *asteroides;

	// Partículas en la escena
	static std::vector<Particula *> *particulas;
	static std::vector<ConjuntoParticulas *> *conjuntosParticulas;

	// Si se ha soltado el botón de disparo
	static bool botonDisparoSoltado;

	// Número de disparos de la nave en una pulsación del botón de disparo
	static int numDisparosPulsacion;

	// Si se ha soltado el botón de resetar la partida
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
		// cada método del controlador
		Controlador::posicionNave = Controlador::nave->getPosicionReferencia ();
		Controlador::velocidadNave = Controlador::nave->getVelocidadReferencia ();
		Controlador::rotacionNave = Controlador::nave->getRotacionReferencia ();
		Controlador::correcionRotNave = Controlador::nave->getCorreccionRotacionReferencia ();
		Controlador::coefAceleracionNave = Controlador::nave->getCoefAceleracionReferencia ();
	}


	/* Métodos */

	// Control de la cámara
	static void calcularViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &posicionCamara);

	// Input del teclado
	static void inputTeclado (GLFWwindow *ventana);

	// Redimensionado de la ventana
	static void redimensionarVentana (GLFWwindow *ventana, int ancho, int alto);


private:

	/* Atributos */

	// Nave
	static Nave *nave;

	// Posición de la nave
	static glm::vec3 *posicionNave;

	// Velocidad de la nave en cada coordenada
	static glm::vec3 *velocidadNave;

	// Posición a la que apunta la nave
	static glm::vec3 *rotacionNave;

	// Corrección de la rotación de la nave
	static glm::vec3 *correcionRotNave;

	// Coeficiente de aceleración de la nave
	static glm::vec3 *coefAceleracionNave;
};

#endif
