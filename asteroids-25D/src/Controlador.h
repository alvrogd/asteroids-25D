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


class Controlador
{
public:
	/* Atributos */

	// Tama�o de la ventana
	static int *wWidth;
	static int *wHeight;
	
	// Relaci�n de aspecto de la ventana
	static float *relacionAspecto;

	// Modo de la c�mara
	static int modoCamara;

	// Posici�n de la nave
	static glm::vec3 *posicionNave;
	
	// Velocidad de la nave en cada coordenada
	static glm::vec3 *velocidadNave;

	// Posici�n a la que apunta la nave
	static glm::vec3 *rotacionNave;

	// Correcci�n de la rotaci�n de la nave
	static glm::vec3 *correcionRotNave;


	/* M�todos */

	// Control de la c�mara
	static void calcularViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &posicionCamara);

	// Input del teclado
	static void inputTeclado (GLFWwindow *ventana);

	// Redimensionado de la ventana
	static void redimensionarVentana (GLFWwindow *ventana, int ancho, int alto);
};

#endif
