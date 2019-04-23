// Valor de PI
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

// Variables de windows
#include <windows.h>

// Librer�as de OpenGL
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

// Para representar las figuras b�sicas
#include "Forma.h"

// Para crear puntos de luz en la escena
#include "PuntoLuz.h"

// Para crear los objetos a representar en pantalla
#include "Objeto.h"
#include "Movil.h"

// Para representar una skybox
#include "Cubemap.h"


// Propiedades de la ventana a crear
int wWidth = 800;
int wHeight = 800;

// Relaci�n de aspecto de la ventana
float relacionAspecto = wWidth / wHeight;

// FOV de la c�mara
const double FOV = 80.0;

// Nombres de los ficheros que contienen los shaders
const char *fragmentShader = "shader.frag";
const char *vertexShader = "shader.vert";

// Shaders que emplear en el renderizado
Shader *shader = NULL;
Shader *shaderSkybox = NULL;

// Objetos que representar en pantalla y sus modelos
Movil *nave = NULL;
Modelo *modeloNave = NULL;

PuntoLuz *luz = NULL;

/*std::vector<std::string> caras
{
	"right.jpg",
	"left.jpg",
	"top.jpg",
	"bottom.jpg",
	"front.jpg",
	"back.jpg"
};*/

std::vector<std::string> caras
{
	"PositiveX.png",
	"NegativeX.png",
	"PositiveY.png",
	"NegativeY.png",
	"PositiveZ.png",
	"NegativeZ.png"
};

Cubemap *skybox = NULL;

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

	// Se emplear� toda la ventana en el renderizado
	glViewport (0, 0, wWidth, wHeight);

	// Se representar�n los cuerpos con relleno
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);


	// Se calculan las matrices de proyecci�n y de visionado

	// Se aplica una proyecci�n en perspectiva
	glm::mat4 projectionMatrix = glm::perspective (glm::radians (FOV), (double)wWidth / (double)wHeight, 0.1, 100.0);

	// La matriz de visionado la calcula en control puesto que la c�mara es controlada por el input del usuario
	glm::mat4 viewMatrix;
	glm::vec3 posicionCamara;
	Controlador::calcularViewMatrix (viewMatrix, posicionCamara);

	// Se aplican las matrices a los shaders a emplear
	

	shader->usar ();
	shader->setMat4 ("projectionMatrix", projectionMatrix);
	shader->setMat4 ("viewMatrix", viewMatrix);


	// Adem�s, es necesario cargar la posici�n de la c�mara para el c�lcula de la iluminaci�n en dichos shaders
	luz->cargar (shader, glm::mat4(1.0f));

	// Se representa la nave
	nave->dibujar (glm::mat4(1.0f), shader);

	shaderSkybox->usar ();
	shaderSkybox->setMat4 ("projectionMatrix", projectionMatrix);
	shaderSkybox->setMat4 ("viewMatrix", glm::mat4 (glm::mat3 (viewMatrix)));

	skybox->dibujar (shaderSkybox);

	//std::cout << glGetError () << std::endl;
}


int main (int argc, char **argv) {

	/* Inicializaci�n de OpenGL */

	GLFWwindow *ventana = NULL;
	Inicializador::inicializarOpenGL (&ventana, "Asteroids 2.5D", &wWidth, &wHeight, &relacionAspecto);


	/* Generaci�n de los objetos */

	// Se inicializan las formas b�sicas
	Forma::inicializarFormas ();

	modeloNave = new Modelo ("Viper-mk-IV-fighter.obj");
	// La nave mira inicialmente hacia el eje -X, por lo que se establece la rotaci�n inicial correspondiente para
	// corregirlo de cara al c�lculo de su movimiento
	nave = new Movil (glm::vec3 (1.0f, 1.0f, 1.0f), modeloNave, glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f,
		0.0f), glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 1.5f), glm::vec3(0.0f, -90.0f, 0.0f));

	luz = new PuntoLuz (glm::vec3 (0.0f, 30.0f, 30.0f), 1.0f, 0.000028f, 0.00000014f, glm::vec3 (0.5f, 0.5f, 0.5f),
		glm::vec3 (1.0f, 1.0f, 1.0f), glm::vec3 (1.0f, 1.0f, 1.0f));

	skybox = new Cubemap (caras);


	/* Compilaci�n de los shaders */
	
	shader = new Shader (vertexShader, fragmentShader);
	shaderSkybox = new Shader ("shaderSkybox.vert", "shaderSkybox.frag");

	shaderSkybox->usar ();
	shaderSkybox->setInt ("skybox", 0);


	/* Configuraci�n de OpenGL */

	// Se configura OpenGL con una serie de opciones ya definidas
	Inicializador::configurarOpenGL ();


	/* Bucle de renderizado (main loop) */

	// Para saber el tiempo transcurrido entre frames
	float tiempoAnterior = 0;
	float tiempoActual = 0;

	// Se almacena en el controlador la referencias a los valores a modificar
	Controlador::posicionNave = nave->getPosicionReferencia ();
	Controlador::velocidadNave = nave->getVelocidadReferencia ();
	Controlador::rotacionNave = nave->getRotacionReferencia ();
	Controlador::correcionRotNave = nave->getCorreccionRotacionReferencia ();


	// Mientras no se haya indicado la finalizaci�n
	while (!glfwWindowShouldClose (ventana)) {

		/* Input */

		// Se procesa el input del usuario
		Controlador::inputTeclado (ventana);


		/* Actualizaci�n */

		// Se obtiene el tiempo transcurrido desde que se ha cargado la librer�a
		tiempoActual = (float)glfwGetTime ();

		nave->actualizarEstado ();

		// Tras ejecutar las actualizaciones, se almacena el momento en que se ejecutaron
		tiempoAnterior = tiempoActual;


		/* Render */

		// Se emplea la funci�n ya definida para renderizar un frame
		display ();


		/* Final iteraci�n */

		// Se intercambian los buffers de color delantero y trasero
		glfwSwapBuffers (ventana);

		// Se procesan eventos pendientes (entrada por teclado, redimensionado...)
		glfwPollEvents ();
	}

	// Se destruyen los objetos creados y sus modelos
	delete nave;
	delete modeloNave;

	delete luz;

	delete skybox;
	

	// Se destruyen las formas b�sicas
	Forma::destruirFormas ();

	// Se destruyen los shaders
	delete shader;
	delete shaderSkybox;


	return(0);
}
