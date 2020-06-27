#ifndef INITIALIZER_H
#define INITIALIZER_H

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// I/O
#include <iostream>

// Input handler
#include "Controller.h"


class Initializer
{
public:

	/* Methods */

	static void initializeOpenGL (GLFWwindow **window, const char *wTitle, int *wWidth, int *wHeight, float
		*aspectRatio);

	static void initializeGLFW ();

	static void createWindow (GLFWwindow **window, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const char
		*wTitle);

	static void initializeGLAD ();

	static void configureWindowResizing (GLFWwindow *window, int *wWidth, int *wHeight, float
		*aspectRatio);

	static void configureWindowInput (GLFWwindow *window);

	static void configureOpenGL ();
};

#endif