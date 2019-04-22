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

// Para representar el sistema solar
#include "Astro.h"
#include "Estrella.h"

// Para representar las figuras básicas
#include "Forma.h"

// Para dar a los objetos distintos materiales con los que reaccionar a la luz
#include "Material.h"

// Para crear puntos de luz en la escena
#include "PuntoLuz.h"

// Para asignar texturas a los astros
#include "Textura.h"


// Propiedades de la ventana a crear
int wWidth = 800;
int wHeight = 800;

// Relación de aspecto de la ventana
float relacionAspecto = wWidth / wHeight;

// FOV de la cámara
const double FOV = 80.0;


// Shaders para los vértices y su nombre
const char *vertexShaderColor = "shaderColor.vert";
const char *vertexShaderColorT1 = "shaderColorT1.vert";
const char *vertexShaderColorT2 = "shaderColorT2.vert";
const char *vertexShaderIluminacionT1 = "shaderPhongT1.vert";
const char *vertexShaderIluminacionT2 = "shaderPhongT2.vert";
const char *vertexShaderIluminacionNuevoT1 = "shaderPhongNuevoT1.vert";


// Shaders para los fragmentos y su nombre
const char *fragmentShaderColor = "shaderColor.frag";
const char *fragmentShaderColorT1 = "shaderColorT1.frag";
const char *fragmentShaderColorT2 = "shaderColorT2.frag";
const char *fragmentShaderIluminacionT1 = "shaderPhongT1.frag";
const char *fragmentShaderIluminacionT2 = "shaderPhongT2.frag";
const char *fragmentShaderIluminacionNuevoT1 = "shaderPhongNuevoT1.frag";

// Shaders que emplear en el renderizado
Shader *shaderColor = NULL;
Shader *shaderColorT1 = NULL;
Shader *shaderColorT2 = NULL;
Shader *shaderIluminacionT1 = NULL;
Shader *shaderIluminacionT2 = NULL;
Shader *shader = NULL;


Modelo *modelo = NULL;
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
	/*shaderColor->usar ();
	shaderColor->setMat4 ("projectionMatrix", projectionMatrix);
	shaderColor->setMat4 ("viewMatrix", viewMatrix);

	shaderColorT1->usar ();
	shaderColorT1->setMat4 ("projectionMatrix", projectionMatrix);
	shaderColorT1->setMat4 ("viewMatrix", viewMatrix);

	shaderColorT2->usar ();
	shaderColorT2->setMat4 ("projectionMatrix", projectionMatrix);
	shaderColorT2->setMat4 ("viewMatrix", viewMatrix);

	shaderIluminacionT1->usar ();
	shaderIluminacionT1->setMat4 ("projectionMatrix", projectionMatrix);
	shaderIluminacionT1->setMat4 ("viewMatrix", viewMatrix);

	shaderIluminacionT2->usar ();
	shaderIluminacionT2->setMat4 ("projectionMatrix", projectionMatrix);
	shaderIluminacionT2->setMat4 ("viewMatrix", viewMatrix);*/

	shader->usar ();
	shader->setMat4 ("projectionMatrix", projectionMatrix);
	shader->setMat4 ("viewMatrix", viewMatrix);

	// Además, es necesario cargar la posición de la cámara para el cálcula de la iluminación en dichos shaders
	/*shaderIluminacionT1->usar ();
	shaderIluminacionT1->setVec3 ("posicionCamara", posicionCamara);
	shaderIluminacionT2->usar ();
	shaderIluminacionT2->setVec3 ("posicionCamara", posicionCamara);*/

	shader->usar ();
	shader->setMat4 ("modelMatrix", glm::mat4 (1.0f));
	shader->setMat3 ("normalMatrix", glm::transpose (glm::inverse (glm::mat3 (1.0f))));
	shader->setVec3 ("posicionCamara", posicionCamara);

	luz->cargar (shader, glm::mat4(1.0f));

	modelo->dibujar (shader);

	std::cout << glGetError () << std::endl;
}


int main (int argc, char **argv) {

	/* Inicialización de OpenGL */

	GLFWwindow *ventana = NULL;
	Inicializador::inicializarOpenGL (&ventana, "P6_33a", &wWidth, &wHeight, &relacionAspecto);


	/* Generación de los objetos */

	// Se inicializan las formas básicas
	Forma::inicializarFormas ();



	/* Compilación de los shaders */
	
	/*shaderColor = new Shader (vertexShaderColor, fragmentShaderColor);
	shaderColorT1 = new Shader (vertexShaderColorT1, fragmentShaderColorT1);
	shaderColorT2 = new Shader (vertexShaderColorT2, fragmentShaderColorT2);
	shaderIluminacionT1 = new Shader (vertexShaderIluminacionT1, fragmentShaderIluminacionT1);
	shaderIluminacionT2 = new Shader (vertexShaderIluminacionT2, fragmentShaderIluminacionT2);*/

	shader = new Shader ("shader.vert", "shader.frag");


	modelo = new Modelo ("Viper-mk-IV-fighter.obj");

    luz = new PuntoLuz(glm::vec3 (0.0f, 30.0f, 30.0f), 1.0f, 0.000028f, 0.00000014f, glm::vec3 (0.5f, 0.5f, 0.5f),
		glm::vec3 (1.0f, 1.0f, 1.0f), glm::vec3 (1.0f, 1.0f, 1.0f));

	/* Configuración de OpenGL */

	// Se configura OpenGL con una serie de opciones ya definidas
	Inicializador::configurarOpenGL ();


	/* Bucle de renderizado (main loop) */

	// Para saber el tiempo transcurrido entre frames
	float tiempoAnterior = 0;
	float tiempoActual = 0;

	// Se almacena en el controlador la referencias a los valores a modificar
	Controlador::orbitasAstros = &(Astro::representarOrbitas);
	
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
	delete shaderColor;
	delete shaderColorT1;
	delete shaderColorT2;
	delete shaderIluminacionT1;
	delete shaderIluminacionT2;

	return(0);
}
