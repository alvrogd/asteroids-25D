#ifndef LECTOR_IMAGEN_H
#define LECTOR_IMAGEN_H

// Librer�as de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para conjuntos de datos
#include <vector>


class LectorImagen
{
public:

	/* M�todos */

	static unsigned int ficheroATextura (const char *ruta);

	static unsigned int cargarCubemap (std::vector<std::string> caras);
};

#endif
