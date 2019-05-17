#include "LectorImagen.h"


// Carga de imágenes
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
// Para cargar imágenes del disco
#include <stb_image.h>
#endif

#include <iostream>


unsigned int LectorImagen::ficheroATextura (const char *ruta)
{
	// Se genera un ID de OpenGL para la textura a cargar
	unsigned int idTextura;
	glGenTextures (1, &idTextura);

	// Variables en las que almacenar las propiedades de la imagen a leer
	int ancho, alto, numeroComponentes;

	// Se carga la imagen dada, pasando referencias a las variables en las que almacenar sus propiedades
	unsigned char *datos = stbi_load (ruta, &ancho, &alto, &numeroComponentes, 0);

	// Si se ha leído correctamente la imagen
	if (datos != NULL)
	{
		// Se obtiene el formato de color de la imagen en función del número de canales de los que se compone
		GLenum formato;

		if (numeroComponentes == 1)
		{
			formato = GL_RED;
		}

		else if (numeroComponentes == 3)
		{
			formato = GL_RGB;
		}

		else if (numeroComponentes == 4)
		{
			formato = GL_RGBA;
		}

		// Se activa el ID generado como una textura 2D para poder configurarla
		glBindTexture (GL_TEXTURE_2D, idTextura);

		// Se genera la textura 2D a partir de los datos obtenidos del fichero:
		//	- Textura 2D
		//	- Nivel de detalle (mipmap) 0
		//	- Formato leído
		//  - Ancho y alto leídos
		//	- El valor del parámetro "border" debe ser 0
		//	- Formato leído
		//	- Bytes sin signo
		//	- Datos leídos del fichero especificado
		glTexImage2D (GL_TEXTURE_2D, 0, formato, ancho, alto, 0, formato, GL_UNSIGNED_BYTE, datos);

		// Se generan automáticamente los mipmaps de la textura
		glGenerateMipmap (GL_TEXTURE_2D);

		// Si las coordenadas (s, t) no se encuentran en el rango (0.0f, 1.0f), se repiten las texturas (la otra
		// opción sería truncar los valores a dicho rango mediante GL_CLAMP)
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Se emplea un interpolado lineal entre mipmaps y texels en la minimización para reducir el aliasing
		// producido
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Se emplea un filtrado lineal en la magnificación para reducir el aliasing producido
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	else
	{
		std::cout << "ERROR::LECTOR_IMAGEN::FICHERO_A_TEXTURA::FICHERO_NO_LEIDO::" << ruta << std::endl;
	}

	// Se libera la memoria ocupada por los datos leídos del fichero
	stbi_image_free (datos);

	return idTextura;
}

unsigned int LectorImagen::cargarCubemap (std::vector<std::string> caras)
{
	// Se genera un ID de OpenGL para la textura a cargar
	unsigned int idTextura;
	glGenTextures (1, &idTextura);

	// Se activa dicho ID como un mapa de texturas de un cubemap para configurarlo
	glBindTexture (GL_TEXTURE_CUBE_MAP, idTextura);

	// Variables en las que almacenar las propiedades de la imagen a leer
	int ancho, alto, numeroComponentes;

	// Se cargan todos los ficheros dados como caras del cubemap
	for (unsigned int i = 0; i < caras.size (); i++)
	{
		// Se carga la imagen iterada, pasando referencias a las variables en las que almacenar sus propiedades
		unsigned char *datos = stbi_load (caras[i].c_str (), &ancho, &alto, &numeroComponentes, 0);

		// Si se ha leído correctamente la imagen
		if (datos != NULL)
		{
			// Se genera la textura para la cara iterada del cubemap a partir de los datos obtenidos del fichero:
			//	- La enumeración GL_TEXTURE_CUBE_MAP_POSITIVE_X permite obtener las demás caras sumándole un entero
			//	- Nivel de detalle (mipmap) 0
			//	- Formato RGB sin canal alpha
			//  - Ancho y alto leídos
			//	- El valor del parámetro "border" debe ser 0
			//	- Formato RGB sin canal alpha
			//	- Bytes sin signo
			//	- Datos leídos del fichero especificado
			glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE,
				datos);
		}
		else
		{
			std::cout << "ERROR::LECTOR_IMAGEN::CARGAR_CUBEMAP::FICHERO_NO_LEIDO::" << caras[i].c_str () << std::endl;
		}

		// Se libera la memoria ocupada por los datos leídos del fichero
		stbi_image_free (datos);
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

	return idTextura;
}
