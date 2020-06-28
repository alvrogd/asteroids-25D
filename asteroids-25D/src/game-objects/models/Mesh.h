#ifndef MESH_H
#define MESH_H

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// To render itself
#include "../../utilities/Shader.h"

#include <vector>


// Vertex representation (struct as it benefits the principle of locality)
struct SVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinates;
};

// Texture representation (struct as it benefits the principle of locality)
struct STexture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:

	/* Constructor */

	Mesh (std::vector<SVertex> vertexes, std::vector<unsigned int> indexes, std::vector<STexture> textures);


	/* Getters & setters */

	std::vector<SVertex> & getVertexes ()
	{
		return this->vertexes;
	}

	void setVertexes (std::vector<SVertex> vertexes)
	{
		this->vertexes = vertexes;
	}

	std::vector<unsigned int> & getIndexes ()
	{
		return this->indexes;
	}

	void setIndexes (std::vector<unsigned int> indexes)
	{
		this->indexes = indexes;
	}

	std::vector<STexture> & getTextures ()
	{
		return this->textures;
	}

	void setTextures (std::vector<STexture> textures)
	{
		this->textures = textures;
	}

	unsigned int getVAO () const
	{
		return this->VAO;
	}

	void setVAO (unsigned int VAO)
	{
		this->VAO = VAO;
	}

	unsigned int getVBO () const
	{
		return this->VBO;
	}

	void setVBO (unsigned int VBO)
	{
		this->VBO = VBO;
	}

	unsigned int getEBO () const
	{
		return this->EBO;
	}

	void setEBO (unsigned int EBO)
	{
		this->EBO = EBO;
	}


	/* Methods */

	void draw (Shader *shader);


private:

	/* Attributes */

	// Mesh data
	std::vector<SVertex> vertexes;
	std::vector<unsigned int> indexes;
	std::vector<STexture> textures;

	// Render data
	unsigned int VAO, VBO, EBO;


	/* Methods */

	void configureMesh ();
};

#endif