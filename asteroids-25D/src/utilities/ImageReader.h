#ifndef IMAGE_READER_H
#define IMAGE_READER_H

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>


class ImageReader
{
public:

	/* Methods */

	static unsigned int textureFromFile (const char *path);

	static unsigned int loadCubemap (std::vector<std::string> faces);
};

#endif
