#include "Shader.h"

Shader::Shader (const char *vertexShaderFichero, const char *fragmentShaderFichero)
{
	// Identificadores de los dos shaders a cargar
	unsigned int vertexShader;
	unsigned int fragmentShader;

	// Se carga el vertex shader
	cargarShader (vertexShader, vertexShaderFichero, GL_VERTEX_SHADER);

	// Se carga el fragment shader
	cargarShader (fragmentShader, fragmentShaderFichero, GL_FRAGMENT_SHADER);

	// Se crea el shader program
	this->ID = glCreateProgram ();

	// Se le vinculan los dos shaders creados
	glAttachShader (this->ID, vertexShader);
	glAttachShader (this->ID, fragmentShader);

	// Se compila el shader program
	glLinkProgram (this->ID);
	comprobarError (this->ID, 1);

	// Se eliminan los shaders puesto que ya han sido vinculados
	glDeleteShader (vertexShader);
	glDeleteShader (fragmentShader);
}

void Shader::usar () const
{
	// Se indica a OpenGL que emplee este shader
	glUseProgram (this->ID);
}

void Shader::setBool (const std::string &nombre, bool valor) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniform1i (localizador, (int)valor);
}

void Shader::setInt (const std::string &nombre, int valor) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniform1i (localizador, valor);
}

void Shader::setFloat (const std::string &nombre, float valor) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniform1f (localizador, valor);
}

void Shader::setVec2 (const std::string &nombre, const glm::vec2 &valor) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniform2fv (localizador, 1, &valor[0]);
}
void Shader::setVec2 (const std::string &nombre, float x, float y) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniform2f (localizador, x, y);
}

void Shader::setVec3 (const std::string &nombre, const glm::vec3 &valor) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniform3fv (localizador, 1, &valor[0]);
}
void Shader::setVec3 (const std::string &nombre, float x, float y, float z) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniform3f (localizador, x, y, z);
}

void Shader::setVec4 (const std::string &nombre, const glm::vec4 &valor) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniform4fv (localizador, 1, &valor[0]);
}
void Shader::setVec4 (const std::string &nombre, float x, float y, float z, float w)
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniform4f (localizador, x, y, z, w);
}

void Shader::setMat2 (const std::string &nombre, const glm::mat2 &matriz) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniformMatrix2fv (localizador, 1, GL_FALSE, glm::value_ptr(matriz));
}

void Shader::setMat3 (const std::string &nombre, const glm::mat3 &matriz) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniformMatrix3fv (localizador, 1, GL_FALSE, glm::value_ptr (matriz));
}

void Shader::setMat4 (const std::string &nombre, const glm::mat4 &matriz) const
{
	// Se busca la variable especificada por el nombre dado
	int localizador = glGetUniformLocation (this->ID, nombre.c_str ());

	if (localizador < 0)
	{
		std::cout << "ERROR::SHADER::PROGRAM::VARIABLE_UNIFORM_INCORRECTA" << std::endl;
		return;
	}

	// Se almacena en ella el valor dado
	glUniformMatrix4fv (localizador, 1, GL_FALSE, glm::value_ptr (matriz));
}

void Shader::cargarShader (unsigned int &shader, const char *nombreFichero, GLenum tipo)
{
	// Se genera el identificador del shader
	shader = glCreateShader (tipo);

	// Se vincula el código del shader al objeto creado
	vincularCodigo (shader, nombreFichero);

	// Se compila el shader
	glCompileShader (shader);
	comprobarError (shader, 0);
}

void Shader::vincularCodigo (unsigned int & shader, const char * nombreFichero)
{
	// Fichero a leer
	std::ifstream fichero;

	// Contenido leído del fichero
	std::string contenido;

	// Se activan las excepciones del fichero
	fichero.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Se abre el fichero
		fichero.open (nombreFichero);

		// Se crea un stream y se lee en él el contenido del fichero
		std::stringstream streamFichero;
		streamFichero << fichero.rdbuf ();

		// Se cierra el descriptor del fichero
		fichero.close ();

		// Se guarda el contenido del fichero
		contenido = streamFichero.str ();
	}

	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::LECTURA_FICHERO_FALLIDA" << std::endl;
		exit (1);
	}

	// Es necesario obtener el contenido como un "puntero const char *" para OpenGL
	const char *codigo = contenido.c_str ();

	// Se vincula el código del shader al objeto dado
	glShaderSource (shader, 1, &codigo, NULL);;
}

void Shader::comprobarError (unsigned int shader, int tipo) const
{
	// Variables en las que almacenar el estado de un shader
	int exito;
	char log[512];

	// Se obtiene el estado del shader dado
	glGetShaderiv (shader, GL_COMPILE_STATUS, &exito);

	// Si no se ha podido crear el shader
	if (!exito)
	{
		// Se obtiene un log con la información correspondiente al error y se muestra
		glGetShaderInfoLog (shader, 512, NULL, log);

		// Si se trata de un único shader
		if (tipo == 0)
		{
			std::cout << "ERROR::SHADER::COMPILACION_FALLIDA\n" << log << std::endl;
		}

		// Si se trata de un program shader
		else
		{
			std::cout << "ERROR::SHADER::PROGRAM::VINCULACION_FALLIDA\n" << log << std::endl;
		}

		exit (3);
	}
}
