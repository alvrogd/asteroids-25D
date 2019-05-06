#include "Controlador.h"

#include <iostream>


int *Controlador::wWidth = NULL;
int *Controlador::wHeight = NULL;

float *Controlador::relacionAspecto = NULL;

int Controlador::modoCamara = 4;

std::vector<Asteroide *> *Controlador::asteroides = NULL;

glm::vec3 *Controlador::posicionNave = NULL;
glm::vec3 *Controlador::velocidadNave = NULL;
glm::vec3 *Controlador::rotacionNave = NULL;
glm::vec3 *Controlador::correcionRotNave = NULL;
glm::vec3 *Controlador::coefAceleracionNave = NULL;

Nave *Controlador::nave = NULL;

bool Controlador::botonDisparoSoltado = true;

int Controlador::numDisparosPulsacion = 0;


// Control de la cámara
void Controlador::calcularViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &posicionCamara)
{
	/*viewMatrix = glm::lookAt (glm::vec3 (5.5f, 0.0f, 0.5f), glm::vec3 (0.0f, 0.0f, 0.0f),
		glm::vec3 (0.0f, 1.0f, 0.0f));

	posicionCamara = glm::vec3 (5.5f, 0.0f, 0.5f);*/

	viewMatrix = glm::lookAt (
		glm::vec3 (posicionNave->x + sinf (glm::radians (rotacionNave->y)) * 160.f,
			80.0f,
			posicionNave->z + cosf (glm::radians (rotacionNave->y)) * 160.0f),
		glm::vec3 (posicionNave->x, posicionNave->y, posicionNave->z),
		glm::vec3 (0.0f, 1.0f, 0.0f));
	/*
	gluLookAt (posicionX + sinf (direccion) * 0.7, posicionY + 0.5, posicionZ + cosf (direccion) * 0.7, posicionX
		- sinf (direccion) * 0.7, posicionY, posicionZ - cosf (direccion) * 0.7, 0.0, 1.0, 0.0);*/

}

void Controlador::inputTeclado (GLFWwindow *ventana)
{
	// ESC
	if (glfwGetKey (ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// Se indica a la ventana que se cierre
		glfwSetWindowShouldClose (ventana, true);
	}

	// W
	if (glfwGetKey (ventana, GLFW_KEY_W) == GLFW_PRESS)
	{
		// Se añade velocidad a la nave en función de la dirección a la que apunta
		velocidadNave->x -= sinf (glm::radians (rotacionNave->y)) * coefAceleracionNave->x;
		velocidadNave->z -= cosf (glm::radians(rotacionNave->y)) * coefAceleracionNave->z;
		//std::cout << velocidadNave->x << velocidadNave->z << std::endl;
	}

	// A
	if (glfwGetKey (ventana, GLFW_KEY_A) == GLFW_PRESS)
	{
		// Se incrementa el ángulo de rotación de la nave en el eje Y
		rotacionNave->y += 2.0f;
		//std::cout << rotacionNave->y << std::endl;
	}

	// D
	if (glfwGetKey (ventana, GLFW_KEY_D) == GLFW_PRESS)
	{
		// Se reduce el ángulo de rotación de la nave en el eje Y
		rotacionNave->y -= 2.0f;
		//std::cout << rotacionNave->y << std::endl;
	}

	// Espacio presionado
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

	// R
	if (glfwGetKey (ventana, GLFW_KEY_R) == GLFW_PRESS)
	{
		// Se crean 100 asteroides
		for (int i = 0; i < 100; i++)
		{
			Controlador::asteroides->push_back (new Asteroide ());
		}
	}

	// C
	if (glfwGetKey (ventana, GLFW_KEY_C) == GLFW_PRESS)
	{
		// Se eliminan los asteroides en la escena
		Controlador::asteroides->clear ();
	}

	/*// F1
	else if (glfwGetKey (ventana, GLFW_KEY_F1) == GLFW_PRESS)
	{
		Controlador::modoCamara = 1;
	}

	// F2
	else if (glfwGetKey (ventana, GLFW_KEY_F2) == GLFW_PRESS)
	{
		Controlador::modoCamara = 2;
	}

	// F3
	else if (glfwGetKey (ventana, GLFW_KEY_F3) == GLFW_PRESS)
	{
		Controlador::modoCamara = 3;
	}

	// F4
	else if (glfwGetKey (ventana, GLFW_KEY_F4) == GLFW_PRESS)
	{
		Controlador::modoCamara = 4;
	}

	// F5
	else if (glfwGetKey (ventana, GLFW_KEY_F5) == GLFW_PRESS)
	{
		Controlador::modoCamara = 5;
	}

	// F6
	else if (glfwGetKey (ventana, GLFW_KEY_F6) == GLFW_PRESS)
	{
		Controlador::modoCamara = 6;
	}

	// F7
	else if (glfwGetKey (ventana, GLFW_KEY_F7) == GLFW_PRESS)
	{
		Controlador::modoCamara = 7;
	}

	// F8
	else if (glfwGetKey (ventana, GLFW_KEY_F8) == GLFW_PRESS)
	{
		Controlador::modoCamara = 8;
	}

	// F9
	else if (glfwGetKey (ventana, GLFW_KEY_F9) == GLFW_PRESS)
	{
		Controlador::modoCamara = 9;
	}

	// F10
	else if (glfwGetKey (ventana, GLFW_KEY_F10) == GLFW_PRESS)
	{
		Controlador::modoCamara = 10;
	}

	// F11
	else if (glfwGetKey (ventana, GLFW_KEY_F11) == GLFW_PRESS)
	{
		Controlador::modoCamara = 11;
	}*/
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

