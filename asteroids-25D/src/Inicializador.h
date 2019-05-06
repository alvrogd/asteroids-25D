#ifndef INICIALIZADOR_H
#define INICIALIZADOR_H

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// E/S
#include <iostream>

// Controlador de eventos
#include "Controlador.h"

class Inicializador
{
public:
	static void inicializarOpenGL (GLFWwindow **ventana, const char *nombreVentana, int *wWidth, int *wHeight, float
		*relacionAspecto);

	static void inicializarGLFW ();

	static void crearVentana (GLFWwindow **ventana, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const char
		*nombreVentana);

	static void inicializarGLAD ();

	static void configurarDimensionadoVentana (GLFWwindow *ventana,	int *wWidth, int *wHeight, float *relacionAspecto);

	static void configurarInputVentana (GLFWwindow *ventana);

	static void configurarOpenGL ();
};

#endif