#include "ImageReader.h"


// Image loading from disk
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include <iostream>


unsigned int ImageReader::textureFromFile (const char *path)
{
	// The texture needs its own OpenGL ID
	unsigned int textureId;
	glGenTextures (1, &textureId);

	// The texture's image is loaded from disk
	int width, height, componentCount;
	unsigned char *data = stbi_load (path, &width, &height, &componentCount, 0);

	// If the image has been successfully read
	if (data != NULL)
	{
		// The color format is obtained through its component count
		GLenum format;

		if (componentCount == 1)
		{
			format = GL_RED;
		}

		else if (componentCount == 3)
		{
			format = GL_RGB;
		}

		else if (componentCount == 4)
		{
			format = GL_RGBA;
		}

		// The texture is linked in order to configure it
		glBindTexture (GL_TEXTURE_2D, textureId);

		//	- 2D texture
		//	- Detail level (mipmap) 0
		//	- Various read properties
		//	- The value in the "border" parameter must be 0
		//	- File stored as unsigned bytes
		glTexImage2D (GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		// OpenGL takes care of generating the texture's mipmaps
		glGenerateMipmap (GL_TEXTURE_2D);

		// If the coordinates (s, t) are not in the (0.0f, 1.0f) interval, the texture gets repeated
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Lineal interpolation between mipmaps and texels in the minification process in order to reduce the aliasing
		// effect
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Lineal filtering in the magnification process in order to reduce the aliasing effect
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	else
	{
		std::cout << "ERROR::IMAGE_READER::TEXTURE_FROM_FILE::FILE_NOT_READ::" << path << std::endl;
	}

	// The raw data from disk is no longer needed
	stbi_image_free (data);

	return textureId;
}

unsigned int ImageReader::loadCubemap (std::vector<std::string> faces)
{
	// The texture needs its own OpenGL ID
	unsigned int textureId;
	glGenTextures (1, &textureId);

	// The ID is linked as a cubemap's texture map in order to configure it
	glBindTexture (GL_TEXTURE_CUBE_MAP, textureId);

	int width, height, componentCount;

	// Each given file is loaded from disk as one of the cubemap's faces
	for (unsigned int i = 0; i < faces.size (); i++)
	{
		unsigned char *data = stbi_load (faces[i].c_str (), &width, &height, &componentCount, 0);

		// If the image has been successfully read
		if (data != NULL)
		{
			// The texture for the current cubemap's face is generated using the data read from disk:
			//	- The enum GL_TEXTURE_CUBE_MAP_POSITIVE_X allows pointing the remaining faces just by adding a certain integer
			//	- Detail level (mipmap) 0
			//	- RGB format, no alpha channel
			//	- Various read properties
			//	- The value in the "border" parameter must be 0
			//	- File stored as unsigned bytes
			glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
				data);
		}
		else
		{
			std::cout << "ERROR::IMAGE_READER::LOAD_CUBEMAP::FILE_NOT_READ::" << faces[i].c_str () << std::endl;
		}

		// The raw data from disk is no longer needed
		stbi_image_free (data);
	}

	// Lineal interpolation between mipmaps and texels in the minification process in order to reduce the aliasing
	// effect
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Lineal filtering in the magnification process in order to reduce the aliasing effect
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// If the coordinates (s, t) are not in the (0.0f, 1.0f) interval, the textures are truncated
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureId;
}
