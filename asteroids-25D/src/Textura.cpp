#include "Textura.h"

/*// Carga de imágenes
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
// Para cargar imágenes del disco
#include <stb_image.h>
#endif*/

#include <iostream>

Textura::Textura (const char *fichero, GLenum modo)
{
	// Se genera un índice para la textura
	glGenTextures (1, &(this->idTextura));

	// Se carga la textura
	cargarTextura (fichero, modo);
}

Textura::~Textura ()
{
	// Se destruye el índice en OpenGL de la textura
	glDeleteTextures (1, &(this->idTextura));
}

void Textura::activar (Shader *shader, const char *uniform, int unidadTextura) const
{
	// Se selecciona la unidad de textura dada
	glActiveTexture (GL_TEXTURE0 + unidadTextura);

	// Se vincula la textura en OpenGL
	glBindTexture (GL_TEXTURE_2D, this->idTextura);

	// Se guarda la variable uniform especificada del shader dado la unidad de textura
	shader->setInt (uniform, unidadTextura);
}

void Textura::desactivar (int unidadTextura)
{
	// Se selecciona la unidad de textura dada
	glActiveTexture (GL_TEXTURE0 + unidadTextura);

	// Se desvincula la textura 2D activa
	glBindTexture (GL_TEXTURE_2D, 0);
}

void Textura::cargarTextura (const char * fichero, GLenum modo)
{
	/*// Se vincula la textura para que las operaciones se realicen sobre ella
	glBindTexture (GL_TEXTURE_2D, this->idTextura);

	// Si las coordenadas (s, t) no se encuentran en el rango (0.0f, 1.0f), se repiten las texturas (la otra opción
	// sería truncar los valores a dicho rango mediante GL_CLAMP)
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Se emplea un filtrado lineal en la magnificación para reducir el aliasing producido
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Se emplea un interpolado lineal entre mipmaps y texels en la minimización para reducir el aliasing producido
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Se indica a la librería que invierta el eje Y a la hora de cargar la imagen (OpenGL interpreta que la coordenada
	// 0.0 de Y se encuentra abajo, pero las imágenes habitúan situarla arriba)
	stbi_set_flip_vertically_on_load (true);

	// Se lee la textura del fichero indicado; se indica '0' para no forzar un número determinado de canales
	int ancho, alto, numeroCanales;

	unsigned char *contenido = stbi_load (fichero, &ancho, &alto, &numeroCanales, 0);

	// Si se ha podido leer correctamente el fichero
	if (contenido != NULL)
	{
		// Se genera la textura (sin mipmaps)
		//	- Textura 2D sobre el ID vinculado
		//	- Nivel de mipmap (detalle) 0
		//	- Componentes RGB/RGBA
		//	- Ancho y alto leídos
		//	- Borde de 0 píxeles
		//	- Color RGB/RGBA
		//	- Bytes sin signo
		//	- Puntero a la imagen leída
		glTexImage2D (GL_TEXTURE_2D, 0, modo, ancho, alto, 0, modo, GL_UNSIGNED_BYTE, contenido);

		// Se generan automáticamente los mipmaps de la textura
		glGenerateMipmap (GL_TEXTURE_2D);
	}

	// En caso contario, se informa sobre el error
	else
	{
		std::cout << "ERROR::TEXTURA::FICHERO_LECTURA_INCORRECTA" << std::endl; 
	}

	// Se libera la memoria empleada para la lectura del fichero
	stbi_image_free (contenido);

	// Se desactiva la textura
	glBindTexture (GL_TEXTURE_2D, 0);*/
}
