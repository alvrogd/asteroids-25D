﻿// Valor de PI
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

// Sonido
#include <irrKlang.h>

// Ejecución multihilo
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

// Para la reproducción de sonido
#include "Sonido.h"

// Para conjuntos de datos
#include <vector>

// Para la generación de números aleatorios
#include <ctime>


// Mútexes y variables de condición mediante las cuales emular barreras (estas últimas no se encuentran disponibles
// en C++):
//
//	- Región 1: el hilo auxiliar esperará en ella hasta que el hilo principal comience una nueva iteración del main
//	            loop.
//	- Región 2: el hilo principal esperará en ella hasta que el hilo auxiliar realice una iteración de la función de
//				actualización de partículas, de modo que se sincronicen para evitar carreras críticas.
std::mutex mutexRegion1;
std::mutex mutexRegion2;
std::condition_variable condicionRegion1;
std::condition_variable condicionRegion2;


// Tiempo transcurrido entre frames; se actualizan en cada iteración del bucle principal
float tiempoAnterior = 0;
float tiempoActual = 0;
float delta = 0;
// Variables auxiliares empleadas para la sincronización entre los dos hilos usados
bool nuevoDelta = false;
bool hiloAuxiliarFinalizado = false;

// Si existe temblor y cuándo se activó uno por última vez
bool hayTemblor = false;
float ultimoTemblor = 0.0f;


// Propiedades de la ventana a crear
int wWidth = 800;
int wHeight = 800;

// Relación de aspecto de la ventana
float relacionAspecto = (float)wWidth / (float)wHeight;

// FOV de la cámara
const double FOV = 80.0;

// Distancia máxima a la que renderizar los objetos de la escena
double distanciaMaximaRenderizado = 100.0;


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

	// Se empleará toda la ventana en el renderizado
	glViewport (0, 0, wWidth, wHeight);

	// Se representarán los cuerpos con relleno
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);


	// Se calculan las matrices de proyección y de visionado

	// Se aplica una proyección en perspectiva
	glm::mat4 projectionMatrix = glm::perspective (glm::radians (FOV), (double)wWidth / (double)wHeight, 0.1,
		distanciaMaximaRenderizado);

	// La matriz de visionado la calcula en control puesto que la cámara es controlada por el input del usuario
	glm::mat4 viewMatrix;
	glm::vec3 posicionCamara;
	Controlador::calcularViewMatrix (viewMatrix, posicionCamara);

	// Se inicializa la matriz base de los modelos a la identidad
	glm::mat4 modelMatrixBase = glm::mat4 (1.0f);

	// Se aplica un temblor a la matriz base de los modelos si es necesario
	if (hayTemblor)
	{
		modelMatrixBase = glm::translate (modelMatrixBase, glm::vec3 (
			cosf (tiempoActual * 10.0f) * 2.0f,
			-sinf (tiempoActual * 10.0f) * 3.5f,
			-cosf (tiempoActual * 10.0f) * 2.7f
		));
	}

	// Se aplican las matrices al shader de los modelos
	shader->usar ();
	shader->setMat4 ("projectionMatrix", projectionMatrix);
	shader->setMat4 ("viewMatrix", viewMatrix);

	// Además, es necesario cargar las luces presentes en la escena
	luz->cargar (shader, glm::mat4(1.0f), 0);
	
	// Ya se ha cargado la luz principal
	int numLuces = 1;

	// Es necesario considerar el límite de luces del shader
	int max = std::min (disparos.size (), (size_t) 50);
	for (int i = 0; i < max; i++)
	{
		// Se carga la luz del disparo iterado
		disparos.at (i)->getLuz ()->cargar (shader, glm::mat4 (1.0f), numLuces);

		// Se incrementa el número de luces cargadas
		numLuces++;
	}

	// Se indica el número de luces cargadas
	shader->setInt ("numPuntosLuz", numLuces);

	// Y la posición de la cámara para el cálculo de la iluminación
	shader->setVec3 ("posicionCamara", posicionCamara);

	// Se representa la nave si aún no ha sido destruida
	if (!(nave->getIsDestruida ()))
	{
		nave->dibujar (modelMatrixBase, shader);
	}

	// Se representan todos los asteroides
	for (Asteroide *asteroide : asteroides)
	{
		asteroide->dibujar (modelMatrixBase, shader);
	}

	// Se carga el shader de color y se le aplican las matrices
	shaderColor->usar ();
	shaderColor->setMat4 ("projectionMatrix", projectionMatrix);
	shaderColor->setMat4 ("viewMatrix", viewMatrix);

	// Se representan todos los disparos
	for (Disparo *disparo : disparos)
	{
		disparo->dibujar (modelMatrixBase, shaderColor);
	}

	// Se carga el shader de partículas y se aplican las matrices
	shaderParticulas->usar ();
	shaderParticulas->setMat4 ("projectionMatrix", projectionMatrix);
	shaderParticulas->setMat4 ("viewMatrix", viewMatrix);

	// Se representan todas las partículas, tanto las individuales como las contenidas en conjuntos de partículas
	for (Particula *particula : particulas)
	{
		particula->dibujar (modelMatrixBase, shaderParticulas);
	}

	for (ConjuntoParticulas *conjuntoParticulas : conjuntosParticulas)
	{
		conjuntoParticulas->dibujar (modelMatrixBase, shaderParticulas);
	}
	
	// Ahora se carga el shader de la skybox y se aplican también las matrices necesarias
	shaderSkybox->usar ();
	shaderSkybox->setMat4 ("projectionMatrix", projectionMatrix);
	shaderSkybox->setMat4 ("viewMatrix", glm::mat4 (glm::mat3 (viewMatrix)));

	// Y se representa la skybox
	skybox->dibujar (shaderSkybox);
}


/*
 * Se actualiza la posición de las partículas representadas en la escena que pertenecen a conjuntos y, si se les agota
 * el tiempo de vida, son eliminadas
 */
void actualizarParticulasConjuntos (std::future<void> condicionFinalizacion)
{
	// El hilo auxiliar se ejecutará hasta que el hilo principal le indique que debe finalizar para acabar el programa
	while (true)
	{
		{
			// Se bloquea el mutex de la región 1
			std::unique_lock<std::mutex> lock (mutexRegion1);

			// Mientras no se haya calculado un nuevo delta
			while (!nuevoDelta)
			{
				condicionRegion1.wait (lock);
			}

		} // Al salir del bloque, se libera el bloqueo sobre el mutex de la región 1

		// Se comprueba si ha recibido una señal de finalización
		if (condicionFinalizacion.wait_for (std::chrono::microseconds (100)) != std::future_status::timeout)
		{
			break;
		}

		// Se actualiza la posición de los conjuntos de partículas visibles en pantalla
		for (unsigned int i = 0; i < conjuntosParticulas.size (); i++)
		{
			ConjuntoParticulas *conjuntoParticulas = conjuntosParticulas.at (i);

			conjuntoParticulas->actualizarEstado (delta);

			// Si se han agotado la vida de la partícula, se elimina de la escena
			if (conjuntoParticulas->isMuerto ())
			{
				delete conjuntoParticulas;

				// Corrección del iterador para no saltarse ninguna partícula
				i--;
			}
		}

		{
			// Se bloquea el mutex de la región 2
			std::unique_lock<std::mutex> lock (mutexRegion2);

			// Se indica que el hilo auxiliar ha completado su iteración
			hiloAuxiliarFinalizado = true;

			// Además, él mismo establece a falsa la condición de que se haya generado un nuevo delta para bloquearse
			// hasta que el hilo principal comience una nueva actualización de los objetos en la escena
			nuevoDelta = false;

			// Se despierta al hilo principal por si se ha dormido previamente esperando a que el hilo auxiliar
			// completase una iteración de su función de trabajo
			condicionRegion2.notify_one ();

		} // Al salir del bloque, se libera el bloqueo sobre el mutex de la región 2
	}
}


int main (int argc, char **argv) {

	// Se obtiene una semilla en función del tiempo para la generación de números aleatorios
	std::srand (std::time (NULL));


	/* Inicialización de OpenGL */

	GLFWwindow *ventana = NULL;
	Inicializador::inicializarOpenGL (&ventana, "Asteroids 2.5D", &wWidth, &wHeight, &relacionAspecto);


	/* Generación de los objetos */

	// Se inicializan las formas básicas
	Forma::inicializarFormas ();

	// Se establece el warp de los objetos móviles, permitiendo que se muevan dentro de un cuadrado de lado 1000
	// IMPORTANTE ESTABLECERLO ANTES DE GENERAR UN ASTEROIDE
	Movil::coordenadasWarp = glm::vec3 (2000.0f, 2000.0f, 2000.0f);

	// La distancia máxima de renderizado debe ser el mayor que el warp máximo establecido para los móviles respecto
	// al origen de coordenadas (0, 0, 0), para poder ver así todos los objetos de la escena aunque el jugador se
	// encuentre en una punta de la escena. Además debe considerarse que, al ser el área cuadrada, el jugador podría
	// encontrarse en una esquina y, de no hacerse, los objetos de la esquina contraria no se podrían ver si tan solo
	// se establece la distancia máxima al doble del warp máximo.
	distanciaMaximaRenderizado = std::sqrt(2.0 * 4000.0 * 4000.0);

	// Se guarda en la clase de asteroides una referencia al conjunto que contiene los asteroides a representar en
	// pantalla
	Asteroide::conjuntoAsteroides = &asteroides;

	// Se guarda en la clase de disparos una referencia al conjunto que contiene los disparos a representar en
	// pantalla
	Disparo::conjuntoDisparos = &disparos;

	// Se guarda en la clase de partículas una referencia al conjunto que contiene las partículas a representar en
	// pantalla
	Particula::conjuntoParticulas = &particulas;

	// Se guarda en la clase de conjuntos de partículas una referencia al conjunto que contiene los conjuntos de
	// partículas a representar en pantalla
	ConjuntoParticulas::conjuntoConjuntoParticulas = &conjuntosParticulas;

	// Se cargan los modelos necesarios
	modeloNave = new Modelo ("Viper-mk-IV-fighter.obj");
	modeloAsteroide = new Modelo ("rock_by_dommk.obj");

	// Se almacena una referencia al modelo que emplerán los asteroides
	Asteroide::modelo = modeloAsteroide;

	// La nave mira inicialmente hacia el eje -X, por lo que se establece la rotación inicial correspondiente para
	// corregirlo de cara al cálculo de su movimiento
	nave = new Nave (glm::vec3 (1.0f, 1.0f, 1.0f), modeloNave, 8.0f, glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f,
		0.0f), glm::vec3 (0.5f, 0.5f, 0.5f), glm::vec3 (0.02f, 0.02f, 0.02f), glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -90.0f, 0.0f));

	// Se crean 100 asteroides de forma aleatoria
	for (int i = 0; i < 100; i++)
	{
		asteroides.push_back (new Asteroide ());
	}

	// Se crea una pequeña luz que ilumine la escena, principalmente, de color blanco
	luz = new PuntoLuz (glm::vec3 (0.0f, 30.0f, 30.0f), 1.0f, 0.000028f, 0.00000014f, glm::vec3 (0.5f, 0.5f, 0.5f),
		glm::vec3 (1.0f, 1.0f, 1.0f), glm::vec3 (1.0f, 1.0f, 1.0f));

	// Se crea un skybox para dar la sensación de encontrarse en medio del espacio
	skybox = new Cubemap (caras);


	/* Compilación de los shaders */
	
	shader = new Shader (vertexShaderModelos, fragmentShaderModelos);
	shaderColor = new Shader (vertexShaderColor, fragmentShaderColor);
	shaderSkybox = new Shader (vertexShaderSkybox, fragmentShaderSkybox);
	shaderParticulas = new Shader (vertexShaderParticulas, fragmentShaderParticulas);


	/* Configuración de OpenGL */

	// Se configura OpenGL con una serie de opciones ya definidas
	Inicializador::configurarOpenGL ();


	/* Bucle de renderizado (main loop) */

	// Se almacena en el controlador la referencias a los valores que necesita para su correcto funcionamiento
	Controlador::asteroides = &asteroides;
	Controlador::particulas = &particulas;
	Controlador::conjuntosParticulas = &conjuntosParticulas;
	Controlador::setNave (nave);

	// Se comienza a reproducir la música de fondo en un bucle infinito
	Sonido::getSonido()->getSonido2D ()->play2D("Galactic Funk.ogg", GL_TRUE);

	// Se crea un hilo que ayude concurrentemente con la actualización de las partículas que se encuentran en
	// conjuntos; para ello, se le indicará una variable que debe comprobar en cada iteración para saber si debe
	// finalizar su ejecución
	std::promise<void> senalSalida;
	std::future<void> objetoFuturo = senalSalida.get_future ();

	std::thread hiloParticulas (actualizarParticulasConjuntos, std::move(objetoFuturo));


	std::cout << std::endl << "========== Comenzando el juego ==========" << std::endl << std::endl;


	// Mientras no se haya indicado la finalización del programa
	while (!glfwWindowShouldClose (ventana))
	{
		/* Input */

		// Se procesa el input del usuario
		Controlador::inputTeclado (ventana);


		/* Actualización */

		// Se obtiene el tiempo transcurrido desde que se ha cargado la librería
		tiempoActual = (float)glfwGetTime ();

		// Se actualizarán los objetos en pantalla en función del tiempo transcurrido
		{
			// Se bloquea el mutex de la región 1
			std::unique_lock<std::mutex> lock (mutexRegion1);

			// Se almacena el nuevo delta y se indica que es necesario efectuar una actualización de los objetos en
			// pantalla
			delta = tiempoActual - tiempoAnterior;
			nuevoDelta = true;

			// Se establece a falsa la condición de que el hilo auxiliar haya completado la iteración del bucle que
			// deberá realizar; esta condición será establecida a verdadera por él mismo, de modo que el hilo
			// principal pueda avanzar sin temor a causar carreras críticas
			hiloAuxiliarFinalizado = false;

			// Se despierta al hilo auxiliar por si se ha dormido previamente esperando por una nueva actualización de
			// los objetos en pantalla
			condicionRegion1.notify_one ();

		} // Al salir del bloque, se libera el bloqueo sobre el mutex de la región 1

		// Se actualiza las posición de la nave si no ha sido destruida
		if (!(nave->getIsDestruida ()))
		{
			nave->actualizarEstado (delta);
		}

		// Se actualizan las posiciones de los asteroides
		for (Asteroide *asteroide : asteroides)
		{
			asteroide->actualizarEstado (delta);
		}

		// NO SE PUEDE EMPLEAR UN BLOQUE "FOR EACH" PORQUE, SI SE DESTRUYE UNO DE LOS ÍTEMS ITERADOS, ESTE SERÁ
		// ELIMINADO TAMBIÉN DEL CONJUNTO DE DATOS ITERADO Y CAUSARÁ UN ERROR

		// Se actualizan las posiciones de los disparos
		for (unsigned int i = 0; i < disparos.size(); i++)
		{
			Disparo *disparo = disparos.at (i);

			disparo->actualizarEstado (delta);

			// Si se han agotado los warps restantes, se elimina el disparo de la escena
			if (disparo->getWarpsRestantes () < 0)
			{
				delete disparo;

				// Corrección del iterador para no saltarse ningún disparo
				i--;
			}
		}

		// Se actualiza la posición de las partículas visibles en pantalla y que no se encuentran en conjuntos
		for (unsigned int i = 0; i < particulas.size (); i++)
		{
			Particula *particula = particulas.at (i);

			particula->actualizarEstado (delta);

			// Si se han agotado la vida de la partícula, se elimina de la escena
			if (particula->isMuerta ())
			{
				delete particula;

				// Corrección del iterador para no saltarse ninguna partícula
				i--;
			}
		}


		// Se establece la nueva posición de la nave como la posición del oyente de los sonidos 3D activos; también es
		// necesaria la dirección en la que mira el oyente
		glm::vec3 posicionNave = nave->getPosicion ();

		glm::vec3 direccionNave = glm::vec3 (
			-sinf (glm::radians (nave->getRotacion ().y)),
			0.0f,
			-cos (glm::radians (nave->getRotacion ().y))
		);
		// cos^2(x) + sen^2(x) = 1 -> para que sea un vector unitario
		direccionNave *= direccionNave;

		Sonido::getSonido ()->actualizar (posicionNave, direccionNave);


		// Se comprueba si la nave ha colisionado con algún asteroide, en caso de que aún no haya sido destruida
		bool hayColision = false;
		Asteroide *asteroideChoque = NULL;

		if (!(nave->getIsDestruida ()))
		{
			for (unsigned int i = 0; i < asteroides.size (); i++)
			{
				if (nave->checkColision (asteroides.at (i)))
				{
					// Se marca el asteroide para destrucción
					asteroideChoque = asteroides.at (i);

					// Se indica que ha habido una colisión
					hayColision = true;

					// Ya no será necesario comprobar la colisión con ningún asteroide más
					break;
				}
			}
		}

		// Se espera a que el hilo auxiliar finalice; hasta este momento, el hilo auxiliar probablemente habrá tenido
		// suficiente entero como para ejecutar la mayor parte de su tarea mientras que el hilo principal se encargaba
		// de efectuar otras actualizaciones. La espera se realiza aquí para evitar carreras críticas entre los hilos,
		// dado que el hilo principal deberá comprobar ahora una gran cantidad de colisiones y, en caso de ser
		// necesario, generar las correspondientes partículas de una explosión.
		//
		// La comprobación de colisiones entre la nave y los asteroides se hace a propósito en el anterior bucle,
		// pero, en vez de manejarlas al instante, simplemente se marca el asteroide que puede haber colisionado para
		// llevar a cabo las acciones de una colisión tras asegurarse que no se producirán carreras críticas. De este
		// modo, se aumenta la cantidad de trabajo que los dos hilos pueden realizar de forma paralela.
		//
		// En el siguiente bucle tan solo estará activo el hilo principal dada la complejidad de intentar permitir que
		// los dos hilos se ejecuten en paralelo mientras el principal se encuentre en dicho bucle.
		{
			// Se bloquea el mutex de la región 2
			std::unique_lock<std::mutex> lock (mutexRegion2);

			// Mientras el hilo auxiliar no haya acabado
			while (!hiloAuxiliarFinalizado)
			{
				condicionRegion2.wait (lock);
			}

		} // Al salir del bloque, se libera el bloqueo sobre el mutex de la región 2

		// Inicialmente, se presupone que en la actualización no se ha generado un nuevo temblor
		bool generarTemblor = false;

		// Si ha habido una colisión de la nave con los asteroides, se lleva a cabo
		if (hayColision)
		{
			// Se destruye el asteroide que ha colisionado
			asteroideChoque->explotar ();

			// Y se destruye la nave
			nave->explotar ();

			// Se aplica un efecto de temblor a la escena
			generarTemblor = true;
		}

		// Se comprueba si los disparos han colisionado con algún asteroide
		for (unsigned int i = 0; i < asteroides.size (); i++)
		{
			for (unsigned int j = 0; j < disparos.size (); j++)
			{
				if (disparos.at (j)->checkColision (asteroides.at (i)))
				{
					// Se destruye el asteroide
					asteroides.at (i)->explotar ();

					// Se destruyen los disparos próximos al disparo que ha explotado
					std::vector<Disparo *> disparosEliminar;

					for (unsigned int k = 0; k < disparos.size (); k++)
					{
						// Si se encuentran distanciados en menos de 100 unidades (se incluye a propósito el propio
						// disparo que ha explotado para eliminarlo en el mismo bucle
						if (glm::distance (disparos.at (k)->getPosicion (),	disparos.at (j)->getPosicion ()) < 100.0f)
						{
							// Se marca el disparo para destrucción
							disparosEliminar.push_back (disparos.at (k));
						}
					}

					// Se destruyen los disparos encontrados
					while (disparosEliminar.size () > 0)
					{
						delete disparosEliminar.at (0);
						// Es necesario eliminar el disparo del vector manualmente, dado que los disparos simplemente
						// se eliminan a sí mismos en el vector global de "main.cpp"
						disparosEliminar.erase (disparosEliminar.begin());
					}

					// Se aplica un efecto de temblor a la escena
					generarTemblor = true;

					// Corrección del iterador para no saltarse ningún asteroide
					i--;

					// Ya no es necesario comprobar la colisión del asteroide iterado con algún otro disparo
					break;
				}
			}
		}


		// Tras ejecutar las actualizaciones, se almacena el momento en que se ejecutaron
		tiempoAnterior = tiempoActual;

		// Y, si se ha producido un temblor, se registra cuándo
		if (generarTemblor)
		{
			hayTemblor = true;
			ultimoTemblor = tiempoActual;

		}

		// En caso contrario, se desactiva el temblor si lleva más de 120 ms activo
		else if (tiempoActual - ultimoTemblor > 0.12f)
		{
			hayTemblor = false;
		}


		/* Render */

		// Se emplea la función ya definida para renderizar un frame
		display ();


		/* Final iteración */

		// Se intercambian los buffers de color delantero y trasero
		glfwSwapBuffers (ventana);

		// Se procesan eventos pendientes (entrada por teclado, redimensionado...)
		glfwPollEvents ();
	}

	// Se indica al hilo auxiliar que finalice; para ello, se activa en primer lugar la señal de salida, y tras ello
	// se envía una señal asociada a su condición de bloqueo por si se encuentra esperando a tener que realizar una
	// nueva actualización de los objetos en pantalla
	senalSalida.set_value ();
	// Es importante establecer la condición "nuevoDelta" a verdadera para que, cuando se envíe la señal asociada a la
	// variable de condición por la cual el hilo auxiliar se ha bloqueado, este no vuelva a bloquearse y continúe para
	// finalizar su ejecución
	nuevoDelta = true;
	condicionRegion1.notify_one ();

	// Se destruyen los objetos creados y sus modelos
	Forma::destruirFormas ();

	delete nave;
	delete modeloNave;

	while (particulas.size () > 0)
	{
		// Las partículas que no pertenencen a conjuntos se eliminan automáticamente del vector
		delete particulas.at (0);
	}

	while (conjuntosParticulas.size () > 0)
	{
		// Las conjuntos de partículas se eliminan automáticamente del vector
		delete conjuntosParticulas.at (0);
	}

	// Se eliminan los asteroides en la escena
	while (asteroides.size () > 0)
	{
		// Los asteroides se eliminan automáticamente del vector
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

	// Se finaliza la ejecución
	exit(EXIT_SUCCESS);
}
