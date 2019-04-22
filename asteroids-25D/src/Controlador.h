#ifndef CONTROLADOR_H
#define CONTROLADOR_H

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para almacenar referencias a astros
#include "Astro.h"

// Para modificar la matriz de visionado
#include "Shader.h"


class Controlador
{
public:
	// Tamaño de la ventana
	static int *wWidth;
	static int *wHeight;
	
	// Relación de aspecto de la ventana
	static float *relacionAspecto;

	// Modo de la cámara
	static int modoCamara;
	
	// Referencia al multiplicador del tiempo transcurrido
	static float *multiplicador;

	// Referencia al switch con el que representar las órbitas de los astros
	static bool *orbitasAstros;

	// Referencias a los astros necesarios para el modo telescopio
	static Astro *sol;
	static Astro *mercurio;
	static Astro *venus;
	static Astro *tierra;
	static Astro *iss;
	static Astro *luna;
	static Astro *marte;
	static Astro *jupiter;
	static Astro *saturno;
	static Astro *urano;
	static Astro *neptuno;
	

	// Control de la cámara
	static void calcularViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &posicionCamara);

	// Input del teclado
	static void inputTeclado (GLFWwindow *ventana);

	// Redimensionado de la ventana
	static void redimensionarVentana (GLFWwindow *ventana, int ancho, int alto);
};

#endif
