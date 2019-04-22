#ifndef SHADER_H
#define SHADER_H

// E/S y procesamiento de ficheros y strings
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
	/* Constructor */

	Shader (const char *vertexShaderFichero, const char *fragmentShaderFichero);


	/* Getters y setters */

	unsigned int getID () const { return this->ID; }


	/* Métodos */

	// Para que OpenGL emplee este program shader
	void usar () const;

	// Métodos para cargar valores en el shader program

	void setBool (const std::string &nombre, bool valor) const;
	
	void setInt (const std::string &nombre, int valor) const;

	void setFloat (const std::string &nombre, float valor) const;

	void setVec2 (const std::string &nombre, const glm::vec2 &valor) const;
	void setVec2 (const std::string &nombre, float x, float y) const;

	void setVec3 (const std::string &nombre, const glm::vec3 &valor) const;
	void setVec3 (const std::string &nombre, float x, float y, float z) const;

	void setVec4 (const std::string &nombre, const glm::vec4 &valor) const;
	void setVec4 (const std::string &nombre, float x, float y, float z, float w);

	void setMat2 (const std::string &nombre, const glm::mat2 &matriz) const;

	void setMat3 (const std::string &nombre, const glm::mat3 &matriz) const;

	void setMat4 (const std::string &nombre, const glm::mat4 &matriz) const;


private:
	// Identificador del shader en OpenGL
	unsigned int ID;

	// Métodos auxiliares del constructor
	void cargarShader (unsigned int &shader, const char *nombreFichero, GLenum tipo);
	void vincularCodigo (unsigned int &shader, const char *nombreFichero);
	void comprobarError (unsigned int shader, int tipo) const;
};

#endif
