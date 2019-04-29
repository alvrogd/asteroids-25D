#include "LectorImagen.h"

// Carga de imágenes
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
// Para cargar imágenes del disco
#include <stb_image.h>
#endif


// TODO documentar

unsigned int LectorImagen::ficheroATextura (const char * path, const std::string & directory)
{
	/*std::string filename = std::string (path);
	filename = directory + '/' + filename;*/

	// Se genera un ID de OpenGL para la textura a cargar
	unsigned int textureID;
	glGenTextures (1, &textureID);

	// Propiedades de la imagen a leer
	int width, height, nrComponents;

	//unsigned char *data = stbi_load (filename.c_str (), &width, &height, &nrComponents, 0);

	// Se carga la imagen dada, pasando referencias a las variables en las que almacenar sus propiedades
	unsigned char *data = stbi_load (path, &width, &height, &nrComponents, 0);

	// Si se ha leído correctamente la imagen
	if (data)
	{
		// Se obtiene el formato de color de la imagen
		GLenum format;

		if (nrComponents == 1)
		{
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}

		// Se activa el ID generado como una textura 2D para configurarla
		glBindTexture (GL_TEXTURE_2D, textureID);

		// Se genera la textura 2D a partir de los datos obtenidos del fichero
		glTexImage2D (GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		// Se generan automáticamente los mipmaps de la textura
		glGenerateMipmap (GL_TEXTURE_2D);

		// Si las coordenadas (s, t) no se encuentran en el rango (0.0f, 1.0f), se repiten las texturas (la otra opción
		// sería truncar los valores a dicho rango mediante GL_CLAMP)
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Se emplea un interpolado lineal entre mipmaps y texels en la minimización para reducir el aliasing producido
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Se emplea un filtrado lineal en la magnificación para reducir el aliasing producido
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Se libera la memoria ocupada por los datos leídos del fichero
		stbi_image_free (data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		/*std::cout << "Texture failed to load at filename: " << filename.c_str () << std::endl;*/

		// Se libera la memoria ocupada por los datos leídos del fichero
		stbi_image_free (data);
	}

	return textureID;
}

unsigned int LectorImagen::cargarCubemap (std::vector<std::string> caras)
{
	// Se genera un ID de OpenGL para la textura a cargar
	unsigned int textureID;
	glGenTextures (1, &textureID);

	// Se activa dicho ID como un mapa de texturas de un cubemap para configurarlo
	glBindTexture (GL_TEXTURE_CUBE_MAP, textureID);

	// Propiedades de la imagen a leer
	int width, height, nrChannels;

	// Se cargan todos los ficheros dados como caras del cubemap
	for (unsigned int i = 0; i < caras.size (); i++)
	{
		// Se carga la imagen iterada, pasando referencias a las variables en las que almacenar sus propiedades
		unsigned char *data = stbi_load (caras[i].c_str (), &width, &height, &nrChannels, 0);

		// Si se ha leído correctamente la imagen
		if (data)
		{
			// Se genera la textura para la cara iterada del cubemap a partir de los datos obtenidos del fichero (la
			// enumeración GL_TEXTURE_CUBE_MAP_POSITIVE_X permite obtener las demás caras sumándole un valor)
			glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			// Se libera la memoria ocupada por los datos leídos del fichero
			stbi_image_free (data);
		}
		else
		{
			//std::cout << "Cubemap texture failed to load at path: " << caras[i] << std::endl;
			std::cout << "fail" << std::endl;

			// Se libera la memoria ocupada por los datos leídos del fichero
			stbi_image_free (data);
		}
	}

	// Se emplea un interpolado lineal entre mipmaps y texels en la minimización para reducir el aliasing producido
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Se emplea un filtrado lineal en la magnificación para reducir el aliasing producido
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Si las coordenadas (s, t) no se encuentran en el rango (0.0f, 1.0f), se truncan los valores a dicho rango (la
	// otra opción sería repetir las texturas mediante GL_REPEAT)
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
