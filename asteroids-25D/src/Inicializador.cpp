#include "Inicializador.h"

void Inicializador::inicializarOpenGL (GLFWwindow **ventana, const char *nombreVentana, int *wWidth, int *wHeight,
	float *relacionAspecto)
{
	// Se inicializa GLFW
	Inicializador::inicializarGLFW ();

	// Se crea una ventana
	Inicializador::crearVentana (ventana, *wWidth, *wHeight, nombreVentana);
	
	// Se inicializa GLAD
	Inicializador::inicializarGLAD ();

	// Se informa de la implementaci�n de OpenGL que se emplear�
	std::cout << glGetString (GL_VERSION) << std::endl;

	// Se especifican las dimensiones de la vista inicial y c�mo redimensionarlas
	Inicializador::configurarDimensionadoVentana (*ventana, wWidth, wHeight, relacionAspecto);

	// Se configura el input de la ventana
	Inicializador::configurarInputVentana (*ventana);
}

void Inicializador::inicializarGLFW ()
{
	// Se inicializa la librer�a de GLFW
	glfwInit ();

	// Se indica que se va a usar OpenGL 3.3 core (se elimina la retrocompatibilidad con algunas funciones)
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Inicializador::crearVentana (GLFWwindow **ventana, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const char
	*nombreVentana)
{
	// Se crea una ventana
	*ventana = glfwCreateWindow (WINDOW_WIDTH, WINDOW_HEIGHT, nombreVentana, NULL, NULL);

	// Si no se ha podido crear la ventana
	if (ventana == NULL) {
		std::cout << "ERROR::INICIALIZADOR::VENTANA_NO_CREADA" << std::endl;
		glfwTerminate ();
		exit(-1);
	}

	// Se indica al hilo actual que establezca el contexto de la ventana creada como su contexto de OpenGL
	glfwMakeContextCurrent (*ventana);
}

void Inicializador::inicializarGLAD ()
{
	// Se inicializa GLAD, cargando las direcciones de las funciones del driver gr�fico para poder emplar las
	// funcionalidades de OpenGL
	if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress)) {
		perror ("ERROR::INICIALIZADOR::GLAD_NO_INICIALIZADO");
		exit (-1);
	}
}

void Inicializador::configurarDimensionadoVentana (GLFWwindow *ventana, int *wWidth, int *wHeight, float *relacionAspecto)
{
	// Se indica a OpenGL el tama�o de la ventana (los dos primeros par�metros son las coordenadas de la esquina inferior
	// izquierda
	glViewport (0, 0, *wWidth, *wHeight);

	// Se almacenan en el controlador las referencias al tama�o de la ventana
	Controlador::wWidth = wWidth;
	Controlador::wHeight = wHeight;

	// Se almacena en el controlador la referencia a la relaci�n de aspecto de la ventana
	Controlador::relacionAspecto = relacionAspecto;

	// Se indica la funci�n a ejecutar ante un redimensionado de la ventana
	glfwSetFramebufferSizeCallback (ventana, Controlador::redimensionarVentana);
}

void Inicializador::configurarInputVentana (GLFWwindow * ventana)
{
	// Se establece el input del teclado al modo "sticky"
	glfwSetInputMode (ventana, GLFW_STICKY_KEYS, GLFW_TRUE);
}

void Inicializador::configurarOpenGL ()
{
	// Se activa el Z-buffer
	glEnable (GL_DEPTH_TEST);

	// Se activa la ocultaci�n de caras traseras
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
}
