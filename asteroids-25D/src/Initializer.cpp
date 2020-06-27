#include "Initializer.h"

void Initializer::initializeOpenGL (GLFWwindow **window, const char *wTitle, int *wWidth, int *wHeight,
	float *aspectRatio)
{
	Initializer::initializeGLFW ();

	Initializer::createWindow (window, *wWidth, *wHeight, wTitle);
	
	Initializer::initializeGLAD ();

	// The working OpenGL version is shown
	std::cout << glGetString (GL_VERSION) << std::endl;

	// This method configures both the window's initial dimensions and how to resize it
	Initializer::configureWindowResizing (*window, wWidth, wHeight, aspectRatio);

	Initializer::configureWindowInput (*window);
}

void Initializer::initializeGLFW ()
{
	glfwInit ();

	// OpenGL 3.3 core
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Initializer::createWindow (GLFWwindow **window, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const char
	*wTitle)
{
	*window = glfwCreateWindow (WINDOW_WIDTH, WINDOW_HEIGHT, wTitle, NULL, NULL);

	if (window == NULL)
	{
		std::cout << "ERROR::INITIALIZER::WINDOW_NOT_CREATED" << std::endl;

		glfwTerminate ();
		exit(EXIT_FAILURE);
	}

	// The OpenGL context is set to be the new window's context
	glfwMakeContextCurrent (*window);
}

void Initializer::initializeGLAD ()
{
	// Addresses of the graphics driver functions are loaded in order to use them through OpenGL
	if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress)) {
		perror ("ERROR::INITIALIZER::GLAD_NOT_INITIALIZED");
		exit (-1);
	}
}

void Initializer::configureWindowResizing (GLFWwindow *window, int *wWidth, int *wHeight, float
	*aspectRatio)
{
	// OpenGL needs to know the window's size (the first two parameters point out the lower-left corner coordinates)
	glViewport (0, 0, *wWidth, *wHeight);

	Controller::wWidth = wWidth;
	Controller::wHeight = wHeight;

	Controller::aspectRatio = aspectRatio;

	// GLFW needs a custom funcion in order to adapt the viewport when resizing the window
	glfwSetFramebufferSizeCallback (window, Controller::resizeWindow);
}

void Initializer::configureWindowInput (GLFWwindow * window)
{
	// Keyboard input is set to "sticky" mode
	glfwSetInputMode (window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

void Initializer::configureOpenGL ()
{
	// Z-buffer is enabled
	glEnable (GL_DEPTH_TEST);

	// Enabling back face culling
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
}
