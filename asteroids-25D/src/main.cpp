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

// Para crear puntos de luz en la escena
#include "PuntoLuz.h"

// Para crear los objetos a representar en pantalla
#include "Forma.h"
#include "Objeto.h"
#include "Movil.h"
#include "Nave.h"
#include "Asteroide.h"
#include "Disparo.h"

// Para representar una skybox
#include "Cubemap.h"

// Para conjuntos de datos
#include <vector>

// Para la generaci�n de n�meros aleatorios
#include <ctime>


// Propiedades de la ventana a crear
int wWidth = 800;
int wHeight = 800;

// Relaci�n de aspecto de la ventana
float relacionAspecto = wWidth / wHeight;

// FOV de la c�mara
const double FOV = 80.0;

// Nombres de los ficheros que contienen los shaders
const char *fragmentShaderModelos = "shader.frag";
const char *vertexShaderModelos = "shader.vert";
const char *fragmentShaderColor = "shaderColor.frag";
const char *vertexShaderColor = "shaderColor.vert";
const char *fragmentShaderSkybox = "shaderSkybox.frag";
const char *vertexShaderSkybox = "shaderSkybox.vert";

// Shaders que emplear en el renderizado
Shader *shader = NULL;
Shader *shaderColor = NULL;
Shader *shaderSkybox = NULL;

// Objetos que representar en pantalla y sus modelos
Nave *nave = NULL;
Modelo *modeloNave = NULL;

PuntoLuz *luz = NULL;

std::vector<Asteroide *> asteroides;
Modelo *modeloAsteroide = NULL;

std::vector<Disparo *> disparos;

// Ficheros que componen la skybox
std::vector<std::string> caras
{
	"PositiveX.png",
	"NegativeX.png",
	"PositiveY.png",
	"NegativeY.png",
	"PositiveZ.png",
	"NegativeZ.png"
};

// Skybox
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
	// TODO relacionar el final con el �rea de warp
	glm::mat4 projectionMatrix = glm::perspective (glm::radians (FOV), (double)wWidth / (double)wHeight, 0.1, 2000.0);

	// La matriz de visionado la calcula en control puesto que la c�mara es controlada por el input del usuario
	glm::mat4 viewMatrix;
	glm::vec3 posicionCamara;
	Controlador::calcularViewMatrix (viewMatrix, posicionCamara);

	// Se aplican las matrices al shader de los modelos
	shader->usar ();
	shader->setMat4 ("projectionMatrix", projectionMatrix);
	shader->setMat4 ("viewMatrix", viewMatrix);

	// Adem�s, es necesario cargar las luces presentes en la escena
	luz->cargar (shader, glm::mat4(1.0f), 0);
	
	// Ya se ha cargado la luz principal
	int numLuces = 1;

	// Es necesario considerar el l�mite de luces del shader
	int max = std::min (disparos.size (), (size_t) 50);
	for (int i = 0; i < max; i++)
	{
		// Se carga la luz del disparo iterado
		disparos.at (i)->getLuz ()->cargar (shader, glm::mat4 (1.0f), numLuces);

		// Se incrementa el n�mero de luces cargadas
		numLuces++;
	}

	// Se indica el n�mero de luces cargadas
	shader->setInt ("numPuntosLuz", numLuces);

	// Y la posici�n de la c�mara para el c�lculo de la iluminaci�n
	shader->setVec3 ("posicionCamara", posicionCamara);

	// Se representa la nave
	nave->dibujar (glm::mat4(1.0f), shader);

	// Se representan todos los asteroides
	for (Asteroide *asteroide : asteroides)
	{
		asteroide->dibujar (glm::mat4 (1.0f), shader);
	}

	// Se carga el shader de color y se le aplican las matrices
	shaderColor->usar ();
	shaderColor->setMat4 ("projectionMatrix", projectionMatrix);
	shaderColor->setMat4 ("viewMatrix", viewMatrix);

	// Se representan todos los disparos
	for (Disparo *disparo : disparos)
	{
		disparo->dibujar (glm::mat4 (1.0f), shaderColor);
	}
	
	// Ahora se carga el shader de la skybox y se aplican tambi�n las matrices necesarias
	shaderSkybox->usar ();
	shaderSkybox->setMat4 ("projectionMatrix", projectionMatrix);
	shaderSkybox->setMat4 ("viewMatrix", glm::mat4 (glm::mat3 (viewMatrix)));

	// Y se representa la skybox
	skybox->dibujar (shaderSkybox);

	//std::cout << glGetError () << std::endl;
}


int main (int argc, char **argv) {

	// Se obtiene una semilla en funci�n del tiempo para la generaci�n de n�meros aleatorios
	std::srand (std::time (NULL));


	/* Inicializaci�n de OpenGL */

	GLFWwindow *ventana = NULL;
	Inicializador::inicializarOpenGL (&ventana, "Asteroids 2.5D", &wWidth, &wHeight, &relacionAspecto);


	/* Generaci�n de los objetos */

	// Se inicializan las formas b�sicas
	Forma::inicializarFormas ();

	// Se establece el warp de los objetos m�viles, permitiendo que se muevan dentro de un cuadrado de lado 1000
	// IMPORTANTE ESTABLECERLO ANTES DE GENERAR UN ASTEROIDE
	Movil::coordenadasWarp = glm::vec3 (1000.0f, 1000.0f, 1000.0f);

	// Se guarda en la clase de asteroides una referencia al conjunto que contiene los asteroides a representar en
	// pantalla
	Asteroide::conjuntoAsteroides = &asteroides;

	// Se guarda en la clase de disparos una referencia al conjunto que contiene los disparos a representar en
	// pantalla
	Disparo::conjuntoDisparos = &disparos;

	// Se cargan los modelos necesarios
	modeloNave = new Modelo ("Viper-mk-IV-fighter.obj");
	modeloAsteroide = new Modelo ("rock_by_dommk.obj");

	// Se almacena una referencia al modelo que empler�n los asteroides
	Asteroide::modelo = modeloAsteroide;

	// La nave mira inicialmente hacia el eje -X, por lo que se establece la rotaci�n inicial correspondiente para
	// corregirlo de cara al c�lculo de su movimiento
	nave = new Nave (glm::vec3 (1.0f, 1.0f, 1.0f), modeloNave, 8.0f, glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f,
		// TODO antes el �ltimo era 1,5 0,0 0,0 no se por qu�
		0.0f), glm::vec3 (0.5f, 0.5f, 0.5f), glm::vec3 (0.02f, 0.02f, 0.02f), glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -90.0f, 0.0f));

	/*asteroide = new Movil (glm::vec3 (0.1f, 0.1f, 0.1f), modeloAsteroide, glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (
		0.5f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3 (0.0f, 0.0f, 0.0f));*/
	// Se crean 100 asteroides
	for (int i = 0; i < 100; i++)
	{
		asteroides.push_back (new Asteroide ());
	}

	luz = new PuntoLuz (glm::vec3 (0.0f, 30.0f, 30.0f), 1.0f, 0.000028f, 0.00000014f, glm::vec3 (0.5f, 0.5f, 0.5f),
		glm::vec3 (1.0f, 1.0f, 1.0f), glm::vec3 (1.0f, 1.0f, 1.0f));

	skybox = new Cubemap (caras);


	/* Compilaci�n de los shaders */
	
	shader = new Shader (vertexShaderModelos, fragmentShaderModelos);
	shaderColor = new Shader (vertexShaderColor, fragmentShaderColor);
	shaderSkybox = new Shader (vertexShaderSkybox, fragmentShaderSkybox);

	/*shaderSkybox->usar ();
	shaderSkybox->setInt ("skybox", 0);*/


	/* Configuraci�n de OpenGL */

	// Se configura OpenGL con una serie de opciones ya definidas
	Inicializador::configurarOpenGL ();


	/* Bucle de renderizado (main loop) */

	// Para saber el tiempo transcurrido entre frames
	float tiempoAnterior = 0;
	float tiempoActual = 0;
	float delta = 0;

	// Se almacena en el controlador la referencias a los valores a modificar
	Controlador::posicionNave = nave->getPosicionReferencia ();
	Controlador::velocidadNave = nave->getVelocidadReferencia ();
	Controlador::rotacionNave = nave->getRotacionReferencia ();
	Controlador::correcionRotNave = nave->getCorreccionRotacionReferencia ();
	Controlador::coefAceleracionNave = nave->getCoefAceleracionReferencia ();

	Controlador::nave = nave;


	// Mientras no se haya indicado la finalizaci�n
	while (!glfwWindowShouldClose (ventana)) {

		/* Input */

		// Se procesa el input del usuario
		Controlador::inputTeclado (ventana);


		/* Actualizaci�n */

		// Se obtiene el tiempo transcurrido desde que se ha cargado la librer�a
		tiempoActual = (float)glfwGetTime ();

		// Se actualizar�n los objetos en pantalla en funci�n del tiempo transcurrido
		delta = tiempoActual - tiempoAnterior;
		
		// Se actualizan las posiciones de los objetos
		nave->actualizarEstado (delta);

		for (Asteroide *asteroide : asteroides)
		{
			asteroide->actualizarEstado (delta);
		}

		// NO CAMBIAR A FOR EACH PORQUE, SI SE ELIMINA UN DISPARO, PUEDE PETAR
		for (int i = 0; i < disparos.size(); i++)
		{
			Disparo *disparo = disparos.at (i);

			disparo->actualizarEstado (delta);

			// Si se han agotado los warps restantes, se elimina el disparo de la escena
			if (disparo->getWarpsRestantes () < 0)
			{
				delete disparo;

				// Correcci�n del iterador para no saltarse ning�n asteroide
				i--;
			}
		}

		// Se comprueba si la nave ha colisionado con alg�n asteroide
		// TODO ser�a m�s eficiente fusionar los bucles
		for (int i = 0; i < asteroides.size(); i++)
		{
			if (nave->checkColision (asteroides.at(i)))
			{
				std::cout << "colisi�n con el asteroide n�: " << i << std::endl;

				// Se destruye el asteroide
				asteroides.at (i)->explotar ();

				// Correcci�n del iterador para no saltarse ning�n asteroide
				i--;
			}
		}

		// Se comprueba si los disparos han colisionado con alg�n asteroide
		for (int i = 0; i < asteroides.size (); i++)
		{
			for (int j = 0; j < disparos.size (); j++)
			{
				if (disparos.at (j)->checkColision (asteroides.at (i)))
				{
					std::cout << "colisi�n con el asteroide n�: " << i << std::endl;

					// Se destruye el asteroide
					asteroides.at (i)->explotar ();

					// Se destruye el disparo
					delete disparos.at (j);

					// Correcci�n del iterador para no saltarse ning�n asteroide
					i--;

					// Ya no es necesario comprobar la colisi�n del asteroide iterado con alg�n otro disparo
					break;
				}
			}
		}

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
	Forma::destruirFormas ();

	delete nave;
	delete modeloNave;

	for (Asteroide *asteroide : asteroides)
	{
		delete asteroide;
	}

	delete modeloAsteroide;

	delete luz;

	delete skybox;
	

	// Se destruyen los shaders
	delete shader;
	delete shaderColor;
	delete shaderSkybox;


	return (0);
}
