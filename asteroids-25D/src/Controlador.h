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

	// Posición de la nave
	static glm::vec3 *posicionNave;
	
	// Velocidad de la nave en cada coordenada
	static glm::vec3 *velocidadNave;

	// Posición a la que apunta la nave
	static glm::vec3 *rotacionNave;

	// Corrección de la rotación de la nave
	static glm::vec3 *correcionRotNave;


	/* Métodos */

	// Control de la cámara
	static void calcularViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &posicionCamara);

	// Input del teclado
	static void inputTeclado (GLFWwindow *ventana);

	// Redimensionado de la ventana
	static void redimensionarVentana (GLFWwindow *ventana, int ancho, int alto);
};

#endif
