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

// Para almacenar referencias a los asteroides de la escena
#include "Asteroide.h"

// Para almacenar una referencia a la nave representada
#include "Nave.h"


class Controlador
{
public:
	/* Atributos */

	// Tamaño de la ventana
	static int *wWidth;
	static int *wHeight;
	
	// Relación de aspecto de la ventana
	static float *relacionAspecto;

	// Modo de la cámara
	static int modoCamara;

	// Asteroides en la escena
	static std::vector<Asteroide *> *asteroides;

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

	// Nave
	static Nave *nave;

	// Si se ha soltado el botón de disparo
	static bool botonDisparoSoltado;

	// Número de disparos de la nave en una pulsación del botón de disparo
	static int numDisparosPulsacion;


	/* Métodos */

	// Control de la cámara
	static void calcularViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &posicionCamara);

	// Input del teclado
	static void inputTeclado (GLFWwindow *ventana);

	// Redimensionado de la ventana
	static void redimensionarVentana (GLFWwindow *ventana, int ancho, int alto);
};

#endif
