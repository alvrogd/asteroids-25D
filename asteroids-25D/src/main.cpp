// Valor de PI
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

// Variables de windows
#include <windows.h>

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Para cargar modelos
#include "Modelo.h"

// Shaders
#include "Shader.h"

// E/S
#include <iostream>

// Inicializador de OpenGL
#include "Inicializador.h"

// Controlador de eventos
#include "Controlador.h"

// Para representar las figuras básicas
#include "Forma.h"

// Para crear puntos de luz en la escena
#include "PuntoLuz.h"

// Para crear los objetos a representar en pantalla
#include "Objeto.h"
#include "Movil.h"


// Propiedades de la ventana a crear
int wWidth = 800;
int wHeight = 800;

// Relación de aspecto de la ventana
float relacionAspecto = wWidth / wHeight;

// FOV de la cámara
const double FOV = 80.0;

// Nombres de los ficheros que contienen los shaders
const char *fragmentShader = "shader.frag";
const char *vertexShader = "shader.vert";

// Shader que emplear en el renderizado
Shader *shader = NULL;

// Objetos que representar en pantalla y sus modelos
Movil *nave = NULL;
Modelo *modeloNave = NULL;

PuntoLuz *luz = NULL;


/**
 * Se renderiza un frame
 */
void display ()
{	
	// Se especifica el valor para limpiar los buffers de color
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
	// Se limpia el buffer de color
	glClear (GL_COLOR_BUFFER_BIT);

	// Se especifica al valor para limpiar el buffer de profundidad
	glClearDepth (1.0f);
	// Se limpia el buffer de profundidad
	glClear (GL_DEPTH_BUFFER_BIT);

	// Se empleará toda la ventana en el renderizado
	glViewport (0, 0, wWidth, wHeight);

	// Se representarán los cuerpos con relleno
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);


	// Se calculan las matrices de proyección y de visionado

	// Se aplica una proyección en perspectiva
	glm::mat4 projectionMatrix = glm::perspective (glm::radians (FOV), (double)wWidth / (double)wHeight, 0.1, 2000.0);

	// La matriz de visionado la calcula en control puesto que la cámara es controlada por el input del usuario
	glm::mat4 viewMatrix;
	glm::vec3 posicionCamara;
	Controlador::calcularViewMatrix (viewMatrix, posicionCamara);

	// Se aplican las matrices a los shaders a emplear
	shader->usar ();
	shader->setMat4 ("projectionMatrix", projectionMatrix);
	shader->setMat4 ("viewMatrix", viewMatrix);

	// Además, es necesario cargar la posición de la cámara para el cálcula de la iluminación en dichos shaders
	luz->cargar (shader, glm::mat4(1.0f));

	// Se representa la nave
	nave->dibujar (glm::mat4(1.0f), shader);

	std::cout << glGetError () << std::endl;
}


int main (int argc, char **argv) {

	/* Inicialización de OpenGL */

	GLFWwindow *ventana = NULL;
	Inicializador::inicializarOpenGL (&ventana, "Asteroids 2.5D", &wWidth, &wHeight, &relacionAspecto);


	/* Generación de los objetos */

	// Se inicializan las formas básicas
	Forma::inicializarFormas ();

	modeloNave = new Modelo ("Viper-mk-IV-fighter.obj");
	nave = new Movil (glm::vec3 (2.5f, 1.0f, 1.0f), modeloNave, glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f,
		0.0f), glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 0.0f));

	luz = new PuntoLuz (glm::vec3 (0.0f, 30.0f, 30.0f), 1.0f, 0.000028f, 0.00000014f, glm::vec3 (0.5f, 0.5f, 0.5f),
		glm::vec3 (1.0f, 1.0f, 1.0f), glm::vec3 (1.0f, 1.0f, 1.0f));


	/* Compilación de los shaders */
	
	shader = new Shader (vertexShader, fragmentShader);


	/* Configuración de OpenGL */

	// Se configura OpenGL con una serie de opciones ya definidas
	Inicializador::configurarOpenGL ();


	/* Bucle de renderizado (main loop) */

	// Para saber el tiempo transcurrido entre frames
	float tiempoAnterior = 0;
	float tiempoActual = 0;

	// Se almacena en el controlador la referencias a los valores a modificar
	
	// Mientras no se haya indicado la finalización
	while (!glfwWindowShouldClose (ventana)) {

		/* Input */

		// Se procesa el input del usuario
		Controlador::inputTeclado (ventana);


		/* Actualización */

		// Se obtiene el tiempo transcurrido desde que se ha cargado la librería
		tiempoActual = (float)glfwGetTime ();


		// Tras ejecutar las actualizaciones, se almacena el momento en que se ejecutaron
		tiempoAnterior = tiempoActual;


		/* Render */

		// Se emplea la función ya definida para renderizar un frame
		display ();


		/* Final iteración */

		// Se intercambian los buffers de color delantero y trasero
		glfwSwapBuffers (ventana);

		// Se procesan eventos pendientes (entrada por teclado, redimensionado...)
		glfwPollEvents ();
	}

	// Se destruyen los astros
	

	// Se destruyen las formas básicas
	Forma::destruirFormas ();

	// Se destruyen los shaders
	delete shader;

	delete nave;
	delete modeloNave;

	return(0);
}
