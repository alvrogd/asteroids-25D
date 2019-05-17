#include "Controlador.h"

#include <iostream>


/* Propiedades de la ventana */

int *Controlador::wWidth = NULL;
int *Controlador::wHeight = NULL;

float *Controlador::relacionAspecto = NULL;


/* Objetos presentes en la escena */

std::vector<Asteroide *> *Controlador::asteroides = NULL;
std::vector<Particula *> *Controlador::particulas = NULL;
std::vector<ConjuntoParticulas *> *Controlador::conjuntosParticulas = NULL;

Nave *Controlador::nave = NULL;
glm::vec3 *Controlador::posicionNave = NULL;
glm::vec3 *Controlador::velocidadNave = NULL;
glm::vec3 *Controlador::rotacionNave = NULL;
glm::vec3 *Controlador::correcionRotNave = NULL;
glm::vec3 *Controlador::coefAceleracionNave = NULL;


/* Control del input del usuario */

bool Controlador::botonDisparoSoltado = true;
int Controlador::numDisparosPulsacion = 0;

bool Controlador::botonResetSoltado = true;


/* Modo de la cámara */

int Controlador::modoCamara = 1;


// Control de la cámara
void Controlador::calcularViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &posicionCamara)
{
	// Se sitúa la cámara por detrás de la nave y sobre ella
	switch (Controlador::modoCamara)
	{
		// Cercana
		case 0:
			viewMatrix = glm::lookAt (
				glm::vec3 (posicionNave->x + sinf (glm::radians (rotacionNave->y)) * 60.0f,
					20.0f,
					posicionNave->z + cosf (glm::radians (rotacionNave->y)) * 60.0f),
				glm::vec3 (posicionNave->x, posicionNave->y, posicionNave->z),
				glm::vec3 (0.0f, 1.0f, 0.0f));
			break;

		// Alejada
		case 1:
		default:
			viewMatrix = glm::lookAt (
				glm::vec3 (posicionNave->x + sinf (glm::radians (rotacionNave->y)) * 300.f,
					160.0f,
					posicionNave->z + cosf (glm::radians (rotacionNave->y)) * 300.0f),
				glm::vec3 (posicionNave->x, posicionNave->y, posicionNave->z),
				glm::vec3 (0.0f, 1.0f, 0.0f));
			break;
	}
}

void Controlador::inputTeclado (GLFWwindow *ventana)
{
	// ESC = salir del juego
	if (glfwGetKey (ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// Se indica a la ventana que se cierre
		glfwSetWindowShouldClose (ventana, true);
	}

	// Si la nave no ha sido destruida
	if (!(nave->getIsDestruida ()))
	{
		// W = acelerar la nave
		if (glfwGetKey (ventana, GLFW_KEY_W) == GLFW_PRESS)
		{
			// Se añade velocidad a la nave en función de la dirección a la que apunta
			velocidadNave->x -= sinf (glm::radians (rotacionNave->y)) * coefAceleracionNave->x;
			velocidadNave->z -= cosf (glm::radians (rotacionNave->y)) * coefAceleracionNave->z;

			// Y se le indica que se encuentra acelerando
			nave->setIsAcelerando (true);
		}

		else
		{
			// Se indica a la nave que no se encuentra acelerando
			nave->setIsAcelerando (false);
		}

		// A = maniobrar hacia la izquierda
		if (glfwGetKey (ventana, GLFW_KEY_A) == GLFW_PRESS)
		{
			// Se incrementa el ángulo de rotación de la nave en el eje Y
			rotacionNave->y += 2.0f;
		}

		// D = maniobrar hacia la derecha
		else if (glfwGetKey (ventana, GLFW_KEY_D) == GLFW_PRESS)
		{
			// Se reduce el ángulo de rotación de la nave en el eje Y
			rotacionNave->y -= 2.0f;
		}

		// Espacio presionado = disparar
		if (glfwGetKey (ventana, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			// Si la barra espaciadora se encontraba suelta previamente o aún no se ha alcanzado el máximo de disparos en
			// una pulsación
			if (Controlador::botonDisparoSoltado || Controlador::numDisparosPulsacion < 4)
			{
				// Se indica a la nave que debe disparar
				Controlador::nave->disparar ();

				// Se indica que se ha presionado la barra espaciadora
				Controlador::botonDisparoSoltado = false;

				// Y se incrementa el número de disparos en una pulsación de la barra espaciadora
				Controlador::numDisparosPulsacion++;
			}
		}

		// Espacio soltado
		else
		{
			// Se indica que se ha soltado el botón de disparo
			Controlador::botonDisparoSoltado = true;

			// Y se resetea el número de disparos realizados en una pulsación
			Controlador::numDisparosPulsacion = 0;
		}
	}

	// R(eset) = reiniciar el juego
	if (glfwGetKey (ventana, GLFW_KEY_R) == GLFW_PRESS)
	{
		// Si el correspondiente botón se encontraba suelto previamente
		if (Controlador::botonResetSoltado)
		{
			// Se eliminan las partículas presentes en la escena (no es posible efectuar un "clear" del vector
			// correspondiente porque la memoria usada por los objetos no será liberada)
			while (Controlador::particulas->size () > 0)
			{
				// Las partículas que no pertenencen a conjuntos se eliminan automáticamente del vector
				delete Controlador::particulas->at (0);
			}

			while (Controlador::conjuntosParticulas->size () > 0)
			{
				// Las conjuntos de partículas se eliminan automáticamente del vector
				delete Controlador::conjuntosParticulas->at (0);
			}

			// Se eliminan los asteroides en la escena
			while (Controlador::asteroides->size () > 0)
			{
				// Los asteroides se eliminan automáticamente del vector
				delete Controlador::asteroides->at (0);
			}

			// Se crean 100 asteroides
			for (int i = 0; i < 100; i++)
			{
				Controlador::asteroides->push_back (new Asteroide ());
			}

			// Se vuelve a representar la nave
			nave->setIsDestruida (false);

			// Se resetea a 0 el número de disparos consecutivos realizados
			Controlador::numDisparosPulsacion = 0;

			// Se indica que se ha presionado el botón de reset
			Controlador::botonResetSoltado = false;
		}
	}

	// R(eset) soltado
	else
	{
		// Se indica que se ha soltado el botón de reseteo de la partida
		Controlador::botonResetSoltado = true;
	}

	// F1 = cámara cercana
	if (glfwGetKey (ventana, GLFW_KEY_F1) == GLFW_PRESS)
	{
		// Se cambia el modo de la cámara
		Controlador::modoCamara = 0;
	}

	// F2 = cámara lejana
	else if(glfwGetKey (ventana, GLFW_KEY_F2) == GLFW_PRESS)
	{
		// Se cambia el modo de la cámara
		Controlador::modoCamara = 1;
	}
}

void Controlador::redimensionarVentana (GLFWwindow *ventana, int ancho, int alto)
{
	// Variables en las que almacenar las dimensiones del nuevo viewport
	int anchoRender;
	int altoRender;

	// Si hay más ancho disponible que alto necesario
	if ((float)ancho / (float)alto > *(Controlador::relacionAspecto))
	{
		// El factor limitante es la altura
		altoRender = alto;
		anchoRender = *(Controlador::relacionAspecto) * altoRender;
	}

	// Si hay más alto disponible que ancho necesario
	else if ((float)ancho / (float)alto < *(Controlador::relacionAspecto))
	{
		// El factor limitante es la anchura
		anchoRender = ancho;
		altoRender = anchoRender / *(Controlador::relacionAspecto);
	}

	// En caso contrario, la relación de aspecto se mantiene
	else
	{
		anchoRender = ancho;
		altoRender = alto;
	}

	// Se actualiza el viewport teniendo en cuenta el espacio que puede quedar libre tanto a lo ancho como a lo alto
	glViewport ((float)(ancho - anchoRender) / 2, (float)(alto - altoRender) / 2, anchoRender, altoRender);

	// Se actualizan las variables que contienen el tamaño de la ventana
	*(Controlador::wWidth) = ancho;
	*(Controlador::wHeight) = alto;
}
