#include "Shader.h"

Shader::Shader (const char *vertexShaderFile, const char *fragmentShaderFile)
{
	// To store the IDs of the two shaders that will compose the shader program
	unsigned int vertexShader;
	unsigned int fragmentShader;

	loadShader (vertexShader, vertexShaderFile, GL_VERTEX_SHADER);
	loadShader (fragmentShader, fragmentShaderFile, GL_FRAGMENT_SHADER);

	// After loading both shaders, the shader program can now be created by attaching and compiling them
	this->ID = glCreateProgram ();

	glAttachShader (this->ID, vertexShader);
	glAttachShader (this->ID, fragmentShader);

	glLinkProgram (this->ID);
	checkError (this->ID, 1);

	// They can be deleted as they have already been linked
	glDeleteShader (vertexShader);
	glDeleteShader (fragmentShader);
}

void Shader::use () const
{
	// OpenGL will now use this shader program
	glUseProgram (this->ID);
}

void Shader::setBool (const std::string &name, bool value) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniform1i (location, (int)value);
}

void Shader::setInt (const std::string &name, int value) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniform1i (location, value);
}

void Shader::setFloat (const std::string &name, float value) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniform1f (location, value);
}

void Shader::setVec2 (const std::string &name, const glm::vec2 &value) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniform2fv (location, 1, &value[0]);
}

void Shader::setVec2 (const std::string &name, float x, float y) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniform2f (location, x, y);
}

void Shader::setVec3 (const std::string &name, const glm::vec3 &value) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniform3fv (location, 1, &value[0]);
}

void Shader::setVec3 (const std::string &name, float x, float y, float z) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniform3f (location, x, y, z);
}

void Shader::setVec4 (const std::string &name, const glm::vec4 &value) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniform4fv (location, 1, &value[0]);
}

void Shader::setVec4 (const std::string &name, float x, float y, float z, float w)
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniform4f (location, x, y, z, w);
}

void Shader::setMat2 (const std::string &name, const glm::mat2 &matrix) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniformMatrix2fv (location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat3 (const std::string &name, const glm::mat3 &value) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniformMatrix3fv (location, 1, GL_FALSE, glm::value_ptr (value));
}

void Shader::setMat4 (const std::string &name, const glm::mat4 &matrix) const
{
	int location = glGetUniformLocation (this->ID, name.c_str ());

	if (location < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::UNIFORM_VARIABLE_NOT_FOUND::" << name << std::endl;
		return;
	}

	glUniformMatrix4fv (location, 1, GL_FALSE, glm::value_ptr (matrix));
}

void Shader::loadShader (unsigned int &shader, const char *filename, GLenum type)
{
	// Each shader needs its own ID inside OpenGL
	shader = glCreateShader (type);

	linkCode (shader, filename);

	glCompileShader (shader);
	checkError (shader, 0);
}

void Shader::linkCode (unsigned int & shader, const char * filename)
{
	std::ifstream file;
	std::string fileContents;

	// Certain file input exceptions are enabled to check errors
	file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open (filename);

		std::stringstream fileStream;
		fileStream << file.rdbuf ();

		file.close ();

		fileContents = fileStream.str ();
	}

	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_READ_FAILED" << std::endl;
		exit (1);
	}

	// OpenGL requires the file's contents to be provided as a "const char *" data type
	const char *code = fileContents.c_str ();

	// At last, the read code can be linked to the given shader
	glShaderSource (shader, 1, &code, NULL);;
}

void Shader::checkError (unsigned int shader, int type) const
{
	int success;
	char log[512];

	// Retrieving the shader's compilation status
	glGetShaderiv (shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog (shader, 512, NULL, log);

		// Single shader
		if (type == 0)
		{
			std::cout << "ERROR::SHADER::SINGLE::COMPILATION_FAILED" << std::endl << log << std::endl;
		}

		// Shader program (double shader)
		else
		{
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl << log << std::endl;
		}

		exit (3);
	}
}
