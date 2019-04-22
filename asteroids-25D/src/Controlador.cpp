#include "Controlador.h"
#include <iostream>

int *Controlador::wWidth = NULL;
int *Controlador::wHeight = NULL;

float *Controlador::relacionAspecto = NULL;

int Controlador::modoCamara = 4;

float *Controlador::multiplicador = NULL;

bool *Controlador::orbitasAstros = NULL;

Astro *Controlador::sol = NULL;
Astro *Controlador::mercurio = NULL;
Astro *Controlador::venus = NULL;
Astro *Controlador::tierra = NULL;
Astro *Controlador::iss = NULL;
Astro *Controlador::luna = NULL;
Astro *Controlador::marte = NULL;
Astro *Controlador::jupiter = NULL;
Astro *Controlador::saturno = NULL;
Astro *Controlador::urano = NULL;
Astro *Controlador::neptuno = NULL;


// Control de la cámara
void Controlador::calcularViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &posicionCamara)
{
	viewMatrix = glm::lookAt (glm::vec3 (25.0f, 5.0f, 15.0f), glm::vec3 (0.0f, 5.f, 0.0f),
		glm::vec3 (0.0f, 1.0f, 0.0f));
	posicionCamara = glm::vec3 (25.0f, 5.0f, 15.0f);
}

void Controlador::inputTeclado (GLFWwindow *ventana)
{
	// ESC
	if (glfwGetKey (ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// Se indica a la ventana que se cierre
		glfwSetWindowShouldClose (ventana, true);
	}

	// +
	else if (glfwGetKey (ventana, GLFW_KEY_KP_ADD) == GLFW_PRESS)
	{
		*(Controlador::multiplicador) *= 1.1f;
	}

	// -
	else if (glfwGetKey (ventana, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
	{
		*(Controlador::multiplicador) *= 0.9f;
	}

	// n
	else if (glfwGetKey (ventana, GLFW_KEY_N) == GLFW_PRESS)
	{
		*(Controlador::orbitasAstros) = false;
	}

	// o
	else if (glfwGetKey (ventana, GLFW_KEY_O) == GLFW_PRESS)
	{
		*(Controlador::orbitasAstros) = true;
	}

	// F1
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

