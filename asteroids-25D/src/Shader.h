#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:

	/* Constructor */

	Shader (const char *vertexShaderFile, const char *fragmentShaderFile);


	/* Getters & setters */

	unsigned int getID () const { return this->ID; }


	/* Methods */

	// So that OpenGL uses this shader during the rendering process
	void use () const;

	// To load values from the CPU into the shader
	void setBool (const std::string &name, bool value) const;
	void setInt (const std::string &name, int value) const;
	void setFloat (const std::string &name, float value) const;

	void setVec2 (const std::string &name, const glm::vec2 &value) const;
	void setVec2 (const std::string &name, float x, float y) const;
	void setVec3 (const std::string &name, const glm::vec3 &value) const;
	void setVec3 (const std::string &name, float x, float y, float z) const;
	void setVec4 (const std::string &name, const glm::vec4 &value) const;
	void setVec4 (const std::string &name, float x, float y, float z, float w);

	void setMat2 (const std::string &name, const glm::mat2 &matrix) const;
	void setMat3 (const std::string &name, const glm::mat3 &matrix) const;
	void setMat4 (const std::string &name, const glm::mat4 &matrix) const;


private:

	/* Attributes */

	// Shader program's ID inside OpenGL
	unsigned int ID;

	
	/* Methods */

	// They are all auxiliar methods used by the constructor
	void loadShader (unsigned int &shader, const char *filename, GLenum type);
	void linkCode (unsigned int &shader, const char *filename);
	void checkError (unsigned int shader, int type) const;
};

#endif
