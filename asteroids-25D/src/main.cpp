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

// Sonido
#include <irrKlang.h>

// Ejecuci�n multihilo
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <chrono>

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
#include "Particula.h"
#include "ConjuntoParticulas.h"

// Para representar una skybox
#include "Cubemap.h"

// Para la reproducci�n de sonido
#include "Sonido.h"

// Para conjuntos de datos
#include <vector>

// Para la generaci�n de n�meros aleatorios
#include <ctime>


// M�texes y variables de condici�n mediante las cuales emular barreras (estas �ltimas no se encuentran disponibles
// en C++)
std::mutex mutexRegion1;
std::mutex mutexRegion2;
std::condition_variable condicionRegion1;
std::condition_variable condicionRegion2;


// Tiempo transcurrido entre frames; se actualizan en cada iteraci�n del bucle principal
float tiempoAnterior = 0;
float tiempoActual = 0;
float delta = 0;
// Variables auxiliares empleadas para la sincronizaci�n entre los dos hilos usados
bool nuevoDelta = false;
bool hiloAuxiliarFinalizado = false;

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
const char *fragmentShaderParticulas = "shaderParticula.frag";
const char *vertexShaderParticulas = "shaderParticula.vert";

// Shaders que emplear en el renderizado
Shader *shader = NULL;
Shader *shaderColor = NULL;
Shader *shaderSkybox = NULL;
Shader *shaderParticulas = NULL;

// Objetos que representar en pantalla y sus modelos
Nave *nave = NULL;
Modelo *modeloNave = NULL;

PuntoLuz *luz = NULL;

std::vector<Asteroide *> asteroides;
Modelo *modeloAsteroide = NULL;

std::vector<Disparo *> disparos;

std::vector<Particula *> particulas;
std::vector<ConjuntoParticulas *> conjuntosParticulas;

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

	// Se representa la nave si a�n no ha sido destruida
	if (!(nave->getIsDestruida ()))
	{
		nave->dibujar (glm::mat4 (1.0f), shader);
	}

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

	// Se carga el shader de part�culas y se aplican las matrices
	shaderParticulas->usar ();
	shaderParticulas->setMat4 ("projectionMatrix", projectionMatrix);
	shaderParticulas->setMat4 ("viewMatrix", viewMatrix);

	// Se representan todas las part�culas, tanto las individuales como las contenidas en conjuntos de part�culas
	for (Particula *particula : particulas)
	{
		particula->dibujar (glm::mat4 (1.0f), shaderParticulas);
	}

	for (ConjuntoParticulas *conjuntoParticulas : conjuntosParticulas)
	{
		conjuntoParticulas->dibujar (glm::mat4 (1.0f), shaderParticulas);
	}
	
	// Ahora se carga el shader de la skybox y se aplican tambi�n las matrices necesarias
	shaderSkybox->usar ();
	shaderSkybox->setMat4 ("projectionMatrix", projectionMatrix);
	shaderSkybox->setMat4 ("viewMatrix", glm::mat4 (glm::mat3 (viewMatrix)));

	// Y se representa la skybox
	skybox->dibujar (shaderSkybox);

	//std::cout << glGetError () << std::endl;
}


/*
 * Se actualiza la posici�n de las part�culas representadas en la escena que pertenecen a conjuntos y, si se les agota
 * el tiempo de vida, son eliminadas
 */
void actualizarParticulasConjuntos (std::future<void> condicionFinalizacion)
{
	while (true)
	{
		{
			// Se bloquea el mutex de la regi�n 1
			std::unique_lock<std::mutex> lock (mutexRegion1);

			// Mientras no se haya calculado un nuevo delta
			while (!nuevoDelta)
			{
				condicionRegion1.wait (lock);
			}

		} // Al salir del bloque, se libera el bloqueo sobre el mutex de la regi�n 1

		// Se comprueba si ha recibido una se�al de finalizaci�n
		if (condicionFinalizacion.wait_for (std::chrono::microseconds (100)) != std::future_status::timeout)
		{
			break;
		}

		// Se actualiza la posici�n de los conjuntos de part�culas visibles en pantalla
		for (int i = 0; i < conjuntosParticulas.size (); i++)
		{
			ConjuntoParticulas *conjuntoParticulas = conjuntosParticulas.at (i);

			conjuntoParticulas->actualizarEstado (delta);

			// Si se han agotado la vida de la part�cula, se elimina de la escena
			if (conjuntoParticulas->isMuerto ())
			{
				delete conjuntoParticulas;

				// Correcci�n del iterador para no saltarse ninguna part�cula
				i--;
			}
		}

		{
			// Se bloquea el mutex de la regi�n 2
			std::unique_lock<std::mutex> lock (mutexRegion2);

			// Se indica que el hilo auxiliar ha completado su iteraci�n
			hiloAuxiliarFinalizado = true;

			// Adem�s, �l mismo establece a falsa la condici�n de que se haya generado un nuevo delta para bloquearse
			// hasta que el hilo principal comience una nueva actualizaci�n de los objetos en la escena
			nuevoDelta = false;

			// Se despierta al hilo principal por si se ha dormido previamente esperando a que el hilo auxiliar
			// completase una iteraci�n de su funci�n de trabajo
			condicionRegion2.notify_one ();

		} // Al salir del bloque, se libera el bloqueo sobre el mutex de la regi�n 2
	}
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

	// Se guarda en la clase de part�culas una referencia al conjunto que contiene las part�culas a representar en
	// pantalla
	Particula::conjuntoParticulas = &particulas;

	// Se guarda en la clase de conjuntos de part�culas una referencia al conjunto que contiene los conjuntos de
	// part�culas a representar en pantalla
	ConjuntoParticulas::conjuntoConjuntoParticulas = &conjuntosParticulas;

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
	shaderParticulas = new Shader (vertexShaderParticulas, fragmentShaderParticulas);

	/*shaderSkybox->usar ();
	shaderSkybox->setInt ("skybox", 0);*/


	/* Configuraci�n de OpenGL */

	// Se configura OpenGL con una serie de opciones ya definidas
	Inicializador::configurarOpenGL ();


	/* Bucle de renderizado (main loop) */

	// Se almacena en el controlador la referencias a los valores a modificar
	Controlador::asteroides = &asteroides;
	Controlador::particulas = &particulas;
	Controlador::conjuntosParticulas = &conjuntosParticulas;

	// TODO eliminar estas referencias y usar la de la nave directamente
	Controlador::posicionNave = nave->getPosicionReferencia ();
	Controlador::velocidadNave = nave->getVelocidadReferencia ();
	Controlador::rotacionNave = nave->getRotacionReferencia ();
	Controlador::correcionRotNave = nave->getCorreccionRotacionReferencia ();
	Controlador::coefAceleracionNave = nave->getCoefAceleracionReferencia ();

	Controlador::nave = nave;

	// Se comienza a reproducir la m�sica de fondo en un bucle infinito
	Sonido::getSonido()->getSonido2D ()->play2D("Galactic Funk.mp3", GL_TRUE);

	// Se crea un hilo que ayude concurrentemente con la actualizaci�n de las part�culas que se encuentran en
	// conjuntos; para ello, se le indicar� una variable que debe comprobar en cada iteraci�n para saber si dene
	// finalizar su ejecuci�n
	std::promise<void> senalSalida;
	std::future<void> objetoFuturo = senalSalida.get_future ();

	std::thread hiloParticulas (actualizarParticulasConjuntos, std::move(objetoFuturo));

	std::cout << "COMENZANDO" << std::endl;
	// Mientras no se haya indicado la finalizaci�n
	while (!glfwWindowShouldClose (ventana))
	{
		/* Input */

		// Se procesa el input del usuario
		Controlador::inputTeclado (ventana);


		/* Actualizaci�n */

		// Se obtiene el tiempo transcurrido desde que se ha cargado la librer�a
		tiempoActual = (float)glfwGetTime ();

		// Se actualizar�n los objetos en pantalla en funci�n del tiempo transcurrido
		{
			// Se bloquea el mutex de la regi�n 1
			std::unique_lock<std::mutex> lock (mutexRegion1);

			// Se almacena el nuevo delta y se indica que es necesario efectuar una actualizaci�n de los objetos en
			// pantalla
			delta = tiempoActual - tiempoAnterior;
			nuevoDelta = true;

			// Se establece a falsa la condici�n de que el hilo auxiliar haya completado la iteraci�n del bucle que
			// deber� realizar; esta condici�n ser� establecida a verdadera por �l mismo, de modo que el hilo
			// principal pueda avanzar sin temor a causar carreras cr�ticas
			hiloAuxiliarFinalizado = false;

			// Se despierta al hilo auxiliar por si se ha dormido previamente esperando por una nueva actualizaci�n de
			// los objetos en pantalla
			condicionRegion1.notify_one ();

		} // Al salir del bloque, se libera el bloqueo sobre el mutex de la regi�n 1

		// Se actualiza las posici�n de la nave si no ha sido destruida
		if (!(nave->getIsDestruida ()))
		{
			nave->actualizarEstado (delta);
		}

		// Se actualizan las posiciones de los asteroides
		for (Asteroide *asteroide : asteroides)
		{
			asteroide->actualizarEstado (delta);
		}

		// NO CAMBIAR A FOR EACH PORQUE, SI SE ELIMINA UN DISPARO, PUEDE PETAR
		// Se actualizan las posiciones de los disparos
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

		// Se actualiza la posici�n de las part�culas visibles en pantalla y que no se encuentran en conjuntos
		for (int i = 0; i < particulas.size (); i++)
		{
			Particula *particula = particulas.at (i);

			particula->actualizarEstado (delta);

			// Si se han agotado la vida de la part�cula, se elimina de la escena
			if (particula->isMuerta ())
			{
				delete particula;

				// Correcci�n del iterador para no saltarse ninguna part�cula
				i--;
			}
		}


		// Se establece la nueva posici�n de la nave como la posici�n del oyente de los sonidos 3D activos; tambi�n es
		// necesaria la direcci�n en la que mira el oyente
		glm::vec3 posicionNave = nave->getPosicion ();

		glm::vec3 direccionNave = glm::vec3 (
			-sinf (glm::radians (nave->getRotacion ().y)),
			0.0f,
			-cos (glm::radians (nave->getRotacion ().y))
		);
		// cos^2(x) + sen^2(x) = 1 -> para que sea un vector unitario
		direccionNave *= direccionNave;

		Sonido::getSonido ()->actualizar (posicionNave, direccionNave);


		// Se espera a que el hilo auxiliar finalice; hasta este momento, el hilo auxiliar probablemente habr� tenido
		// suficiente entero como para ejecutar la mayor parte de su tarea mientras que el hilo principal se encargaba
		// de efectuar otras actualizaciones. La espera se realiza aqu� para evitar carreras cr�ticas entre los hilos,
		// dado que el hilo principal deber� comprobar ahora colisiones y, en caso de ser necesario, generar las
		// correspondientes part�culas de una explosi�n
		{
			// Se bloquea el mutex de la regi�n 2
			std::unique_lock<std::mutex> lock (mutexRegion2);

			// Mientras el hilo auxiliar no haya acabado
			while (!hiloAuxiliarFinalizado)
			{
				condicionRegion2.wait (lock);
			}

		} // Al salir del bloque, se libera el bloqueo sobre el mutex de la regi�n 2


		// Se comprueba si la nave ha colisionado con alg�n asteroide, en caso de que a�n no haya sido destruida
		// TODO ser�a m�s eficiente fusionar los bucles
		if (!(nave->getIsDestruida ()))
		{
			for (int i = 0; i < asteroides.size (); i++)
			{
				if (nave->checkColision (asteroides.at (i)))
				{
					// Se destruye el asteroide
					asteroides.at (i)->explotar ();

					// Se destruye la nave
					nave->explotar ();

					// Ya no ser� necesario comprobar la colisi�n con ning�n asteroide m�s
					break;
				}
			}
		}

		// Se comprueba si los disparos han colisionado con alg�n asteroide
		for (int i = 0; i < asteroides.size (); i++)
		{
			for (int j = 0; j < disparos.size (); j++)
			{
				if (disparos.at (j)->checkColision (asteroides.at (i)))
				{
					// Se destruye el asteroide
					asteroides.at (i)->explotar ();

					// Se destruyen los disparos pr�ximos al disparo que ha explotado
					std::vector<Disparo *> disparosEliminar;

					for (int k = 0; k < disparos.size (); k++)
					{
						// Si se encuentran distanciados en menos de 100 unidades (se incluye a prop�sito el propio
						// disparo que ha explotado para eliminarlo en el mismo bucle
						if (glm::distance (disparos.at (k)->getPosicion (),	disparos.at (j)->getPosicion ()) < 100.0f)
						{
							// Se marca el disparo para destrucci�n
							disparosEliminar.push_back (disparos.at (k));
						}
					}

					// Se destruyen los disparos encontrados
					while (disparosEliminar.size () > 0)
					{
						delete disparosEliminar.at (0);
						// Es necesario eliminar el disparo del vector manualmente, dado que los disparos simplemente
						// se eliminan a s� mismos en el vector global de "main.cpp"
						disparosEliminar.erase (disparosEliminar.begin());
					}

					// Correcci�n del iterador para no saltarse ning�n asteroide
					i--;

					// Ya no es necesario comprobar la colisi�n del asteroide iterado con alg�n otro disparo
					break;
				}
			}
		}


		// Tras ejecutar las actualizaciones, se almacena el momento en que se ejecutaron
		tiempoAnterior = tiempoActual;

		// Se indica que el delta actualmente guardado ya no es v�lido
		nuevoDelta = false;


		/* Render */

		// Se emplea la funci�n ya definida para renderizar un frame
		display ();


		/* Final iteraci�n */

		// Se intercambian los buffers de color delantero y trasero
		glfwSwapBuffers (ventana);

		// Se procesan eventos pendientes (entrada por teclado, redimensionado...)
		glfwPollEvents ();
	}

	// Se indica al hilo auxiliar que finalice; para ello, se activa en primer lugar la se�al de salida, y tras ello se
	// despierta env�a una se�al asociada a su condici�n de bloqueo por si se encuentra esperando a tener que realizar
	// una nueva actualizaci�n de los objetos en pantalla
	senalSalida.set_value ();
	nuevoDelta = true;
	condicionRegion1.notify_one ();

	// Se destruyen los objetos creados y sus modelos
	Forma::destruirFormas ();

	delete nave;
	delete modeloNave;

	while (particulas.size () > 0)
	{
		// Las part�culas que no pertenencen a conjuntos se eliminan autom�ticamente del vector
		delete particulas.at (0);
	}

	while (conjuntosParticulas.size () > 0)
	{
		// Las conjuntos de part�culas se eliminan autom�ticamente del vector
		delete conjuntosParticulas.at (0);
	}

	// Se eliminan los asteroides en la escena
	while (asteroides.size () > 0)
	{
		// Los asteroides se eliminan autom�ticamente del vector
		delete asteroides.at (0);
	}

	delete modeloAsteroide;

	delete luz;

	delete skybox;
	

	// Se destruyen los shaders
	delete shader;
	delete shaderColor;
	delete shaderSkybox;
	delete shaderParticulas;

	// Se destruyen los reproductores de sonido
	Sonido::getSonido ()->~Sonido ();


	// Se espera a que finalice el hilo auxiliar
	hiloParticulas.join ();

	// Se finaliza la ejecuci�n
	return(0);
}
